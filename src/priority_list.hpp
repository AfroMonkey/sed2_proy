#ifndef PRIORITY_LIST
#define PRIORITY_LIST

#include "single_linked_node.hpp"

template <typename T, int(*C)(T&, T&)>
class PriorityList
{
private:
    SLN<T>* _begin;
public:
    PriorityList();
    ~PriorityList();

    bool empty();
    void push(const T data);
    T pop();
    T begin() const;
    bool erase(const T& data, unsigned times = (unsigned)-1);
    SLN<T>* find(T& data) const;
    template <typename F>
    void for_each(F function) const;
    template <typename F>
    void for_each_equal(const T& data, F function) const;
    void clear();
};

template <typename T, int(*C)(T&, T&)>
PriorityList<T, C>::PriorityList()
{
    _begin = nullptr;
}

template <typename T, int(*C)(T&, T&)>
PriorityList<T, C>::~PriorityList()
{
    clear();
}

template <typename T, int(*C)(T&, T&)>
bool PriorityList<T, C>::empty()
{
    return _begin == nullptr;
}

template <typename T, int(*C)(T&, T&)>
void PriorityList<T, C>::push(const T data)
{
    SLN<T>* nn = new SLN<T>(data);
    if (!_begin)
    {
        _begin = nn;
    }
    else
    {
        if (C(nn->data, _begin->data) <= 0)
        {
            nn->next = _begin;
            _begin = nn;
        }
        else
        {
            SLN<T>* aux = _begin;
            while (aux->next && C(aux->next->data, nn->data) < 0)
            {
                aux = aux->next;
            }
            nn->next = aux->next;
            aux->next = nn;
        }
    }
}

template <typename T, int(*C)(T&, T&)>
T PriorityList<T, C>::pop()
{
    T t_begin;
    if (_begin)
    {
        t_begin = _begin->data;
        SLN<T>* aux = _begin->next;
        delete (_begin);
        _begin = aux;
    }
    return t_begin;
}

template <typename T, int(*C)(T&, T&)>
T PriorityList<T, C>::begin() const
{
    if (_begin)
    {
        return _begin->data;
    }
    else
    {
        T empty;
        return empty;
    }
}

template <typename T, int(*C)(T&, T&)>
bool PriorityList<T, C>::erase(const T& data, unsigned times)
{
    if (!_begin) return false;
    if (_begin->data == data)
    {
        --times;
    }
    if (_begin->next)
    {
        SLN<T>* aux = _begin;
        SLN<T>* aux2;
        while (aux->next && C(aux->next->data, data) <= 0 && times)
        {
            if (C(aux->next->data, data) == 0)
            {
                aux2 = aux->next;
                aux->next = aux2->next;
                delete aux2;
                --times;
                continue;
            }
            aux = aux->next;
        }
    }
    if (C(_begin->data, data) == 0)
    {
        pop();
    }
    return times == 0;
}

template <typename T, int(*C)(T&, T&)>
SLN<T>* PriorityList<T, C>::find(T& data) const
{
    SLN<T>* aux = _begin;
    while (aux && C(aux->data, data) < 0)
    {
        aux = aux->next;
    }
    if (aux && C(aux->data, data) == 0)
    {
        return aux;
    }
    return nullptr;
}

template <typename T, int(*C)(T&, T&)>
template <typename F>
void PriorityList<T, C>::for_each(F function) const
{
    SLN<T>* aux = _begin;
    while (aux)
    {
        function(aux);
        aux = aux->next;
    }
}

template <typename T, int(*C)(T&, T&)>
template <typename F>
void PriorityList<T, C>::for_each_equal(const T& data, F function) const
{
    SLN<T>* aux = _begin;
    while (aux && C(aux->data, data) <= 0)
    {
        if (C(aux->data, data) == 0)
        {
            function(aux);
        }
        aux = aux->next;
    }
}

template <typename T, int(*C)(T&, T&)>
void PriorityList<T, C>::clear()
{
    SLN<T>* aux;
    while (_begin)
    {
        aux = _begin;
        _begin = _begin->next;
        delete aux;
    }
}

#endif
