#include <iostream>
#include "../include/Session.h"
#include "../include/Watchable.h"
using namespace std;

int main(int argc, char** argv){
    /*if(argc!=2)
    {
        cout << "usage splflix input_file" << endl;
        return 0;
    }*/
    Session s("/home/nitzan/Desktop/SPLFLIX/config1.json"); //Session s(argv[1])!!!!
    s.start();
    return 0;
}
