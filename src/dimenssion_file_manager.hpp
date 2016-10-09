#ifndef DIMENSSION_FILE_MANAGER_HPP
#define DIMENSSION_FILE_MANAGER_HPP

#include <fstream>
#include <vector>
#include <cstdio>

#include <iostream>

template <typename T>
class DimenssionFileManager
{
private:
    std::fstream file;
    size_t n_fields;
    std::string path;
    bool open();
    void close();
    T* next_record();
    void write(T& data);
    void write_aux(std::ofstream& file_aux, T* data);
public:
    DimenssionFileManager(const char* path, const size_t n_fields);
    ~DimenssionFileManager();
    bool is_open();
    void append(T& data);
    template <typename F>
    void for_each(F function);
    template <typename V, typename F>
    T* find (V value, F function, size_t* num_row = nullptr);
    void write_in(T* data, const size_t num_row);
};

template <typename T>
DimenssionFileManager<T>::DimenssionFileManager(const char* path, const size_t n_fields)
{
    this->path = path;
    this->n_fields = n_fields;
}

template <typename T>
bool DimenssionFileManager<T>::open()
{
    file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::ofstream aux(path);
        aux.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
        file.seekg(0, std::ios::beg);
    }
    return file.is_open();
}

template <typename T>
DimenssionFileManager<T>::~DimenssionFileManager()
{
    if (file.is_open())
    {
        file.close();
    }
}

template <typename T>
bool DimenssionFileManager<T>::is_open()
{
    return file.is_open();
}

template <typename T>
void DimenssionFileManager<T>::write(T& data)
{
    auto fields = data.get_fields();
    size_t chars;
    for (auto field : fields)
    {
        chars = field.length();
        file.write((char*)&chars, sizeof(size_t));
        file.write((char*)field.c_str(), chars);
    }
}

template <typename T>
void DimenssionFileManager<T>::append(T& data)
{
    open();
    if (!file.is_open()) return;
    file.seekp(0, std::ios::end);
    write(data);
    file.close();
}

template <typename T>
T* DimenssionFileManager<T>::next_record()
{
    if (!file.is_open()) return nullptr;
    T* record = new T;
    std::vector<std::string> fields;
    size_t chars;
    char* field;
    for (size_t i = 0; i < n_fields; ++i)
    {
        file.read((char*)&chars, sizeof(size_t));
        if (file.eof()) return nullptr;
        field = new char[chars+1];
        field[chars] = '\x0';
        file.read((char*)field, chars);
        fields.push_back(field);
        delete[] field;
    }
    record->set_fields(fields);
    return record;
}

template <typename T>
template <typename F>
void DimenssionFileManager<T>::for_each(F function)
{
    open();
    T* aux;
    while (true)
    {
        aux = next_record();
        if (!aux) break;
        function(aux);
        delete aux;
    }
    file.close();
}

template <typename T>
template <typename V, typename F>
T* DimenssionFileManager<T>::find(V value, F compare, size_t* num_row)
{
    open();
    T* aux;
    while (true)
    {
        aux = next_record();
        if (!aux) break;
        if(compare(*aux, value) == 0)
        {
            file.close();
            return aux;
        }
        delete aux;
        if (num_row != nullptr)
        {
            ++*num_row;
        }
    }
    file.close();
    return nullptr;
}

template <typename T>
void DimenssionFileManager<T>::write_aux(std::ofstream& file_aux, T* data)
{
    auto fields = data->get_fields();
    size_t chars;
    for (auto field : fields)
    {
        chars = field.length();
        file_aux.write((char*)&chars, sizeof(size_t));
        file_aux << field;
    }
}

template <typename T>
void DimenssionFileManager<T>::write_in(T *data, const size_t num_row)
{
    open();
    if (!file.is_open()) return;
    std::ofstream aux_file("~" + path);
    if (!aux_file.is_open()) return;
    size_t i = 0;
    T* record;
    while (true)
    {
        record = next_record();
        if (file.eof()) break;
        if (i == num_row)
        {
            if (data != nullptr)
            {
                write_aux(aux_file, data);
            }
        }
        else
        {
            write_aux(aux_file, record);
        }
        ++i;
        delete record;
    }
    aux_file.close();
    file.close();
    remove(path.c_str());
    rename(("~" + path).c_str(), path.c_str());
}

#endif
