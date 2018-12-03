class user {
    const char piece, king;
    std::string name;
    // tracks pieces captured by each user
    short numCaptured;
    public:
    // initializes user's name and piece identifiers
    user(char piece, char king, std::string name) : piece(piece), king(king) {
        this->numCaptured = 0;
        this->name = name;
    }
    char getPiece() {
        return piece;
    }
    char getKing() {
        return king;
    }
    std::string getOwner() {
        return name;
    }
    void capturedPiece() {
        this->numCaptured++;
    }
    short getNumCaptured() {
        return this->numCaptured;
    }
};