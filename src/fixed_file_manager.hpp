#ifndef FIXED_FILE_MANAGER_HPP
#define FIXED_FILE_MANAGER_HPP

#include <fstream>

template <typename T>
class FixedFileManager
{
private:
    std::fstream file;
    std::string path;
    bool open();
public:
    FixedFileManager(const char* path);
    ~FixedFileManager();
    bool is_open();
    bool write(const T* data, const size_t pos);
    bool append(T* data);
    T* read(const size_t pos);
    template <typename F>
    size_t find(T data, F cmp);
    template <typename F>
    void for_each(F function);
    template <typename F, typename C, typename O>
    void for_each(F function, C compare, O object);
    void clean();
};

template <typename T>
FixedFileManager<T>::FixedFileManager(const char* path)
{
    this->path = path;
}

template <typename T>
bool FixedFileManager<T>::open()
{
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::ofstream aux(path, std::ios::binary);
        aux.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    }
    return file.is_open();
}

template <typename T>
FixedFileManager<T>::~FixedFileManager()
{
    if (file.is_open())
    {
        file.close();
    }
}

template <typename T>
bool FixedFileManager<T>::is_open()
{
    return file.is_open();
}

template <typename T>
bool FixedFileManager<T>::write(const T* data, const size_t pos)
{
    open();
    if (file.is_open())
    {
        file.seekp(pos * sizeof(T));
        file.write((char*)data, sizeof(T));
        file.close();
        return true;
    }
    file.close();
    return false;
}

template <typename T>
bool FixedFileManager<T>::append(T* data)
{
    open();
    if (file.is_open())
    {
        size_t pos = data->get_id();
        file.seekp(pos * sizeof(T));
        file.write((char*)data, sizeof(T));
        file.close();
        return true;
    }
    file.close();
    return false;
}

template <typename T>
T* FixedFileManager<T>::read(const size_t pos)
{
    open();
    if (file.is_open())
    {
        T* data = new T;
        file.seekg(pos * sizeof(T), std::ios::beg);
        file.read((char*)data, sizeof(T));
        file.close();
        return data;
    }
    file.close();
    return nullptr;
}

template <typename T>
template <typename F>
size_t FixedFileManager<T>::find(T data, F cmp)
{
    open();
    if (file.is_open())
    {
        T* aux = new T;
        file.seekg(0, std::ios::beg);
        while (true)
        {
            file.read((char*)aux, sizeof(T));
            if (file.eof()) break;
            if (cmp(data, *aux) == 0)
            {
                file.close();
                return ((long)file.tellg() - sizeof(T)) / sizeof(T);
            }
        }
        file.clear();
    }
    file.close();
    return (size_t)-1;
}

template <typename T>
template <typename F>
void FixedFileManager<T>::for_each(F function)
{
    open();
    if (!file.is_open()) return;
    T* aux = new T;
    file.seekg(0, std::ios::beg);
    while (true)
    {
        file.read((char*)aux, sizeof(T));
        if (!aux->empty())
        {
            if (file.eof()) break;
            function(*(aux));
        }
    }
    file.clear();
    file.close();
}

template <typename T>
template <typename F, typename C, typename O>
void FixedFileManager<T>::for_each(F function, C compare, O object)
{
    open();
    if (!file.is_open()) return;
    T* aux = new T;
    file.seekg(0, std::ios::beg);
    while (true)
    {
        file.read((char*)aux, sizeof(T));
        if(compare(*aux, object) == 0)
        {
            if (file.eof()) break;
            function(aux);
        }
    }
    file.clear();
    file.close();
}

template <typename T>
void FixedFileManager<T>::clean()
{
    if(open())
    {
        file.close();
        remove(path.c_str());
    }
}

#endif
