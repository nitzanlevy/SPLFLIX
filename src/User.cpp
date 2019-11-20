#include "../include/User.h"
#include "../include/Watchable.h"

//
// Created by amit on 18/11/2019.
//
std::vector<Watchable*> User::get_history() const {
    return this->history;
}
User::User(const std::string &name) : name(name) {

}
std::string User::getName() const {
    return name;
}

User::~User() {

}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    int avgLength(0); //average watch time
    int historyLength=this->get_history().size(); //num of content the user watched
    for(int i=0;i<historyLength;i++) {
        Watchable *watchable=this->get_history()[i];
        avgLength=avgLength+watchable->getLength();
    }
    avgLength=avgLength/historyLength;
}



