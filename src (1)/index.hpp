#ifndef INDEX_HPP
#define INDEX_HPP

template <typename T, typename A>
class Index
{
public:
    Index() {}
    Index(T _key, A _address) : key(_key), address(_address) {}
    Index(T _key) : key(_key) {}


    T key;
    A address;
    bool empty() {return false;}
};

#endif
