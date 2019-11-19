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

Watchable* User::getRecommendation(Session &s) {

}

