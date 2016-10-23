#ifndef INDEX_HPP
#define INDEX_HPP

class Index
{
public:
    Index(int _key = 0, int _address = 0) : key(_key), address(_address) {}

    int key;
    int address;
    bool empty() {return false;}

    static int compare(Index& a, Index& b) {return a.key - b.key;}
};

#endif
