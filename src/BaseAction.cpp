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

void BaseAction::setStatus(ActionStatus actionStatus) {
    this->status=actionStatus;
}

//Watch
void Watch::act(Session &sess) {
    //watch - now recommend
    int length = sess.getUser().get_history().size();
    Watchable *watchable=sess.getUser().get_history()[length-1];
    Movie *m= dynamic_cast<Movie*>(watchable);
    Episode *e=dynamic_cast<Episode*>(watchable);

    if (e){
        int nextId=e->getId()+1;
        Episode *episode=dynamic_cast<Episode*>(sess.getWatchable(nextId));
        if (episode && e->getName()== episode->getName())
            watchable=e;
        else
            watchable=sess.getUser().getRecommendation(sess);
        delete episode;
    }
    else
        watchable=sess.getUser().getRecommendation(sess);

    std::cout <<"we recommend you to watch: "+ watchable->toString() + "continue? [y/n]";
    delete watchable;
    delete m;
    delete e;
    //Remmember to delete!!!

}

std::string Watch::toString() const {
    return "Watching: ";
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

}
//End Delete User


