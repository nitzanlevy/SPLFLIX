#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{ //should implement rule of five
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
    User (User &&);
    User &operator=(User &&);
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
    int indexOfLastRecommendation;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
};

#endif