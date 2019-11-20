//
// Created by amit on 18/11/2019.
//
#include "../include/Action.h"
#include "../include/User.h"

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

//Create User
void CreateUser::act(Session &sess) {

}