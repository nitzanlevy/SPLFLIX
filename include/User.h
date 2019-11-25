#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Watchable.h"

class Watchable;
class Session;

class User : public clonable{ //should implement rule of five
public:
    User(const std::string& name); //constructor
    virtual ~User(); //destructor
    User(const User &); //Copy constructor
    User&operator=(const User&); //Copy assignment operator
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    //std::vector<Watchable*> get_history() const;
    Watchable* getWatchableAt(int index); //added
    int getHistorySize(); //added
    std::vector<Watchable*>& getHistory();
    User (User &&); //move assignment constructor
    User &operator=(User &&); //move assignment operator
    void addToHistory( Watchable* watch);
    void setName(std::string);
    virtual User * clone() const=0;
protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    virtual User * clone() const;
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
};

class RerunRecommenderUser : public User {
public:
    virtual User * clone() const;
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
    int indexOfLastRecommendation;
};

class GenreRecommenderUser : public User {
public:
    virtual User * clone() const;
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
};

#endif