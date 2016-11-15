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
    bool set(const K& key, V value);
    bool set_p(const unsigned long& pos, std::list<V> l);
    bool set_h(const K& key, std::list<V> l);
    bool has(const K& key);
    bool del(const K& key);
    std::list<std::list<V>> get_lists();
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
bool Hash<K, V>::set(const K& key, V value)
{
    unsigned long pos = _hf(key);
    if (pos > _max) return false;
    _table[pos].push_back(value);
    _established[pos] = true;
    return true;
}

template <typename K, typename V>
bool Hash<K, V>::set_p(const unsigned long& pos, std::list<V> l)
{
    if (pos > _max) return false;
    _table[pos] = l;
    _established[pos] = true;
}

template <typename K, typename V>
bool Hash<K, V>::set_h(const K& key, std::list<V> l)
{
    unsigned long pos = _hf(key);
    if (pos > _max) return false;
    _table[pos] = l;
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

template <typename K, typename V>
std::list<std::list<V>> Hash<K, V>::get_lists()
{
    std::list<std::list<V>> l;
    for (unsigned int i = 0; i < _max; ++i)
    {
        if (_established[i])
        {
            l.push_back(_table[i]);
        }
    }
    return l;
}

#endif
