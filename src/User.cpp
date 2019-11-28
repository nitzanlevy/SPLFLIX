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

User::User(const std::string &name) : name(name), history() {}

std::string User::getName() const {
    return name;
}

User::~User() { //null history pointers, destructor
    for(auto & i : this->history) {
        i = nullptr;
    } //null all pointers
    history.clear(); //clean junk values
}

User::User(const User & user) : name(), history() { //copy constructor
    for(auto  i : user.history) //possible &
        history.push_back(i); //pass by pointer, not deepcopy
    this->name=user.name; // careful!!
}

User &User::operator=(const User & user) { //copy assignment operator
    if (this == &user) {  //check for "self assignment"
        return *this;
    }

    for(auto & i : this->history) //destroy old list
        i= nullptr;
    this->history.clear();
    for(auto & i : user.history) //possible &
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


User::User(User && other) : name(),history() { //move constructor
    this->name=other.name;
    for (auto &i :other.history) { //move pointer and destroy other's pointer
        this->history.push_back(i);
        i= nullptr;
    }
    other.history.clear(); //clear junk values
}

User &User::operator=(User &&other) { //move assignment operator
    for (auto &i:this->history) { //we don't delete, it points to content values, every user shares them.
        i= nullptr;
    }
    this->history.clear();//delete user history
    for (auto &i:other.history) {
        this->history.push_back(i);
        i= nullptr; //detach other's resources
    }
    other.history.clear();
    return *this;
}
void User::addToHistory(Watchable* watch) {
    this->history.push_back(watch);

}

void User::setName(std::string newName) {
    this->name=newName;
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
                if (i->toString()==j->toString()){
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

User *LengthRecommenderUser::clone() const {
    LengthRecommenderUser *newUSer=new LengthRecommenderUser(*this);
    return newUSer;
}


//RerunRecommenderUser functions
RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name),indexOfLastRecommendation() {
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



User *RerunRecommenderUser::clone() const {
    RerunRecommenderUser *newUser = new RerunRecommenderUser(*this);
    return newUser;
}
bool sortbyTag(const pair<string,int> &a,
               const pair<string,int> &b)
{
    return (a.first < b.first);
}
bool isEqual(pair<string, int>& element,string tag)
{
    return element.first == tag;
}
//GenreRecommenderUser functions
GenreRecommenderUser::GenreRecommenderUser(const std::string &name): User(name) {}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    if (getHistorySize()==0){
        return nullptr;
    }
    //insert all tags to freq
    unordered_map<string, int> freq;
    for (auto &i :s.getContent()) {
        for (auto &j:i->getTags()) {
            if (freq.find(j)==freq.end()){
                freq.insert({j,0});
            }
        }
    }
    //count the num of tag in freq
    for (auto &i :this->history) {
        for (auto &j:i->getTags()) {
            if (freq.find(j)!=freq.end()){
                freq.find(j)->second++;
            }
        }
    }
    //vector contains all the freq elements and now sorted by string tag
    vector<pair<string,int>> elems(freq.begin(), freq.end());
    sort(elems.begin(), elems.end(),sortbyTag);
    bool found= false;
    int maxTag=0;
    string mostTag="";
    Watchable *output;
    //start to search the mostTag in content
    while(!found) {
        //search the mostTag string in elems
        bool isAllNeg= true; //indicates weather all is -1 , means they were candidate to be nextRecommendation, but failed.
        for (auto &i : elems) {
            if (i.second > maxTag)
                mostTag = i.first;
            if (i.second>=0)
                isAllNeg= false;
        } //mostTag holds greatestTag
        if (isAllNeg) //all -1, nothing to recommend, you watched all.
            return nullptr;
        //check in content-history for the recommendation
        for ( auto &i : s.getContent()) {
            if(!found) {
                bool flag = false;
                for (auto &j : this->history) {
                    if (i == j) {
                        flag = true;
                        break;
                    } //flag = true -> he already watched the one we want to recommend
                }
                for (int k = 0; k < i->getTags().size() && !flag; k++) {
                    if (i->getTags().at(k) == mostTag) {
                        output = i;
                        found = true;
                    }
                }
            } else break;
        }
        //if not matching for mostTag we search for the nextTag by deleting the mostTag from elems
        if(!found) {
            /*for(auto it = elems.begin(); it!=elems.end(); it++ )
                if(it->first == mostTag) //added .base()!!
                    elems.erase(it);
            maxTag=0;*/
            for(auto &k :elems){
                if (k.first==mostTag){
                    k.second=-1; //setting k out of reach
                }
            }
            maxTag=0;
        }
    }
    freq.clear();
    elems.clear();
    return output;
}


User *GenreRecommenderUser::clone() const {
    GenreRecommenderUser *newUser=new GenreRecommenderUser(*this);
    return newUser;
}




