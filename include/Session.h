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
    Session(const Session &); //Copy constructor
    Session&operator=(const Session&); //Copy assignment operator
    Session &operator=(Session &&); //move assignment operator
    ~Session();
    void start();
    User * getActiveUser(); //added to get recommendation
    std::vector<Watchable*>& getContent();// added
    Watchable * getWatchable(int watchId);// added
    void addUser(const string&,User*);//added
    string getAction();//added
    void addAction(BaseAction*);//added
    User* getUser(const string& userName);
    void setNewActiveUser(User*);
    void deleteUser(const string& userName);
    std::vector<BaseAction*>& getActionLog();
    void setAction(const string&);
    void stopRunning();
    int contentSize();// added
    bool userExist(const string& userName);
    void addActionString(const string & action);
    std::vector<std::string> &getLogString();
private:
    bool isNumber(const string& check);
    bool isValid(const string& check);
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    string action; //added
    bool continueToRun; //added
    std::vector<std::string> logString;
};
#endif