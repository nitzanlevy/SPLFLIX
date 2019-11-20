//
// Created by amit on 18/11/2019.
//
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

BaseAction::BaseAction() {

}

void BaseAction::act(Session &sess) {

}

void BaseAction ::complete() {

}

void BaseAction::error(const std::string &errorMsg) {

}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
 ActionStatus BaseAction::getStatus() const {
     return status;
}

std::string BaseAction::toString() const {
    std::cout << status +" "+errorMsg;
}

BaseAction::~BaseAction() {

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
