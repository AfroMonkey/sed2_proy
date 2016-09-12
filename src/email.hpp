#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <ctime>
#include <cstring>

struct EmailData
{
};

class Email
{
private:
    int id;
    time_t time;
    char from[256];
    char to[256];
    char cc[256];
    char bcc[256];
    char subject[78];
    char content[512];
public:
    void set_id(const int id);
    int get_id();
    void set_time(const time_t time);
    time_t get_time();
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
    static int cmp_from(char *from, Email* a);
};

void Email::set_id(const int id)
{
    this->id = id;
}

int Email::get_id()
{
    return id;
}

void Email::set_time(const time_t time)
{
    this->time = time;
}

time_t Email::get_time()
{
    return time;
}

void Email::set_from(const char* from)
{
    strcpy(this->from, from);
}

char* Email::get_from()
{
    return from;
}

void Email::set_to(const char* to)
{
    strcpy(this->to, to);
}

char* Email::get_to()
{
    return to;
}

void Email::set_cc(const char* cc)
{
    strcpy(this->cc, cc);
}

char* Email::get_cc()
{
    return cc;
}

void Email::set_bcc(const char* bcc)
{
    strcpy(this->bcc, bcc);
}

char* Email::get_bcc()
{
    return bcc;
}

void Email::set_subject(const char* subject)
{
    strcpy(this->subject, subject);
}

char* Email::get_subject()
{
    return subject;
}

void Email::set_content(const char* content)
{
    strcpy(this->content, content);
}

char* Email::get_content()
{
    return content;
}

int Email::cmp_from(char* from, Email* b)
{
    return strcmp(from, b->get_from());
}

#endif
