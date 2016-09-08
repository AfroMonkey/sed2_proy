#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <ctime>
#include <cstring>
#include <fstream>

struct EmailData
{
    int id;
    time_t time;
    char from[256];
    char to[256];
    char cc[256];
    char bcc[256];
    char subject[78];
    char content[512];
};

class Email
{
private:
    EmailData data;
    char time_c[100];
    char date_c[100];
    void update_time();
public:
    void set_id(const int id);
    int get_id();
    void set_time(const time_t time);
    char* get_time();
    char* get_date();
    void set_from(const char *from);
    char* get_from();
    void set_to(const char *to);
    char* get_to();
    void set_cc(const char *cc);
    char* get_cc();
    void set_bcc(const char *bcc);
    char* get_bcc();
    void set_subject(const char *subject);
    char* get_subject();
    void set_content(const char *content);
    char* get_content();
    void write(std::ofstream &file);
    void read(std::ifstream &file);
    void write(std::fstream &file);
    void read(std::fstream &file);
};

void Email::update_time()
{
    if (!strftime(time_c, sizeof(time_c), "%H:%M", localtime(&data.time)))
    {
        strcpy(time_c, "00:00");
    }

    if (!strftime(date_c, sizeof(date_c), "%Y-%m-%d", localtime(&data.time)))
    {
        strcpy(date_c, "1970-01-01");
    }
}

void Email::set_id(const int id)
{
    this->data.id = id;
}

int Email::get_id()
{
    return data.id;
}

void Email::set_time(const time_t time)
{
    this->data.time = time;
    update_time();
}

char* Email::get_time()
{
    return time_c;
}

char* Email::get_date()
{
    return date_c;
}

void Email::set_from(const char* from)
{
    strcpy(this->data.from, from);
}

char* Email::get_from()
{
    return data.from;
}

void Email::set_to(const char* to)
{
    strcpy(this->data.to, to);
}

char* Email::get_to()
{
    return data.to;
}

void Email::set_cc(const char* cc)
{
    strcpy(this->data.cc, cc);
}

char* Email::get_cc()
{
    return data.cc;
}

void Email::set_bcc(const char* bcc)
{
    strcpy(this->data.bcc, bcc);
}

char* Email::get_bcc()
{
    return data.bcc;
}

void Email::set_subject(const char* subject)
{
    strcpy(this->data.subject, subject);
}

char* Email::get_subject()
{
    return data.subject;
}

void Email::set_content(const char* content)
{
    strcpy(this->data.content, content);
}

char* Email::get_content()
{
    return data.content;
}

void Email::write(std::ofstream &file)
{
    file.write((char*)&data, sizeof(EmailData));
}

void Email::read(std::ifstream &file)
{
    file.read((char*)&data, sizeof(EmailData));
    update_time();
}

void Email::write(std::fstream &file)
{
    file.write((char*)&data, sizeof(EmailData));
}

void Email::read(std::fstream &file)
{
    file.read((char*)&data, sizeof(EmailData));
    update_time();
}

#endif
