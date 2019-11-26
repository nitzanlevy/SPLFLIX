//
// Created by amit on 18/11/2019.
//
#include <sstream>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

using std::string;

BaseAction::BaseAction() {
    status=PENDING;
    errorMsg="";
}

void BaseAction ::complete() {
    this->status=COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    this->status=ERROR;
    this->errorMsg=errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
 ActionStatus BaseAction::getStatus() const {
     return status;
}

BaseAction::~BaseAction() {

}

//function to check if string include only characters
bool BaseAction::isValid(string& check) {
    bool verify = true;
    for (int i = 0; i < check.length(); i++)
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

void BaseAction::copy_from(BaseAction* toCopy) {
    this->status=toCopy->status;
    this->errorMsg=toCopy->errorMsg;
}

//CreateUser
void CreateUser::act(Session &sess) {
    sess.addAction(this); //push the action with pending status
    string action=sess.getAction();
    if(action.find(" ")!=-1)
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
            this->error("The user name is already exist");
    }
    else
        this->error("name or code is missing");

}

std::string CreateUser::toString() const {
    return "CreateUser: "+getStatusString() + this->getErrorMsg();
}

BaseAction *CreateUser::clone() const {
    return new CreateUser(*this);
}

//Watch- ******need to fix this function.******
void Watch::act(Session &sess) {
    //watch - now recommend
    sess.addAction(this); //push the action with pending status
    std::stringstream geek(sess.getAction());
    int id;
    geek>>id;
    Watchable* watchable=sess.getWatchable(id); //watchable holds the watchable we want to watch
    sess.getActiveUser()->addToHistory(watchable); //became seen
    watchable=watchable->getNextWatchable(sess); //getNextWatchable
    this->complete();
    std::cout <<"we recommend you to watch: "+ watchable->toString() + "continue? [y/n]";
    string command;
    getline(std::cin, command);
    if(command=="y") {
        sess.setAction(std::to_string(watchable->getId()));
        Watch *watch = new Watch();
        watch->act(sess);
    }
    //Remmember to delete!!!
}

std::string Watch::toString() const {
    return "Watching: "+getStatusString()+ this->getErrorMsg();
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
        error("User not exists");
    }
}

std::string ChangeActiveUser::toString() const {
    return "Change Active User: "+getStatusString()+ this->getErrorMsg();
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
        error("User not exists");
    }
}

std::string DeleteUser::toString() const {
    return "Delete user: "+ getStatusString()+ this->getErrorMsg();
}

BaseAction *DeleteUser::clone() const {
    return new DeleteUser(*this);
}
//End Delete User

//Duplicate User
void DuplicateUser::act(Session &sess) {
    sess.addAction(this);
    string action=sess.getAction();//get the new name from action!!!
    if(action.find(" ")!=-1) {
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
            error("User not exists");
        }
    } else error("unvalid Input");

}
std::string DuplicateUser::toString() const {
    return "Duplicate User: "+ getStatusString()+ this->getErrorMsg();
}

BaseAction *DuplicateUser::clone() const {
    return new DuplicateUser(*this);
}
//end Duplicate user

//start print content list
void PrintContentList::act(Session &sess) {
    sess.addAction(this);
    for(auto & i : sess.getContent()) {
        std::cout << i->toString();
    }
    this->complete();
}
std::string PrintContentList::toString() const {
    return "Print Content List: " + getStatusString()+ this->getErrorMsg()+",";
}

BaseAction *PrintContentList::clone() const {
    return new PrintContentList(*this);
}
//end print content list

//print watch history
void PrintWatchHistory::act(Session &sess) {
    sess.addAction(this);
    std::cout<< "Watch history for:"+sess.getActiveUser()->getName() <<std::endl;
    for(auto & i : sess.getActiveUser()->getHistory()) {
        std::cout << i->toString();
    }
    this->complete();
}
std::string PrintWatchHistory::toString() const {
    return "Print Watch History: " + getStatusString()+ this->getErrorMsg()+",";
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
    return "Print Action log: " + getStatusString()+ this->getErrorMsg()+",";
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
    return "Exit: " + getStatusString()+ this->getErrorMsg();
}

BaseAction *Exit::clone() const {
    return new Exit(*this);
}

//end of exit
