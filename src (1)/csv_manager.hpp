#ifndef CSV_MANAGER_HPP
#define CSV_MANAGER_HPP

#include <fstream>
#include <vector>
#include <cstdio>

template <typename T>
class CSV_Manager
{
private:
    std::fstream file;
    std::string to_standard(std::string field);
    std::vector<std::string> from_standard(std::string row);
    std::string path;
    bool open();
    void close();
    std::string next_row();
    T* next_record(std::string row = "x");
    std::string to_row(T& data);
public:
    CSV_Manager(const char* path);
    ~CSV_Manager();
    bool is_open();
    void append(T& data);
    template <typename F>
    void for_each(F function);
    template <typename V, typename F>
    T* find (V value, F function, size_t* num_row = nullptr);
    void write_in(T* data, const size_t num_row);
};

template <typename T>
CSV_Manager<T>::CSV_Manager(const char* path)
{
    this->path = path;
}

template <typename T>
bool CSV_Manager<T>::open()
{
    file.open(path, std::ios::in | std::ios::out);
    if (!file.is_open())
    {
        std::ofstream aux(path);
        aux.close();
        file.open(path, std::ios::in | std::ios::out);
        file.seekg(0, std::ios::beg);
    }
    return file.is_open();
}

template <typename T>
CSV_Manager<T>::~CSV_Manager()
{
    if (file.is_open())
    {
        file.close();
    }
}

template <typename T>
bool CSV_Manager<T>::is_open()
{
    return file.is_open();
}

template <typename T>
std::string CSV_Manager<T>::to_row(T& data)
{
    std::string record;
    auto fields = data.get_fields();
    size_t num_fields = fields.size();
    for (auto field : fields)
    {
        record += to_standard(field);
        if (--num_fields)
        {
            record += ",";
        }
    }
    return record + '\n';
}

template <typename T>
void CSV_Manager<T>::append(T& data)
{
    open();
    if (!file.is_open()) return;
    file.seekp(0, std::ios::end);
    file << to_row(data);
    file.close();
}

template <typename T>
std::string CSV_Manager<T>::to_standard(std::string field)
{
    bool quotes = false;
    std::string standard;
    for (auto c : field)
    {
        if (c == '\"' || c == ',' || c == '\n')
        {
            quotes = true;
            if (c == '\"')
            {
                standard += "\"";
            }
        }
        standard += c;
    }
    if (quotes) return "\"" + standard + "\"";
    return field;
}

template <typename T>
std::vector<std::string> CSV_Manager<T>::from_standard(std::string row)
{
    std::vector<std::string> fields;
    bool quotes = false;
    bool last_is_quote = false;
    std::string aux;
    for (auto c : row)
    {
        if ((c == ',' || c == '\n' || c == EOF) && !quotes)
        {
            fields.push_back(aux);
            aux = "";
        }
        else
        {
            if (c == '"')
            {
                if(last_is_quote && !quotes)
                {
                    aux += c;
                }
                last_is_quote = true;
                quotes = !quotes;
            }
            else
            {
                last_is_quote = false;
                aux += c;
            }
        }
    }
    return fields;
}

template <typename T>
std::string CSV_Manager<T>::next_row()
{
    if (!file.is_open()) return "";
    std::string row;
    bool quotes = false;
    char c;
    file.clear();
    while(true)
    {
        file.read(&c, 1);
        if (file.eof()) return "";
        if (c == EOF || (!quotes && c == '\n')) break;
        if (c == '"')
        {
            quotes = !quotes;
        }
        row += c;
    }
    return row + '\n';
}

template <typename T>
T* CSV_Manager<T>::next_record(std::string row)
{
    T* record;
    if (row == "x")
        row = next_row();
    if (file.eof()) return nullptr;
    record = new T;
    record->set_fields(from_standard(row));
    return record;
}

template <typename T>
template <typename F>
void CSV_Manager<T>::for_each(F function)
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
T* CSV_Manager<T>::find(V value, F compare, size_t* num_row)
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
void CSV_Manager<T>::write_in(T *data, const size_t num_row)
{
    open();
    if (!file.is_open()) return;
    std::ofstream aux_file("~" + path);
    if (!aux_file.is_open()) return;
    size_t i = 0;
    std::string row;
    while (true)
    {
        row = next_row();
        if (file.eof()) break;
        if (i == num_row)
        {
            if (data != nullptr)
            {
                aux_file << to_row(*data);
            }
        }
        else
        {
            aux_file << row;
        }
        ++i;
    }
    aux_file.close();
    file.close();
    remove(path.c_str());
    rename(("~" + path).c_str(), path.c_str());
}

#endif
