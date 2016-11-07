#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <ctime>
#include <cstring>
#include <vector>
#include <string>

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
    bool empty();
    std::vector<std::string> get_fields();
    void set_fields(std::vector<std::string> fields);
    bool operator==(const Email& other) const;
    bool operator!=(const Email& other) const;
    static bool equal(const Email& a, const Email& b);
    static bool diff(const Email& a, const Email& b);
    static bool compare_id(Email& a, const int id);
    static int cmp_from(Email& a, Email& b);

    Email();
};

Email::Email()
{
    id = 0;
    from[0] = '\x0';
    to[0] = '\x0';
    cc[0] = '\x0';
    bcc[0] = '\x0';
    subject[0] = '\x0';
    content[0] = '\x0';
}

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

std::vector<std::string> Email::get_fields()
{
    std::vector<std::string> fields;
    std::string aux;
    char date_c[11];
    char time_c[6];
    fields.push_back(std::to_string(id));
    strftime(date_c, sizeof(date_c), "%Y-%m-%d", localtime(&time));
    aux = date_c;
    fields.push_back(aux);
    strftime(time_c, sizeof(time_c), "%H:%M", localtime(&time));
    aux = time_c;
    fields.push_back(aux);
    aux = from;
    fields.push_back(aux);
    aux = to;
    fields.push_back(aux);
    aux = cc;
    fields.push_back(aux);
    aux = bcc;
    fields.push_back(aux);
    aux = subject;
    fields.push_back(aux);
    aux = content;
    fields.push_back(aux);
    return fields;
}

void Email::set_fields(std::vector<std::string> fields)
{
    std::string date;
    std::string time_c;
    id = atoi(fields[0].c_str());

    date = fields[1];
    date += ' ' + fields[2];
    struct tm tm;
    strptime(date.c_str(), "%Y-%m-%d %H:%M", &tm);
    time = mktime(&tm);
    strcpy(from,fields[3].c_str());
    strcpy(to,fields[4].c_str());
    strcpy(cc,fields[5].c_str());
    strcpy(bcc,fields[6].c_str());
    strcpy(subject,fields[7].c_str());
    strcpy(content,fields[8].c_str());
}

bool Email::operator==(const Email& other) const
{
    return id == other.id && time == other.time && strcmp(from, other.from) == 0 &&
    strcmp(to, other.to) == 0 && strcmp(cc, other.cc) == 0 &&
    strcmp(bcc, other.bcc) == 0 && strcmp(subject, other.subject) == 0 &&
    strcmp(content, other.content) == 0;
}

bool Email::operator!=(const Email& other) const
{
    return !operator==(other);
}

bool Email::equal(const Email& a, const Email& b)
{
    return a == b;
}

bool Email::diff(const Email& a, const Email& b)
{
    return a != b;
}

bool Email::empty()
{
    return id == 0;
}

bool Email::compare_id(Email& a, const int id)
{
    return id - a.get_id();
}

int Email::cmp_from(Email& a, Email& b)
{
    return strcmp(a.get_from(), b.get_from());
}


#endif
