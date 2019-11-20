#include <iostream>
#include "../include/Session.h"
#include "../include/Watchable.h"
using namespace std;

int main(int argc, char** argv){
    if(argc!=2) {
        cout << "usage splflix input_file" << endl;
        return 0;
    }
    /*vector<std::string> v1=*new vector<string>();
    string name="nitzan";
    Watchable *watch= new Movie(1,name,3,v1);
    cout << watch->getLength() << endl;
    cout <<watch->toString();*/
    //Session s(argv[1]);
    //s.start();
    return 0;
}