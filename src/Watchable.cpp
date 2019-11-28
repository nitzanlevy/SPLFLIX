//
// Created by amit on 18/11/2019.
//
#include "../include/Watchable.h"

#include <utility>
#include "../include/User.h"
#include "../include/Session.h"
//Watchable methods
Watchable ::Watchable(long id, int length, std::vector<std::string> tags):id(id),length(length),tags(std::move(tags)) {}//check on tags

Watchable::~Watchable() {
    tags.clear();
}

int Watchable::getLength() const { return this->length;}

std::string Watchable::toString() const {}

long & Watchable::getId() const {
    return const_cast<long &>(id);
}

const std::vector<string>& Watchable::getTags() const {
    return tags;
}

//end of watchable methods

//start Movie methods

Movie::Movie(long id, std::string name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(std::move(name)){}

Watchable* Movie::getNextWatchable(Session & session) const {
    return session.getActiveUser()->getRecommendation(session);

}

std::string Movie::toString() const {
    string s;
    for(int i=0;(unsigned)i<this->getTags().size();i++)
        s=s+this->getTags().at(i)+", ";
    s=s.substr(0,s.length()-2);
    return std::to_string(this->getId()) + ". "+ name +" "+ std::to_string(this->getLength()) +" minutes ["+s+"]" ;
}

Watchable *Movie::clone() const {
    return new Movie(getId(),name,getLength(),getTags());
}

bool Movie::isEpisode() const {
    return false;
}

std::string Movie::getName() {
    return this->name;
}

//end movie methods

//start Episode methods
Episode::Episode(long id, std::string seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :Watchable(id,length,tags),season(season),episode(episode),seriesName(std::move(seriesName)),nextEpisodeId(id+1){
    //nextEpisodeId needs a value!
}

Watchable* Episode::getNextWatchable(Session & session) const {
    Watchable *nextWatchable=getNextEpisode(session);
    if (nextWatchable && nextWatchable->isEpisode()){
        if (((Episode*)nextWatchable)->seriesName==this->seriesName){
            return nextWatchable;
        }
    }
    return session.getActiveUser()->getRecommendation(session);
} //need to check last episode

std::string Episode::toString() const {
    string s;
    for(int i=0; (unsigned)i<this->getTags().size();i++)
        s=s+this->getTags().at(i)+", ";
    s=s.substr(0,s.length()-2);
    return std::to_string(this->getId()) + ". " + seriesName + " S" + std::to_string(this->season)+ "E" + std::to_string(this->episode) +" "+ std::to_string(this->getLength()) + " minutes ["+s+"]" ;
}

Watchable* Episode::getNextEpisode(Session &session) const {
    long id =this->getId()+1;
    for(auto & x:session.getContent()) {
        if (x->getId() == id)
            return x;
    }
    return nullptr;
}

std::string Episode::getName() {
    return seriesName;
}

Watchable *Episode::clone() const {
    return new Episode(getId(),seriesName,getLength(),season,episode,getTags());
}

bool Episode::isEpisode() const {
    return true;
}

std::string Episode::getNumSeason() const { return std::to_string(this->season);}
std::string Episode::getNumEpisode() const { return std::to_string(this->episode);}

//end episode methods


