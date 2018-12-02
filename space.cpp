class space {
    // checker of type piece. for each square.
    bool isAKing, isValid;
    user *owner;
    std::string name;
    public:
    space(user *owner) {
        this->owner = owner;
        // every other row from 0
    }
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
    void setIsValid(bool isValid) { 
        this->isValid = isValid;
    }
    bool getIsValid() {
        return isValid;
    }
    void setKing(bool isAKing) {
        this->isAKing = isAKing;
    }
    bool getKing() {
        return this->isAKing;
    }
};