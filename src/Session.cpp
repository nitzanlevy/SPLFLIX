//
// Created by amit on 18/11/2019.
//
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <fstream>
#include <sstream>

using std::string;
using namespace std;

Session::Session(const std::string &configFilePath) : content(),actionsLog(),userMap(),activeUser(),action(),continueToRun(){
    using json = nlohmann::json ;
    std::ifstream i(configFilePath);
    json j;
    i>>j;
    long id=1;
    json movies=j["movies"];
    for (int k = 0; (unsigned) k <movies.size() ; k++) {
        json & js=movies[k];
        content.push_back(new Movie(id,js["name"],js["length"],js["tags"]));
        id++;
    }

    json tv=j["tv_series"]; //takes all series
    for (int k = 0; (unsigned)k <tv.size() ; k++) {
        json & jt=tv[k]; //take a series
        json & seas=jt["seasons"]; // num Of Seasons
        for (int i = 0;(unsigned) i < seas.size(); ++i) {
            for (int l = 0; l <seas[i] ; ++l) {
                content.push_back(new Episode(id,jt["name"],jt["episode_length"],i+1,l+1,jt["tags"]));
                id++;
            }
        }
    }
    continueToRun=true; //keep running until otherwise
    LengthRecommenderUser *defaultUser=new LengthRecommenderUser("default");
    this->userMap.insert({defaultUser->getName(),defaultUser});
    this->activeUser=defaultUser;
}

void Session::start() {
    std::cout<< "SPLFLIX is now on!";
    arrangePointers();
    while (continueToRun){
        cout<<""<<endl;// get down a line each time
        string command;
        cout<<"enter a command:"<<endl;
        getline(cin, command);
        setAction(command);
        //command with 1 word
        if((signed) command.find(" ") ==-1) {
            if(this->isValid(command)) {
                //handle the command
                if (command == "exit") {
                    Exit *exit = new Exit();
                    exit->act(*this);
                }
                else if (command == "log") {
                    PrintActionsLog *printActionsLog = new PrintActionsLog();
                    printActionsLog->act(*this);
                }
                else if (command == "watchhist") {
                    PrintWatchHistory *printWatchHistory = new PrintWatchHistory();
                    printWatchHistory->act(*this);
                }
                else if (command == "content") {
                    PrintContentList *printContentList = new PrintContentList();
                    printContentList->act(*this);
                }
                else
                    cout << "Invalid input, try again";
            }
            else
                cout << "Invalid input, try again";
        }
        //command with 2 words or more
        else {
            string theAct = command.substr(0, command.find(" "));
            string info = command.substr(command.find(" ") + 1, command.length());
            //command with 2 words
            if((signed) info.find(" ") ==-1) {
                if (this->isValid(theAct) && theAct.size() > 0 && info.size() > 0) {
                    if(theAct=="changeuser" && this->isValid(info)){
                        this->action=info;
                        ChangeActiveUser* changeActiveUser=new ChangeActiveUser();
                        changeActiveUser->act(*this);
                        //cout<<changeActiveUser->toString();
                    }
                    else if(theAct=="deleteuser" && this->isValid(info)){
                        this->action=info;
                        DeleteUser* deleteUser=new DeleteUser();
                        deleteUser->act(*this);
                        //cout<<deleteUser->toString();
                    }
                    else if(theAct=="watch" && this->isNumber(info))
                    {
                        std::stringstream geek(info);
                        int id;
                        geek>>id;
                        if( 0 < id && (unsigned) id <= this->content.size()) {
                            this->action = info;
                            Watch *watch = new Watch();
                            watch->act(*this);
                            //cout<<watch->toString();
                        }
                        else
                            cout << "Invalid input, try again";
                    }
                    else
                        cout << "Invalid input, try again";
                }
                else
                    cout << "Invalid input, try again";
            }
            //command with 3 words
            else{
                string part1 = info.substr(0, info.find(" "));
                string part2 = info.substr(info.find(" ") + 1, info.length());
                if (this->isValid(theAct) && this->isValid(part1) && this->isValid(part2) && theAct.size() > 0 && part1.size() > 0 && part2.size()>0) {
                    if(theAct=="createuser"){
                        this->action=info;
                        CreateUser* createUser=new CreateUser();
                        createUser->act(*this);
                        //cout << createUser->toString();
                    }
                    else if(theAct=="dupuser"){
                        this->action=info;
                        DuplicateUser* duplicateUser=new DuplicateUser();
                        duplicateUser->act(*this);
                        //cout<<duplicateUser->toString();
                    }
                    else
                        cout << "Invalid input, try again";
                }
                else
                    cout << "Invalid input, try again";
            }
        }
        if (!continueToRun){
            cout<<"rerun? [y/n]"<<endl;
            string rerun;
            getline(cin,rerun);
            if (rerun=="y")
                continueToRun= true;
        }
    }
}


Session::~Session() {
    for(auto & i : this->content)
        delete i;
    for (auto & i : this->actionsLog)
        delete i;
    for(auto & x:this->userMap)
        delete x.second;
    this->activeUser= nullptr; //point to null after deletion
    content.clear();
    actionsLog.clear();
    userMap.clear();
}

User * Session::getActiveUser() const {
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
void Session::addUser(const string& name, User* user) {
    this->userMap.insert({name,user});
}
void Session::addAction(BaseAction * baseAction) {
    this->actionsLog.push_back(baseAction);
}

User *Session::getUser(const string& userName) const {
    for(auto & i : this->userMap) {
        if (i.first == userName)
            return i.second;
    }
    return nullptr;
}

void Session::setNewActiveUser(User * user) {
    activeUser=user;
}

void Session::deleteUser(const string& userName) {
    delete getUser(userName);
    for(auto  &i : this->userMap) {
        if (i.first == userName) {
            userMap.erase(userMap.find(i.first));
            break;
        }
    }
}

int Session::contentSize() {
    return this->content.size();
}


std::vector<BaseAction *>&Session::getActionLog() {
    return actionsLog;
}

void Session::setAction(const string& newAction) {
        this->action=newAction;
}

void Session::stopRunning() {
    continueToRun= false;
}
bool Session::userExist(const string &userName) {
    for(auto & i : this->userMap) {
        if(i.first==userName)
            return true;
    }
    return false;
}

//function to check if string include only characters
bool Session::isValid(const string& check) {
    bool verify = true;
    for (int i = 0; (unsigned) i < check.length(); i++)
    {
        if ((check[i] >= 'a' && check[i] <= 'z') || (check[i] >= 'A' && check[i] <= 'Z'))
            verify = true;
        else
        {
            verify = false;
            break;
        }
    }
    return verify;
}

bool Session::isNumber(const string& str)
{
    for(int i = 0;(unsigned) i < str.size();i++) {
        return str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' ||
               str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9';
    }
    return false;
}

Session &Session::operator=(Session && other) { //move assignment operator

    for(auto &i:this->actionsLog){ //destroy old lists
        delete i;
    }
    this->actionsLog.clear();
    for(auto &i:this->userMap){
        delete i.second;
    }
    this->userMap.clear();
    for(auto &i:this->actionsLog){
        delete i;
    }
    this->actionsLog.clear();
    //now start moving and deleting other resources
    this->activeUser=other.activeUser;
    other.activeUser= nullptr; //added
    this->continueToRun=other.continueToRun;
    //now dealing with pointers, we set this pointer to the obj, and other pointer to null.
    for (auto &i:other.actionsLog) {
        this->actionsLog.push_back(i);
        i= nullptr;
    }
    other.actionsLog.clear(); //clean junk values
    for (auto &i:other.userMap) {
        this->userMap.insert(i);
        i.second=nullptr;
    }
    other.userMap.clear();
    for (auto &i:other.content) {
        this->content.push_back(i);
        i= nullptr;
    }
    other.content.clear();
    return *this;
}

Session &Session::operator=(const Session & other) { //copy assignment operator
    if (this==&other)
        return *this;
    for(auto &i:this->actionsLog){ //destroy old lists
        delete i;
    }
    this->actionsLog.clear();
    for(auto &i:this->userMap){
        delete i.second;
    }
    this->userMap.clear();
    for(auto &i:this->actionsLog){
        delete i;
    }
    this->actionsLog.clear();
    //start copying from other
    for(auto & i:other.actionsLog){
        this->actionsLog.push_back(i->clone());
    }
    for(auto & i:other.content){
        this->content.push_back(i->clone());
    }
    for(auto & i:other.userMap){
        if (i.first!=other.getActiveUser()->getName())
            this->userMap.insert({i.first,i.second->clone()}); //second needs clone
        else{
            User *toAdd=i.second->clone();
            activeUser=toAdd;
            userMap.insert({toAdd->getName(),toAdd});
            toAdd= nullptr;
        }
    }
    //done copying
    return *this; //added statement
}

Session::Session(const Session & other): content(),actionsLog(),userMap(),activeUser(),action(),continueToRun() { //copy constructor
    //content:
    for(auto & i : other.content) {
        this->content.push_back(i->clone());
    }
    //this->activeUser=other.activeUser->clone(); //added
    //action log
    for(auto & i : other.actionsLog) {
        this->actionsLog.push_back(i->clone());
    }
    // user map
    for(auto & i : other.userMap) {
        if (i.first!=other.getActiveUser()->getName())
            this->userMap.insert({i.first,i.second->clone()}); //second needs clone
        else{
            User *toAdd=i.second->clone();
            activeUser=toAdd;
            userMap.insert({toAdd->getName(),toAdd});
            toAdd= nullptr;
        }
    }

    //run
    this->continueToRun= true;
    // action - no need

}

Session::Session(Session && other): content(),actionsLog(),userMap(),activeUser(),action(),continueToRun(){ //move constructor
    this->activeUser=other.activeUser;
    other.activeUser= nullptr; //added
    this->continueToRun=other.continueToRun;
    //now dealing with pointers, we set this pointer to the obj, and other pointer to null.
    for (auto &i:other.actionsLog) {
        this->actionsLog.push_back(i);
        i= nullptr;
    }
    other.actionsLog.clear(); //clean junk values
    for (auto &i:other.userMap) {
        this->userMap.insert(i);
        i.second=nullptr;
    }
    other.userMap.clear();
    for (auto &i:other.content) {
        this->content.push_back(i);
        i= nullptr;
    }
    other.content.clear();
}

Session *Session::clone() const {
    return new Session(*this);
}

void Session::arrangePointers() { //when a session start, we gotta make sure users pointing to new watchables addresses
    for(auto &i:userMap){
        for (auto &j:i.second->getHistory()) {
            long id=j->getId();
            for (auto &k:content) {
                if (id==k->getId()) {
                    j = k;
                    break;
                }
            }
        }
    }
}

