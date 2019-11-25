//
// Created by amit on 18/11/2019.
//
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
//Watchable methods
Watchable ::Watchable(long id, int length, const std::vector<std::string> &tags):id(id),length(length),tags(tags) {}//check on tags

Watchable::~Watchable() {}

int Watchable::getLength() const { return this->length;}

std::string Watchable::toString() const {
   std::string s;
   s= "The Id is: " + std::to_string(this->id) + ", The length is: " + std::to_string(this->length) + ", The tags is:" ;
    for (int i = 0; i < tags.size(); i++) {
        s= s + " " + tags.at(i).data();
    }
    return s;
}

int Watchable::getId() const {
    return id;
}

const std::vector<string> Watchable::getTags() const {
    return tags;
}

//end of watchable methods

//start Movie methods

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(name){}

Movie::~Movie() {}

Watchable* Movie::getNextWatchable(Session & session) const {
    return session.getActiveUser()->getRecommendation(session);

}

std::string Movie::toString() const {
    return Watchable::toString() + ", The name is: "+ name + "\n";
}

Watchable *Movie::clone() const {
    return new Movie(getId(),name,getLength(),getTags());
}

bool Movie::isEpisode() const {
    return false;
}
//end movie methods

//start Episode methods
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName),nextEpisodeId(id+1){
    //nextEpisodeId needs a value!
}

Episode::~Episode(){}

Watchable* Episode::getNextWatchable(Session & session) const {
    Watchable *nextWatchable=getNextEpisode(session);
    if (nextWatchable->isEpisode()){
        if (((Episode*)nextWatchable)->seriesName==this->seriesName){
            return nextWatchable;
        }
    }
    return session.getActiveUser()->getRecommendation(session);
} //need to check last episode

std::string Episode::toString() const {
    return Watchable::toString() + ", The season is: " + std::to_string(season) + " The episode is: " + std::to_string(episode) + ", The seriesName is: "+ seriesName;
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

//end episode methods


