#ifndef FIXED_FILE_MANAGER_HPP
#define FIXED_FILE_MANAGER_HPP

#include <fstream>

template <typename T>
class FixedFileManager
{
private:
    std::fstream file;
public:
    FixedFileManager(const char* path);
    ~FixedFileManager();
    bool is_open();
    bool write(const T* data, const size_t pos);
    bool append(const T* data);
    T* read(const size_t pos);
    template <typename F>
    size_t find(T data, F cmp);
};

template <typename T>
FixedFileManager<T>::FixedFileManager(const char* path)
{
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::ofstream aux(path, std::ios::binary);
        aux.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    }

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
    if (file.is_open())
    {
        file.seekp(pos * sizeof(T));
        file.write((char*)data, sizeof(T));
        return true;
    }
        return false;
}

template <typename T>
bool FixedFileManager<T>::append(const T* data)
{
    if (file.is_open())
    {
        file.seekp(0, std::ios::end);
        file.write((char*)data, sizeof(T));
        return true;
    }
    return false;
}

template <typename T>
T* FixedFileManager<T>::read(const size_t pos)
{
    if (file.is_open())
    {
        T* data = new T;
        file.seekg(pos * sizeof(T), std::ios::beg);
        file.read((char*)data, sizeof(T));
        return data;
    }
    return nullptr;
}

template <typename T>
template <typename F>
size_t FixedFileManager<T>::find(T data, F cmp)
{
    if (file.is_open())
    {
        T aux = new T;
        file.seekg(0, std::ios::beg);
        while (true)
        {
            file.read((char*)&aux, sizeof(T));
            if (file.eof()) break;
            if (cmp(data, aux))
            {
                return file.tellg() - sizeof(T);
            }
        }
    }
    return nullptr;
}


#endif
