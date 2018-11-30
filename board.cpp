#include <iostream>
#include "space.cpp"
class board {
    space*** checkerboard;
    public:
    enum movement {LEFT, RIGHT, NONE};
    movement isJumping;
    user *user1, *user2, *currUser, *otherUser;

    movement getJumping() {
        return this->isJumping;
    }
    void setJumping(movement isJumping) {
        this-> isJumping = isJumping;
    }

    board(user *user1, user *user2) {
        this->user1 = user1;
        this->user2 = user2;
         checkerboard = new space**[8];
        for (int y = 0; y < 8; y++) {
            checkerboard[y] = new space*[8];
            for (int x = 0; x < 8; x++) {
                user *owner = NULL;
                if ((x + y) % 2 != 0) {
                    if (y < 3) {
                        owner = user1;
                    } else if (y > 4) {
                        owner = user2;
                    }
                    checkerboard[y][x] = new space(owner);
                } else {
                    checkerboard[y][x] = NULL;
                }
            }
        }
    }

    void print() {
        std::cout << "  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n-----------------------------------\n";
        for (int y = 0; y < 8; y++) {
            std::cout << (y+1);
            for (int x = 0; x < 8; x++) {
                std::cout << " | ";
                if (checkerboard[y][x] == NULL) {
                    std::cout << "/";
                } else {
                    std:: cout << (char)checkerboard[y][x]->getPiece();
                }
            }
            std::cout << " |\n-----------------------------------\n";
        }
    }
    bool isPieceValid(short y1, short x1) {
        // ignoring kings for now...
        // if the selected squre is part of the board (so as not to get seg fault).
        std:: cout << "9 ";
        if (y1 >= 0 && y1 < 8 && x1 >= 0 && x1 < 8) {
            std:: cout << "9 ";
            // if their color piece was chosen
            if (checkerboard[y1][x1] != NULL && checkerboard[y1][x1]->getOwner() == currUser) {
                std:: cout << "9 ";
                // if user is B. (can move any normal piece forwards. if at end it is a king)
                return true;
            }
        }
        return false;
    }
    bool isNextPosValid(short y1, short x1, short y2, short x2, short k) {
        isJumping = NONE;
        std:: cout << "9 ";
        // check if in bounds of array.
        if (y2 >= 0 && y2 < 8 && x2 >= 0 && x2 < 8) {
            std:: cout << "9 ";
            std::cout<<"is in array";
            // if the space object exists here, and there is no owner of the space.
            if (checkerboard[y2][x2] != NULL && checkerboard[y2][x2]->getOwner() == NULL) {
                std::cout<<"space exists, no owner of space";
                // pieces move down
                    // moving left OR right one space.
                    if (abs(x1 - x2) == 1) {
                        // down one space
                        if (y2 == (y1 + k)) {
                            return true;
                        }
                        // for both useres, trying to jump.
                    } else if ((y1 - y2) == -2 * k) {
                        // if moving right
                        if (((x2 - x1) == 2) && checkerboard[y1 + k][x1 + 1]->getOwner() == otherUser) {
                            isJumping = RIGHT;
                            return true;
                            // if moving left.
                        } else if (((x2 - x1) == -2) && checkerboard[y1 + k][x1 - 1]->getOwner() == otherUser) {
                            isJumping = LEFT;
                            return true;
                        }
                            // WORKING ON THIS PART!!! JUMPING A PIECE. CHECKING IF SPACE BETWEEN IS HELD BY OPPOSITE USER.
                    }
                /*} else { // is user2
                    std::cout<<"isplayer2";
                    // if moving down one space, and left OR right one space.
                    if (y2 == (y1 - 1) && (x2 == (x1 - 1) || (x2) == (x1 + 1))) {
                        std::cout<<"moving down and over 1";
                        return true;
                    }
                }*/
            }
        }
        return false;
    }

    bool isMoveValid(short y1, short x1, short y2, short x2, short k) {
        if (isPieceValid(y1, x1)) {
            std::cout<<"piece is valid";
            if (isNextPosValid(y1, x1, y2, x2, k)) {
                std::cout << "pos is valid";
                return true;
            }
        }
        return false;
    }
    void movePiece(short y1, short x1, short y2, short x2, short k) {
        checkerboard[y1][x1]->setOwner(NULL);
        checkerboard[y2][x2]->setOwner(currUser);
        if (getJumping() == RIGHT) {
            checkerboard[y1 + k][x1 + 1]->setOwner(NULL);
        } else if (isJumping == LEFT) {
            checkerboard[y1 + k][x1 - 1]->setOwner(NULL);
        }
        std::cout << currUser->getOwner();
    }
    void start() {
        short y1, x1, y2, x2;
        char a;
        char isAnotherMove = 'N';
        bool isMoveValid = true;
        currUser = user2;
        otherUser = user1;
        std::cout << user1->getOwner() << ", your piece is: " << user1->getPiece() << " your king is: " << user1->getKing() << "\n";
        std::cout << user2->getOwner() << ", your piece is: " << user2->getPiece() << " your king is: " << user2->getKing() << "\n";
        std::cout << "ready to start? press any key to continue and hit enter.";
        std:: cin >> a;
         while(true) {
            print();
            setJumping(NONE);
            // switch to next player's turn;
            currUser = currUser == user1 ? user2 : user1;
            otherUser = otherUser == user1 ? user2 : user1;
            // positive for user 1, negative for user2 (user1 moves pieces down, user2 moves pieces up)
            short k = currUser == user1 ? 1 : -1;
            std::cout << currUser->getOwner() << "'s turn\n";
            do {
                isAnotherMove = 'N';
                std::cout << "choose a row (1-8)";
                std::cin >> y1;
                std::cout << "choose a column";
                std::cin >> x1;
                do {
                    std::cout << "choose the row you want the piece to move";
                    std::cin >> y2;
                    std::cout << "choose the column";
                    std::cin >> x2;

                    // convert from 1 - 8 to 0 - 7.
                    y1--;
                    x1--;
                    y2--;
                    x2--;

                    if (this->isMoveValid(y1, x1, y2, x2, k)) {
                        movePiece(y1, x1, y2, x2, k);
                        if (getJumping() != NONE) {
                            std::cout << "jump again? Y or N";
                            std :: cin >> isAnotherMove;
                        }
                        isMoveValid = true;
                    } else {
                        isMoveValid = false;
                        std:: cout << "invalid. try again";
                        print();
                    }
                } while (isAnotherMove == 'Y' || isAnotherMove == 'y');
            } while(!isMoveValid);

        }

    }
};