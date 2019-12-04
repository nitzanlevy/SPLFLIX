//
// Created by amit on 18/11/2019.
//
#include <sstream>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

using std::string;

BaseAction::BaseAction() :errorMsg(),status() {
    status=PENDING;
    errorMsg="";
}

void BaseAction ::complete() {
    this->status=COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    this->status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<this->errorMsg; //let the user know when error occurs.
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::~BaseAction() {

}

//function to check if a string includes only characters
bool BaseAction::isValid(string& check) {
    bool verify = true;
    for (int i = 0; (unsigned) i < check.length(); i++)
    {
        if ((check[i] >= 'a' && check[i] <= 'z') || (check[i] >= 'A' && check[i] <= 'Z'))
            verify = true;
        else
        {
            verify = false;
            break;
        }
    }
    return verify;
}

std::string BaseAction::getStatusString() const {
    if (status==PENDING){
        return "Pending";
    }
    if (status==COMPLETED){
        return "Completed";
    }
    return "Error";
}

//CreateUser
void CreateUser::act(Session &sess) {
    sess.addAction(this); //push the action with pending status
    string action=sess.getAction();
    if((signed) action.find(" ")!=-1)
    {
        string name = action.substr(0, action.find(" "));
        string code = action.substr(action.find(" ") + 1, action.length());
        if(!sess.userExist(name)) {
            if (isValid(name) && code.size() == 3) {
                if (code == "len") {
                    sess.addUser(name, new LengthRecommenderUser(name));
                    this->complete();
                } else if (code == "rer") {
                    sess.addUser(name, new RerunRecommenderUser(name));
                    this->complete();
                } else if (code == "gen") {
                    sess.addUser(name, new GenreRecommenderUser(name));
                    this->complete();
                } else
                    this->error("code is invalid");
            }
            else
                this->error("name or code is missing");
        }
        else
            this->error("username already exists");
    }
    else
        this->error("name or code is missing");

}

std::string CreateUser::toString() const {
    return "CreateUser: "+getStatusString() +" " + this->getErrorMsg();
}

BaseAction *CreateUser::clone() const {
    return new CreateUser(*this);
}

//Watch
void Watch::act(Session &sess) {
    sess.addAction(this); //push the action with pending status
    std::stringstream geek(sess.getAction());
    int id;
    geek>>id;
    Watchable* watchable=sess.getWatchable(id); //watchable holds the watchable we want to watch
    if(watchable->isEpisode()) {
        std::cout << "Watching " + ((Episode*)(watchable))->getName() +" S"+((Episode*)(watchable))->getNumSeason()+"E"+((Episode*)(watchable))->getNumEpisode() +"\n";
    }
    else{
        std::cout << "Watching " + ((Movie*)(watchable))->getName() +"\n";

    }
    sess.getActiveUser()->addToHistory(watchable->clone()); //became seen
    this->complete(); //watchable seen and added
    watchable=watchable->getNextWatchable(sess); //getNextWatchable
    if(!watchable){
        std::cout<<"you have watched all content";
        return;
    }
    string s=watchable->toString();
    std::cout <<"We recommend watching"+ s.substr(s.find(" ")) + "[y/n]\n";
    string command;
    getline(std::cin, command);
    if(command=="y") {
        sess.setAction(std::to_string(watchable->getId()));
        Watch *watch = new Watch();
        watch->act(sess);
    }
}

std::string Watch::toString() const {
    return "Watching: "+getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *Watch::clone() const {
    return new Watch(*this);
}

//Change User
void ChangeActiveUser::act(Session &sess) {
    sess.addAction(this); //push the action with pending status
    string action=sess.getAction(); //the User name to switch to.
    if (sess.getUser(action)){
        sess.setNewActiveUser(sess.getUser(action));
        this->complete();
    } else{
        error("User does not exist");
    }
}

std::string ChangeActiveUser::toString() const {
    return "Change Active User: "+getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *ChangeActiveUser::clone() const {
    return new ChangeActiveUser(*this);
}
//end Change User

//Delete User
void DeleteUser::act(Session &sess) {
    sess.addAction(this);
    string action=sess.getAction();
    if (sess.getUser(action)){
        sess.deleteUser(action);
        this->complete();
    } else{
        error("User does not exist");
    }
}

std::string DeleteUser::toString() const {
    return "Delete user: "+ getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *DeleteUser::clone() const {
    return new DeleteUser(*this);
}
//End Delete User

//Duplicate User
void DuplicateUser::act(Session &sess) {
    sess.addAction(this);
    string action=sess.getAction();//get the new name from action
    if((signed)action.find(" ")!=-1) {
        string existingUser = action.substr(0, action.find(" "));
        string newOne = action.substr(action.find(" ") + 1, action.length());
        if (sess.userExist(newOne))
            error("user already exists");
        else if (sess.getUser(existingUser)) {
            User *newUser=sess.getUser(existingUser)->clone();
            newUser->setName(newOne);
            sess.addUser(newUser->getName(),newUser);
            this->complete();
        } else {
            error("User does not exist");
        }
    } else error("invalid Input");

}
std::string DuplicateUser::toString() const {
    return "Duplicate User: "+ getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *DuplicateUser::clone() const {
    return new DuplicateUser(*this);
}
//end Duplicate user

//start print content list
void PrintContentList::act(Session &sess) {
    sess.addAction(this);
    for(auto & i : sess.getContent()) {
        std::cout << i->toString() + "\n";
    }
    this->complete();
}
std::string PrintContentList::toString() const {
    return "Print Content List: " + getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *PrintContentList::clone() const {
    return new PrintContentList(*this);
}
//end print content list

//print watch history
void PrintWatchHistory::act(Session &sess) {
    sess.addAction(this);
    std::cout<< "Watch history for:"+sess.getActiveUser()->getName() <<std::endl;
    int counter=1;
    for(auto & i : sess.getActiveUser()->getHistory()) {
        std::cout <<std::to_string(counter)+". "+ i->toString() +"\n";
        i=i+1;
    }
    this->complete();
}
std::string PrintWatchHistory::toString() const {
    return "Print Watch History: " + getStatusString()+" "+ this->getErrorMsg()+",";
}

BaseAction *PrintWatchHistory::clone() const {
    return new PrintWatchHistory(*this);
}
//end of print watch history

//print action log
void PrintActionsLog::act(Session &sess) {
    sess.addAction(this);
    for (int i = sess.getActionLog().size()-1; i >=0 ; i=i-1) {
        BaseAction *action = sess.getActionLog().at(i);
        std::cout << action->toString() << std::endl;
    }
    this->complete();
}

std::string PrintActionsLog::toString() const {
    return "Print Action log: " + getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
//end of print action log

// Exit
void Exit::act(Session &sess) {
    sess.addAction(this);
    sess.stopRunning();
    this->complete();
}
std::string Exit::toString() const {
    return "Exit: " + getStatusString()+" "+ this->getErrorMsg();
}

BaseAction *Exit::clone() const {
    return new Exit(*this);
}

//end of exit
