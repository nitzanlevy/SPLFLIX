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

void BaseAction::setStatus(ActionStatus actionStatus){
    this->status=actionStatus;
}
//function to check if string include only characters
bool BaseAction::isValid(string check) {
    bool verify = true;
    for (int i = 0; i < check.length(); i++)
    {
        if (check[i] >= 'a' && check[i] <= 'z' || check[i] >= 'A' && check[i] <= 'Z')
            verify = true;
        else
        {
            verify = false;
            break;
        }
    }
    return verify;
}

//CreateUser
void CreateUser::act(Session &sess) {
    //need to add function that push act to the actVector
    string action=sess.getAction();
    if(action.find(" ")!=-1)
    {
        string name = action.substr(0, action.find(" "));
        string code = action.substr(action.find(" ") + 1, action.length());
        if(isValid(name) && code.size()==3) {
            if (code == "len") {
                sess.addUser(name, new LengthRecommenderUser(name));
                this->setStatus(COMPLETED);
            }
            else if (code == "rer") {
                sess.addUser(name, new RerunRecommenderUser(name));
                this->setStatus(COMPLETED);
            }
            else if (code == "gen") {
                sess.addUser(name, new GenreRecommenderUser(name));
                this->setStatus(COMPLETED);
            }
            else
                this->error("code is invalid");
        }
        else
            this->error("name or code is missing");
    }
    else
        this->error("name or code is missing");

}

std::string CreateUser::toString() const {
    return " ";
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
