#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Watchable.h"

using std::string;

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    virtual ~BaseAction();
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction * clone() const=0;
    virtual void copy_from(BaseAction* toCopy);
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
    bool isValid(string& str);//added
    string getStatusString() const;//added
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session & sess);
    virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session & sess);
    virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act (Session& sess);
    virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act (Session& sess);
    virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class PrintActionsLog : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    virtual BaseAction * clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};
#endif