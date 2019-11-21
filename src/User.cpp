#include "../include/User.h"
#include "../include/Watchable.h"

//
// Created by amit on 18/11/2019.
//
/*std::vector<Watchable*> User::get_history() const {
    return this->history;
}*/
User::User(const std::string &name) : name(name) {

}
std::string User::getName() const {
    return name;
}

User::~User() { //delete history pointers
    for(auto & i : this->history) {
        delete i;
    } //delete all pointers

}

User::User(const User & user) {
    this->history=user.history;
}

User &User::operator=(const User & user) {
    if (this == &user) {  //check for "self assignment"
        return *this;
    }

    for(auto & i : this->history) //destroy old list
        delete i;

    for(auto  i : user.history) //possible &
        history.push_back(i);

    return *this;
}

int User::getHistorySize() {
    return history.size();
}

Watchable *User::getWatchableAt(int index) {
    return history.at(index);
}


Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    int avgLength(0); //average watch time
    int historyLength=this->history.size(); //num of content the user watched
    for(int i=0;i<historyLength;i++) {
        Watchable *watchable=this->history[i];
        avgLength=avgLength+watchable->getLength();
    }
    avgLength=avgLength/historyLength;
}



