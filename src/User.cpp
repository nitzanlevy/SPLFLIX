#include "../include/User.h"

//
// Created by amit on 18/11/2019.
//
std::vector<Watchable*> User::get_history() const {

}
User::User(const std::string &name) : name(name) {

}
std::string User::getName() const {
    return name;
}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    int avgLength(0);
    int historyLength=this->get_history().size();
    for(int i=0;i<historyLength;i++)
        avgLength=avgLength+this->get_history().;
    avgLength=avgLength/historyLength;
}

