#include <iostream>
#include <string>
using namespace std;

string ssid = "GET 192.19.124.2/301/game?player1=A&player2=5";

int main() {
    cout << "Begin" << endl;
    string url = "GET 192.19.124.2/301/game?player1=A&player2=5";
    //string url="http://qwert.mjgug.ouhnbg:5678/path1/path2.html";
    size_t found = url.find_first_of("=");
    string player1=url.substr(found+1); 

    cout<<player1<<endl;
    cout << "Done" << endl;
    return 0;
}