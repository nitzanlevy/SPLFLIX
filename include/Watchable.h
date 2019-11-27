#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;
struct clonable {
    virtual ~clonable() = default;
    virtual clonable* clone() const = 0;
};

class Watchable :public clonable{
public:
    Watchable(long id, int length, std::vector<std::string>  tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual int getLength() const; // added to use recommendation algorithms
    int getId() const ;//added
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
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual ~Movie();
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
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual ~Episode();
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