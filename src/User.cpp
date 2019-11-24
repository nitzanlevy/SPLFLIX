#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <algorithm>
using namespace std;
//
// Created by amit on 18/11/2019.
//
/*std::vector<Watchable*> User::get_history() const {
    return this->history;
}*/

User::User(const std::string &name) : name(name) {}

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

std::vector<Watchable *> &User::getHistory() {
    return history;
}


User::User(User && other) {
    for (auto &i:this->history) {
        delete i;
    } //delete user history
    this->history=other.history;
    delete &other.history;
}

User &User::operator=(User &&other) {
    for (auto &i:this->history) {
        delete i;
    } //delete user history
    this->history=other.history;
    delete &other.history;
    return *this;
}
void User::addToHistory(Watchable* watch) {
    this->history.push_back(watch);

}

//LengthRecommenderUser functions
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    if(s.contentSize()!=this->history.size()) {
        int avgLength(0); //average watch time
        int historyLength = this->history.size(); //num of content the user watched
        for (int i = 0; i < historyLength; i++)
            avgLength = avgLength + this->history[i]->getLength();
        avgLength = avgLength / historyLength;
        //avgLength now holds the average length, now we need to find the closest one which he hadn't seen
        int minDist=INT32_MAX;
        Watchable* output;
        for(auto & i : s.getContent()) {
            bool flag= false;
            for(auto & j : this->history) {
                if (i==j){
                    flag= true;
                    break;
                }
            }
            if (!flag && minDist>std::abs(i->getLength()-avgLength)){
                output=i;
                minDist=std::abs(i->getLength()-avgLength);
            }
        }
        return output;
    }
    else
        return nullptr;
}

//RerunRecommenderUser functions
RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name) {
    indexOfLastRecommendation=-1; //indexes that nothing had been recommended yet
}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if (getHistorySize()==0){
        return nullptr;
    } //history is empty, nothing to offer
    if (indexOfLastRecommendation==-1){
        indexOfLastRecommendation=0;
        return this->history[indexOfLastRecommendation];
    }
    return this->history[(indexOfLastRecommendation+1)%getHistorySize()];
}

//GenreRecommenderUser functions
GenreRecommenderUser::GenreRecommenderUser(const std::string &name): User(name) {}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    if (getHistorySize()==0){
        return nullptr;
    } //there is no genre to match to.
    unordered_map<string, int>* freq=new unordered_map<string,int >();
    /*for (auto &i :this->history) {
        for (auto &j:i->getTags()) {
            if (freq->find(j)=!freq->end()){

            }
            else{

            }
        }
    }*/
    return nullptr;
}




