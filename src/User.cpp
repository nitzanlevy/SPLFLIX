#include "../include/User.h"

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

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    int avgLength(0);
    int historyLength=this->get_history().size();
    for(int i=0;i<historyLength;i++) {
        Watchable& watch = *(this->history.at(i));
        avgLength = avgLength + watch.getLength();
    }
    avgLength=avgLength/historyLength;
}

