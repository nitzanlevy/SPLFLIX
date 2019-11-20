//
// Created by amit on 18/11/2019.
//
#include "../include/Watchable.h"
#include "../include/User.h"

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
//end of watchable methods

//start Movie methods

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(name){}

Movie::~Movie() {}

Watchable* Movie::getNextWatchable(Session & session) const {
    //if last seen=episode, return next, else: In episode
    //return session.getUser().getRecommendation(session);
}


std::string Movie::toString() const {
    return Watchable::toString() + "\nThe name is: "+ name;
}
//end movie methods

//start Episode methods
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName),nextEpisodeId(id+1){
    //nextEpisodeId needs a value!
}

Episode::~Episode(){}

Watchable* Episode::getNextWatchable(Session & session) const {
    return nullptr ;
}

std::string Episode::toString() const {
    return Watchable::toString()  ;
}
//end episode methods


