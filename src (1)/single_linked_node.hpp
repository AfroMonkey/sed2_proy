#ifndef SINGLE_LINKED_NODE_HPP
#define SINGLE_LINKED_NODE_HPP

template <typename T>
class SLN
{
public:
    T data;
    SLN* next;

    SLN();
    SLN(T data);
    SLN(T data, SLN* next);
};

template <typename T>
SLN<T>::SLN()
{
    this->next = nullptr;
}

template <typename T>
SLN<T>::SLN(T data)
{
    this->data = data;
    this->next = nullptr;
}

template <typename T>
SLN<T>::SLN(T data, SLN* next)
{
    this->data = data;
    this->next = next;
}


#endif
