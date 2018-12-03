class space {
    bool isAKing, isValid;
    user *owner;
    std::string name;
    public:
    // set owner of space
    space(user *owner) {
        this->owner = owner;
    }
    // get piece in this space
    char getPiece() {
        if (!owner)
        return ' ';
        else if (isAKing)
        return owner->getKing();
        else
        return owner->getPiece();
    }
    user* getOwner() {
        return owner;
    }
    void setOwner(user *owner) {
        this->owner = owner;
    }
    void setKing(bool isAKing) {
        this->isAKing = isAKing;
    }
    bool getKing() {
        return this->isAKing;
    }
};