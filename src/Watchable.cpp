//
// Created by amit on 18/11/2019.
//
#include "../include/Watchable.h"
//Watchable methods
Watchable* Watchable ::getNextWatchable(Session & session) const {

}
Watchable ::Watchable(long id, int length, const std::vector<std::string> &tags):id(id),length(length) {

}//check on tags

std::string Watchable::toString() const {
    printf("%ld,%d\n",id,length);
}

Watchable::~Watchable() {

}
//end of watchable methods

//start Movie methods
Watchable* Movie::getNextWatchable(Session & session) const {

}
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags){

}
//end movie methods

//start Episode methods
Watchable* Episode::getNextWatchable(Session & session) const {

}
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :Watchable(id,length,tags),season(season),episode(episode){
    //nextEpisodeId needs a value!
}
//end episode methods


