//
// Created by amit on 18/11/2019.
//
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <fstream>
using std::string;

Session::Session(const std::string &configFilePath){
    using json = nlohmann::json ;
    std::ifstream i(configFilePath);
    json j;
    i>>j;
    int id=0;
    json movies=j["movies"];
    for (int k = 0; k <movies.size() ; k++) {
        json & js=movies[k];
        content.push_back(new Movie(id,js["name"],js["length"],js["tags"]));
        id++;
    }

    json tv=j["tv_series"]; //takes all series
    for (int k = 0; k <tv.size() ; k++) {
        json & jt=tv[k]; //take a series
        json & seas=jt["seasons"]; // num Of Seasons
        for (int i = 0; i < seas.size(); ++i) {
            for (int l = 0; l <seas[i] ; ++l) {
                content.push_back(new Episode(id,jt["name"],jt["episode_length"],i,l,jt["tags"]));
                id++;
            }
        }
    }
}

void Session::start() {

}

Session::~Session() {
    for(auto & i : this->content)
        delete i;
    for (auto & i : this->actionsLog)
        delete i;
    for(auto & x:this->userMap)
        delete x.second;
}
User& Session::getUser() {
    return *activeUser;
}

std::vector<Watchable *> Session::getContent() {
    return content;
}

Watchable* Session::getWatchable(int watchId) {
    for(auto & i : this->content) {
        if (i->getId() == watchId)
            return i;
    }
    return nullptr;
}

string Session::getAction() {
    return action;
}

//add user to the userMap
void Session::addUser(string name, User* user) {
    this->userMap.insert({name,user});
}
void Session::addAction(BaseAction * baseAction) {
    this->actionsLog.push_back(baseAction);
}

User *Session::getUser(const string& userName) {
    for(auto & i : this->userMap) {
        if (i.second->getName() == userName)
            return i.second;
    }
    return nullptr;
}

void Session::setNewActiveUser(User * user) {
    activeUser=user;
}

void Session::deleteUser(const string& userName) {
    for(auto & i : this->userMap) {
        if (i.second->getName() == userName) {
            delete i.second; //delete the user
            userMap.erase(userMap.find(i.first));
        }
    }
}
