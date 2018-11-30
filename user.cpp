class user {
    const char piece, king;
    std::string name;
    public:
    user(char piece, char king, std::string name) : piece(piece), king(king) {
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
};