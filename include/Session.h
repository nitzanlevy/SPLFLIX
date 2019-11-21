#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    User& getUser(); //added to get recommendation
    std::vector<Watchable*> getContent();// added
    Watchable * getWatchable(int watchId);// added
    string getAction();//added
    void addAction(BaseAction*);//added
    User* getUser(const string& userName);
    void setNewActiveUser(User*);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    string action; //added
};
#endif