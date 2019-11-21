#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
using std::string;

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    User * getActiveUser(); //added to get recommendation
    std::vector<Watchable*>& getContent();// added
    Watchable * getWatchable(int watchId);// added
    void addUser(string,User*);//added
    string getAction();//added
    void addAction(BaseAction*);//added
    User* getUser(const string& userName);
    void setNewActiveUser(User*);
    void deleteUser(const string& userName);
    int contentSize();
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    string action; //added
};
#endif