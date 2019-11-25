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
        string command;
        cout<<"enter a command:"<<endl;
        getline(cin, command);
        setAction(command);
        //command with 1 word
        if(command.find(" ") ==-1) {
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
            if(info.find(" ") ==-1) {
                if (this->isValid(theAct) && theAct.size() > 0 && info.size() > 0) {
                    if(theAct=="changeuser" && this->isValid(info)){
                        this->action=info;
                        ChangeActiveUser* changeActiveUser=new ChangeActiveUser();
                        changeActiveUser->act(*this);
                    }
                    else if(theAct=="deleteuser" && this->isValid(info)){
                        this->action=info;
                        DeleteUser* deleteUser=new DeleteUser();
                        deleteUser->act(*this);
                    }
                    else if(theAct=="watch" && this->isNumber(info))
                    {
                        std::stringstream geek(info);
                        int id;
                        geek>>id;
                        if(id < this->content.size()) {
                            this->action = info;
                            Watch *watch = new Watch();
                            watch->act(*this);
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
                        cout << createUser->toString();
                    }
                    else if(theAct=="dupuser"){
                        this->action=info;
                        DuplicateUser* duplicateUser=new DuplicateUser();
                        duplicateUser->act(*this);
                        cout<<duplicateUser->toString();
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
    //delete active user


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
void Session::addUser(const string& name, User* user) {
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
    delete getUser(userName);
    for(auto  &i : this->userMap) {
        if (i.second->getName() == userName) {
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
    for (int i = 0; i < check.length(); i++)
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
    for(int i = 0;i < str.size();i++) {
        if(str[i]=='0' || str[i]=='1'|| str[i]=='2' ||str[i]=='3' ||str[i]=='4' ||str[i]=='5' ||str[i]=='6'||str[i]=='7'||str[i]=='8'||str[i]=='9') {
            return true;
        } else {
            return false;
        }
    }
}

Session &Session::operator=(Session &&) {
    return *this;
}

Session &Session::operator=(const Session &) {
    return *this;
}

Session::Session(const Session &) {

}

