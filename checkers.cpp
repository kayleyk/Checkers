#include <iostream>
#include "user.cpp"
#include "board.cpp"
using namespace std;

int main () {
    string name;
    cout << "player 1 enter your name";
    cin >> name;
    user user1('B', '&', name);
    cout << "player 2 enter your name";
    cin >> name;
    user user2('W', '#', name);
    board board1(&user1, &user2);
    board1.start();

    return 0;
}