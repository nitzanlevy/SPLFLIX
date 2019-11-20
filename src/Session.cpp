//
// Created by amit on 18/11/2019.
//
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include <fstream>

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

}
User& Session::getUser() {
    return *activeUser;
}