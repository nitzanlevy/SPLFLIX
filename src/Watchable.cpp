//
// Created by amit on 18/11/2019.
//
#include "../include/Watchable.h"
//Watchable methods
Watchable* Watchable ::getNextWatchable(Session & session) const {

}
Watchable ::Watchable(long id, int length, const std::vector<std::string> &tags):id(id),length(length),tags(tags) {

}//check on tags

Watchable::~Watchable() {

}
int Watchable::getLength() const { return this->length;}

std::string Watchable::toString() const {
    printf("%ld,%d\n",id,length);
}
//end of watchable methods

//start Movie methods
Watchable* Movie::getNextWatchable(Session & session) const {

}
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(name){

}
int Movie::getLength() const {
    return Watchable::getLength();
}

std::string Movie::toString() const {
    return  Watchable::toString() + "nitzan";
}
//end movie methods

//start Episode methods
Watchable* Episode::getNextWatchable(Session & session) const {

}
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :Watchable(id,length,tags),season(season),episode(episode){
    //nextEpisodeId needs a value!
}
int Episode::getLength() const {
    return Watchable::getLength();
}
std::string Episode::toString() const {
    return Watchable::toString() +"amit";
}
//end episode methods


