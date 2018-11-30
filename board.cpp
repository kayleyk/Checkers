#include <iostream>
#include "space.cpp"
class board {
    space*** checkerboard;
    public:
    // directions based on the perspective of player's side of the board.
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
        if (y1 >= 0 && y1 < 8 && x1 >= 0 && x1 < 8) {
            // if their color piece was chosen
            if (checkerboard[y1][x1] != NULL && checkerboard[y1][x1]->getOwner() == currUser) {
                // if user is B. (can move any normal piece forwards. if at end it is a king)
                return true;
            }
        }
        return false;
    }
    bool isNextPosValid(short y1, short x1, short y2, short x2, short k) {
        isJumping = NONE;
        // check if in bounds of array.
        if (y2 >= 0 && y2 < 8 && x2 >= 0 && x2 < 8) {
            // if the chosen location is a real space and there is no owner of the space.
            if (checkerboard[y2][x2] != NULL && checkerboard[y2][x2]->getOwner() == NULL) {
                // moving left OR right one space, and up/down depending on user
                if (abs(x1 - x2) == 1 && y2 == (y1 + k)) {
                        return true;
                // trying to jump. (two spaces up/down depending on user)
                } else if ((y1 - y2) == -2 * k) {
                    // moving right, checks if piece being captured is owned by other user
                    if (((x2 - x1) == 2) && checkerboard[y1 + k][x1 + 1]->getOwner() == otherUser) {
                        setJumping(RIGHT);
                        return true;
                        // if moving left, checks if piece being captured is owned by other user
                    } else if (((x2 - x1) == -2) && checkerboard[y1 + k][x1 - 1]->getOwner() == otherUser) {
                        setJumping(LEFT);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isMoveValid(short y1, short x1, short y2, short x2, short k) {
        if (isPieceValid(y1, x1)) {
            if (isNextPosValid(y1, x1, y2, x2, k)) {
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
        } else if (getJumping() == LEFT) {
            checkerboard[y1 + k][x1 - 1]->setOwner(NULL);
        }
        std::cout << currUser->getOwner();
        print();
    }
    void start() {
        short y1, x1, y2, x2, y3, x3;
        char a;
        char isAnotherMove = 'N';
        bool isMoveValid = true, jumpedAPiece = false;
        currUser = user2;
        otherUser = user1;
        std::cout << user1->getOwner() << ", your piece is: " << user1->getPiece() << " your king is: " << user1->getKing() << "\n";
        std::cout << user2->getOwner() << ", your piece is: " << user2->getPiece() << " your king is: " << user2->getKing() << "\n";
        std::cout << "ready to start? press any key to continue and hit enter.";
        std:: cin >> a;
        print();
         while(true) {
            short turnCount = 0;
            jumpedAPiece = false;
            isMoveValid = true;
            setJumping(NONE);
            // switch to next player's turn;
            currUser = currUser == user1 ? user2 : user1;
            otherUser = otherUser == user1 ? user2 : user1;
            // positive for user 1, negative for user2 (user1 moves pieces down, user2 moves pieces up)
            short k = currUser == user1 ? 1 : -1;
            std::cout << currUser->getOwner() << "'s turn\n";
            do {
                isAnotherMove = 'N';
                std::cout << "choose a row (1-8): ";
                std::cin >> y1;
                std::cout << "choose a column: ";
                std::cin >> x1;
                std::cout << "choose the row you want the piece to move: ";
                std::cin >> y2;
                std::cout << "choose the column: ";
                std::cin >> x2;
                y1--;
                x1--;
                y2--;
                x2--;
            do {
                if (turnCount > 0 && jumpedAPiece) {
                    std::cout << "choose the row you want the piece to move: ";
                    std::cin >> y3;
                    std::cout << "choose the column: ";
                    std::cin >> x3;
                    // convert from 1 - 8 to 0 - 7.
                    y3--;
                    x3--;

                    // if not first turn, previously jumped, and next pos is valid
                    if (this->isNextPosValid(y2, x2, y3, x3, k)) {
                        isMoveValid = true;
                        movePiece(y2, x2, y3, x2, k);
                        std::cout << "you captured a piece! jump again? Y/N: ";
                        std::cin >> isAnotherMove;
                        turnCount++;
                    } else {
                        std::cout << "invalid try again? Y/N: ";
                        std::cin >> isAnotherMove;
                    }
                } else if (this->isMoveValid(y1, x1, y2, x2, k)) {
                    isMoveValid = true;
                    // if their first move, can move or jump.
                    if (turnCount == 0) {
                        turnCount++;
                        movePiece(y1, x1, y2, x2, k);
                        // if jumped, can jump again.
                        if (getJumping() != NONE) {
                            std::cout << "you captured a piece! jump again? Y/N: ";
                            std::cin >> isAnotherMove;
                            // set first move to a jump.
                            jumpedAPiece = true;
                        }
                        // if not first move then they must jump again.
                    }
                        // if jumped first move and move invalid, choose to jump again
                } else if (jumpedAPiece) {
                    std::cout << "invalid. Try again? Y/N: ";
                    std::cin >> isAnotherMove;
                    // first move invalid, has to move again.
                } else {
                        isMoveValid = false;
                        std::cout<<"move is invalid. try again.";
                    }
                } while (isAnotherMove == 'Y' || isAnotherMove == 'y');
            } while(!isMoveValid);
        }
    }
};