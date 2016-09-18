#ifndef CSV_MANAGER_HPP
#define CSV_MANAGER_HPP

#include <fstream>

template <typename T>
class CSV_Manager
{
private:
    std::fstream file;
    std::string to_standard(std::string field);
public:
    CSV_Manager(const char* path);
    ~CSV_Manager();
    bool is_open();
    // bool write(const T* data, const size_t pos);
    void append(T* data);
    // T* read(const size_t pos);
};

template <typename T>
CSV_Manager<T>::CSV_Manager(const char* path)
{
    file.open(path, std::ios::in | std::ios::out);
    if (!file.is_open())
    {
        std::ofstream aux(path);
        aux.close();
        file.open(path, std::ios::in | std::ios::out);
    }

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
void CSV_Manager<T>::append(T* data)
{
    if (!file.is_open()) return;
    file.seekp(0, std::ios::end);
    auto fields = data->get_fields();
    size_t num_fields = fields.size();
    for (auto field : fields)
    {
        file << to_standard(field);
        if (--num_fields)
        {
            file << ",";
        }
    }
    file << "\n";
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

#endif
