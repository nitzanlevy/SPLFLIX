//
// Created by amit on 18/11/2019.
//
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

}

void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
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

void BaseAction::setStatus(ActionStatus actionStatus){
    this->status=actionStatus;
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
                    this->setStatus(COMPLETED);
                } else if (code == "rer") {
                    sess.addUser(name, new RerunRecommenderUser(name));
                    this->setStatus(COMPLETED);
                } else if (code == "gen") {
                    sess.addUser(name, new GenreRecommenderUser(name));
                    this->setStatus(COMPLETED);
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

//Watch- ******need to fix this function.******
void Watch::act(Session &sess) {
    //watch - now recommend
    sess.addAction(this); //push the action with pending status

    int length = sess.getActiveUser()->getHistorySize();
    Watchable* watchable=sess.getActiveUser()->getWatchableAt(length-1);

   

    Movie *m= dynamic_cast<Movie*>(watchable);
    Episode *e=dynamic_cast<Episode*>(watchable);

    if (e){
        int nextId=e->getId()+1;
        Episode *episode=dynamic_cast<Episode*>(sess.getWatchable(nextId));
        if (episode && e->getName()== episode->getName())
            watchable=e;
        else
            watchable=sess.getActiveUser()->getRecommendation(sess);
        delete episode;
    }
    else
        watchable=sess.getActiveUser()->getRecommendation(sess);

    std::cout <<"we recommend you to watch: "+ watchable->toString() + "continue? [y/n]";
    delete watchable;
    delete m;
    delete e;
    //Remmember to delete!!!
}

std::string Watch::toString() const {
    return "Watching: "+getStatusString()+ this->getErrorMsg();
}

//Change User
void ChangeActiveUser::act(Session &sess) {
    sess.addAction(this); //push the action with pending status
    string action=sess.getAction(); //the User name to switch to.
    if (sess.getUser(action)){
        sess.setNewActiveUser(sess.getUser(action));
        setStatus(COMPLETED);
    } else{
        error("User not exists");
    }
}

std::string ChangeActiveUser::toString() const {
    return "Change Active User: "+getStatusString()+ this->getErrorMsg();
}
//end Change User

//Delete User
void DeleteUser::act(Session &sess) {
    sess.addAction(this);
    string action=sess.getAction();
    if (sess.getUser(action)){
        sess.deleteUser(action);
        setStatus(COMPLETED);
    } else{
        error("User not exists");
    }
}

std::string DeleteUser::toString() const {
    return "Delete user: "+ getStatusString()+ this->getErrorMsg();
}
//End Delete User

//Duplicate User
void DuplicateUser::act(Session &sess) {
    sess.addAction(this);
    string action=sess.getAction();//get the new name from action!!!
    if(action.find(" ")!=-1) {
        string existingUser = action.substr(0, action.find(" "));
        string newOne = action.substr(action.find(" ") + 1, action.length());

        if (sess.getUser(existingUser)) {
            //learn how to know user type
            LengthRecommenderUser *user = dynamic_cast<LengthRecommenderUser *>(sess.getUser(existingUser));
            RerunRecommenderUser *user2 = dynamic_cast<RerunRecommenderUser *>(sess.getUser(existingUser));
            GenreRecommenderUser *user3 = dynamic_cast<GenreRecommenderUser *>(sess.getUser(existingUser));
            //use instanceOf
            if (user) {
                LengthRecommenderUser *newUser= new LengthRecommenderUser(newOne);
                newUser->operator=(*(dynamic_cast<LengthRecommenderUser*>(sess.getUser(existingUser))));
                sess.addUser(newOne,newUser);
            }
            if (user2) {
                RerunRecommenderUser *newUser = new RerunRecommenderUser(newOne);
                newUser->operator=(*(dynamic_cast<RerunRecommenderUser*>(sess.getUser(existingUser))));
                sess.addUser(newOne,newUser);
            }
            if (user3) {
                GenreRecommenderUser *newUser = new GenreRecommenderUser(newOne);
                newUser->operator=(*(dynamic_cast<GenreRecommenderUser*>(sess.getUser(existingUser))));
                sess.addUser(newOne,newUser);
            }
            delete user; //CareFull!
            delete user2;
            delete user3;
            setStatus(COMPLETED);
        } else {
            error("User not exists");
        }
    } else error("unvalid Input");

}
std::string DuplicateUser::toString() const {
    return "Duplicate User: "+ getStatusString()+ this->getErrorMsg();
}
//end Duplicate user

//start print content list
void PrintContentList::act(Session &sess) {
    sess.addAction(this);
    for(auto & i : sess.getContent()) {
        std::cout << i->toString();
    }
    setStatus(COMPLETED);
}
std::string PrintContentList::toString() const {
    return "Print Content List: " + getStatusString()+ this->getErrorMsg();
}
//end print content list

//print watch history
void PrintWatchHistory::act(Session &sess) {
    sess.addAction(this);
    std::cout<< "Watch history for:"+sess.getActiveUser()->getName();
    for(auto & i : sess.getActiveUser()->getHistory()) {
        std::cout << i->toString();
    }
    setStatus(COMPLETED);
}
std::string PrintWatchHistory::toString() const {
    return "Print Watch History: " + getStatusString()+ this->getErrorMsg();
}
//end of print watch history

//print action log
void PrintActionsLog::act(Session &sess) {
    sess.addAction(this);
    for (int i = sess.getActionLog().size()-1; i >=0 ; --i) {
        std::cout<<sess.getActionLog().at(i)->toString();
    }
    setStatus(COMPLETED);
}

std::string PrintActionsLog::toString() const {
    return "Print Action log: " + getStatusString()+ this->getErrorMsg();
}
//end of print action log

// Exit
void Exit::act(Session &sess) {
    sess.addAction(this);
    sess.stopRunning();
    setStatus(COMPLETED);
}
std::string Exit::toString() const {
    return "Exit: " + getStatusString()+ this->getErrorMsg();
}
//end of exit
