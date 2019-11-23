//
// Created by amit on 18/11/2019.
//
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <fstream>
using std::string;
using namespace std;

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
    continueToRun=true; //keep running until otherwise
}

void Session::start() {
    std::cout<< "SPLFLIX is now on!";
    setNewActiveUser(new LengthRecommenderUser("deafult"));
    while (continueToRun){
        cout<<""<<endl;// get down a line each time
        cout<<"enter a command:"<<endl;
        string command;
        cin>>command;
        setAction(command);
        //handle the command
        if (command=="exit"){
            Exit *exit=new Exit();
            exit->act(*this);
            delete exit;
        }
        if (command=="log"){
            PrintActionsLog *printActionsLog=new PrintActionsLog();
            printActionsLog->act(*this);
            delete printActionsLog;
        }
        if (command=="watchhist"){
            PrintWatchHistory * printWatchHistory=new PrintWatchHistory();
            printWatchHistory->act(*this);
            delete printWatchHistory;
        }
        if (command=="content"){
            PrintContentList* printContentList=new PrintContentList();
            printContentList->act(*this);
            delete printContentList;
        }
        // until here is one word commands, now we need to split
    }
}

Session::~Session() {
    for(auto & i : this->content)
        delete i;
    for (auto & i : this->actionsLog)
        delete i;
    for(auto & x:this->userMap)
        delete x.second;
}

User * Session::getActiveUser() {
    return activeUser;

}

std::vector<Watchable *> &Session::getContent() {
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

int Session::contentSize() {
    return this->content.size();
}

std::vector<BaseAction *> &Session::getActionLog() {
    return actionsLog;
}

void Session::setAction(const string& newAction) {
        this->action=newAction;
}

void Session::stopRunning() {
    continueToRun= false;
}
