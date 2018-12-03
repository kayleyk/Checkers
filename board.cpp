#include <iostream>
#include "space.cpp"
class board {
    space*** checkerboard;
    public:
    user *user1, *user2, *currUser, *otherUser;
    board(user *user1, user *user2, int boardSize) : BOARD_SIZE(boardSize) {
        this->user1 = user1;
        this->user2 = user2;
        checkerboard = new space**[this->BOARD_SIZE];
        for (int y = 0; y < this->BOARD_SIZE; y++) {
            checkerboard[y] = new space*[this->BOARD_SIZE];
            for (int x = 0; x < this->BOARD_SIZE; x++) {
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
    private:
    enum movement {LEFT, RIGHT, NONE};
    movement isJumping;
    const int BOARD_SIZE;

    movement getJumping() {
        return this->isJumping;
    }
    void setJumping(movement isJumping) {
        this-> isJumping = isJumping;
    }
    void print() {
        std::cout << "  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n-----------------------------------\n";
        for (int y = 0; y < this->BOARD_SIZE; y++) {
            std::cout << (y+1);
            for (int x = 0; x < this->BOARD_SIZE; x++) {
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
        // if selected square is in within board size.
        if (y1 >= 0 && y1 <  this->BOARD_SIZE && x1 >= 0 && x1 < this->BOARD_SIZE) {
            // if their color piece was chosen
            if (checkerboard[y1][x1] != NULL && checkerboard[y1][x1]->getOwner() == currUser) {
                return true;
            }
        }
        return false;
    }
    bool isNextPosValid(short y1, short x1, short y2, short x2, short k) {
        isJumping = NONE;
        // check if in board size.
        if (y2 >= 0 && y2 < this->BOARD_SIZE && x2 >= 0 && x2 < this->BOARD_SIZE) {
            // set y direction player is trying to move
            short yDirMoving = y2 - y1 > 0 ? 1 : -1;
            // if the piece is a king, set true.
            bool isKing = checkerboard[y1][x1]->getKing();
            // if the chosen location is a real space, there is no owner of the space, and is moving valid y direction.
            if (checkerboard[y2][x2] != NULL && checkerboard[y2][x2]->getOwner() == NULL && (isKing || yDirMoving == k)) {
                // moving left OR right one space, and up/down one space
                if (abs(x1 - x2) == 1 && abs(y1 - y2) == 1) {
                    return true;
                // moving up or down two spaces
                } else if (abs(y2 - y1) == 2) {
                    // moving right, checks if piece being captured is owned by other user
                    if ((x2 - x1) == 2 && checkerboard[y1 + yDirMoving][x1 + 1]->getOwner() == otherUser) {
                        setJumping(RIGHT);
                        return true;
                        // moving left, checks if piece being captured is owned by other user
                    } else if (((x2 - x1) == -2) && checkerboard[y1 + yDirMoving][x1 - 1]->getOwner() == otherUser) {
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
    void movePiece(short y1, short x1, short y2, short x2, short yDirMoving) {
        // clear owner of old space
        checkerboard[y1][x1]->setOwner(NULL);
        // set new space to player's
        checkerboard[y2][x2]->setOwner(currUser);
        // if moving piece is a king, switch space of king
        if (checkerboard[y1][x1]->getKing()) {
            checkerboard[y1][x1]->setKing(false);
            checkerboard[y2][x2]->setKing(true);
            // if moving piece wasn't a king, king the pice
        } else if (y2 == 0 || y2 == this->BOARD_SIZE - 1) {
            checkerboard[y2][x2]->setKing(true);
            std::cout << "You got kinged!\n";
        }
        // captured piece.
        if (getJumping() == RIGHT) {
            // set captured space to no owner.
            checkerboard[y1 + yDirMoving][x1 + 1]->setOwner(NULL);
            // clear captured space of king.
            checkerboard[y1 + yDirMoving][x1 + 1]->setKing(false);
            // increment pieces captured by player.
            currUser->capturedPiece();
            // if jumping left, clear captured space of owner and king
        } else if (getJumping() == LEFT) {
            checkerboard[y1 + yDirMoving][x1 - 1]->setOwner(NULL);
            checkerboard[y1 + yDirMoving][x1 - 1]->setKing(false);
            //increment pieces captured by player
            currUser->capturedPiece();
        }
        print();
    }
    public:
    void start() {
        short y1, x1, y2, x2, y3, x3;
        char a;
        char isAnotherMove = 'N';
        bool isMoveValid = true, jumpedAPiece = false;
        short yDirMoving;
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
                std::cout << "choose a row (1 - 8): ";
                std::cin >> y1;
                std::cout << "choose a column(1 - 8): ";
                std::cin >> x1;
                std::cout << "choose the row you want the piece to move (1 - 8): ";
                std::cin >> y2;
                std::cout << "choose the column(1 - 8): ";
                std::cin >> x2;
                // decrement selecions by 1 to colvert from 1-8, to 0-7 for array.
                y1--;
                x1--;
                y2--;
                x2--;
                do {
                    // if first move was a capture, must capture again.
                    if (jumpedAPiece) {
                        // set old dest to current location
                        y1 = y2;
                        x1 = x2;
                        std::cout << "choose the row you want the piece to move(1 - 8): ";
                        std::cin >> y2;
                        std::cout << "choose the column(1 - 8): ";
                        std::cin >> x2;
                        // convert from 1 - 8 to 0 - 7.
                        y2--;
                        x2--;

                        // if next position is a valid move, and is jumping a piece
                        if (this->isNextPosValid(y1, x1, y2, x2, k) && getJumping() != NONE) {
                            isMoveValid = true;
                            yDirMoving = y2 - y1 > 0 ? 1 : -1;
                            movePiece(y1, x1, y2, x2, yDirMoving);
                            // if player has captured all opponents pieces, end game.
                            if (currUser->getNumCaptured() == BOARD_SIZE/2 * (BOARD_SIZE/2 - 1)) {
                                std::cout << currUser->getOwner() << " won! yeeeet";
                                return;
                            }
                            std::cout << "you captured a piece! jump again? Y/N: ";
                            std::cin >> isAnotherMove;
                            turnCount++;
                        } else {
                            std::cout << "invalid try again? Y/N: ";
                            std::cin >> isAnotherMove;
                        }
                    }
                    // if first move, check if move is valid
                    else if (this->isMoveValid(y1, x1, y2, x2, k)) {
                        isMoveValid = true;
                        turnCount++;
                        yDirMoving = y2 - y1 > 0 ? 1 : -1;
                        movePiece(y1, x1, y2, x2, yDirMoving);
                        // if player captured all opponents pieces, finish game.
                        if (currUser->getNumCaptured() == BOARD_SIZE/2 * (BOARD_SIZE/2 - 1)) {
                            std::cout << currUser->getOwner() << " won! yeeeet";
                            return;
                        }
                        // if jumped, can jump again.
                        if (getJumping() != NONE) {
                            std::cout << "you captured a piece! jump again? Y/N: ";
                            std::cin >> isAnotherMove;
                            // set first move to a jump.
                            jumpedAPiece = true;
                        }
                        // first move invalid, has to move again.
                    } else {
                            isMoveValid = false;
                            std::cout<<"move is invalid. try again.\n";
                    }
                } while (isAnotherMove == 'Y' || isAnotherMove == 'y');
            } while(!isMoveValid);
        }
    }
};