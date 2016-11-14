#ifndef HASH_HPP
#define HASH_HPP

#include <list>

template <typename K, typename V>
class Hash
{
private:
    std::list<V>* _table;
    bool* _established;
    unsigned int _max;
    unsigned long (*_hf)(const K);
public:
    Hash(unsigned long(*hf)(const K), unsigned int max);
    ~Hash();

    void clear();
    std::list<V> get(const K& key);
    void set(const K& key, V value);
    bool has(const K& key);
    bool del(const K& key);
};

template <typename K, typename V>
Hash<K, V>::Hash(unsigned long(*hf)(const K), unsigned int max)
{
    _max = max;
    _table = new std::list<V>[_max];
    _established = new bool[_max];
    _hf = hf;
    for (unsigned i = 0; i < _max; ++i)
    {
        _established[i] = false;
    }
}

template <typename K, typename V>
Hash<K, V>::~Hash()
{
    delete []_table;
    delete []_established;
}

template <typename K, typename V>
void Hash<K, V>::clear()
{
    delete []_table;
    delete []_established;

    _table = new std::list<V>[_max];
    _established = new bool[_max];
    for (unsigned i = 0; i < _max; ++i)
    {
        _established[i] = false;
    }
}

template <typename K, typename V>
std::list<V> Hash<K, V>::get(const K& key)
{
    unsigned long pos = _hf(key);
    if (pos <= _max && _established[pos])
    {
        return _table[pos];
    }
    throw 1;
}

template <typename K, typename V>
void Hash<K, V>::set(const K& key, V value)
{
    unsigned long pos = _hf(key);
    _table[pos].push_back(value);
    _established[pos] = true;
}

template <typename K, typename V>
bool Hash<K, V>::has(const K& key)
{
    return _established[_hf(key)];
}

template <typename K, typename V>
bool Hash<K, V>::del(const K& key)
{
    unsigned long pos = _hf(key);
    if (_established[pos])
    {
        _established[pos] = false;
        return true;
    }
    return false;
}

#endif
