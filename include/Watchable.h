#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, std::vector<std::string>  tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual int getLength() const; // added to use recommendation algorithms
    long & getId() const;//added , changed to long
    const std::vector<std::string>& getTags() const ;
    virtual Watchable * clone() const=0;
    virtual bool isEpisode() const=0;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, std::string  name, int length, const std::vector<std::string>& tags);
    //destructor deleted
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Watchable *clone() const ;
    virtual bool isEpisode() const ;
    std::string getName();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, std::string  seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    //destructor deleted
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    std::string getName(); //added
    virtual Watchable *clone() const ;
    virtual bool isEpisode() const;
    std::string getNumSeason() const ;
    std::string getNumEpisode() const ;

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
    Watchable* getNextEpisode(Session&) const ; //added

};

#endif