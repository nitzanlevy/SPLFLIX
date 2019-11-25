#include <iostream>
#include "../include/Session.h"
#include "../include/Watchable.h"
using namespace std;

int main(int argc, char** argv){
        Session* session=new Session("/home/nitzan/Desktop/SPLFLIX/config1.json");
        session->start();
        return 0;
}

    //Session *session=new Session("/home/nitzan/Desktop/SPLFLIX/config1.json");
    /*vector<std::string> v1=*new vector<string>();
    if(argc!=2) {
        cout << "usage splflix input_file" << endl;
    string name="nitzan";
    Watchable *watch= new Movie(1,name,3,v1);
    cout << watch->getLength() << endl;
    cout <<watch->toString();*/
    //Session s(argv[1]);
    //s.start();
