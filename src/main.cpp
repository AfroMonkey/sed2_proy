#include <functional>

#include "cli.hpp"
#include "email.hpp"
#include "fixed_file_manager.hpp"
#include "csv_manager.hpp"

using namespace std;

#define EMAIL_FILE_PATH "email.bd"
#define FILE_CSV "backup.csv"

FixedFileManager<Email> email_file(EMAIL_FILE_PATH);
CSV_Manager<Email> csv_file(FILE_CSV);


auto append_to_csv = std::bind(&CSV_Manager<Email>::append, &csv_file, std::placeholders::_1);

size_t search_by()
{
    msg("1) ID\n");
    msg("2) Remitente\n");
    switch (get_int())
    {
        case 1:
        {
            return get_int("ID>");
        }
        case 2:
        {
            char from[256];
            size_t pos;
            strcpy(from, get_string("Remitente>").c_str());
            pos = email_file.find(from, Email::cmp_from);
            return pos;
        }

    }
    return -1;
}

int main()
{
    short opt;
    do
    {
        if (email_file.is_open())
        {
            clear_screen();
            display_menu();
            opt = get_int();
        }
        else
        {
            opt = OPT_ERROR_FILE;
        }
        switch (opt)
        {
            case OPT_EXIT:
            {
                break;
            }
            case OPT_WRITE:
            {
                Email *email = new Email;
                fill(email);
                email->set_time(time(nullptr));
                email_file.write(email, email->get_id());
                msg(MSG_DONE);
                break;
            }
            case OPT_READ:
            {
                Email* email;
                size_t pos = search_by();
                if (pos != (size_t)-1 && (email = email_file.read(pos)) && email != nullptr)
                {
                    display(email);
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPT_MODIFY:
            {
                Email* email;
                size_t pos = search_by();
                if (pos != (size_t)-1 && (email = email_file.read(pos)) && email != nullptr)
                {
                    display(email);
                    msg("1) Fecha y Hora\n");
                    msg("2) Remitente\n");
                    msg("3) Destinatario\n");
                    msg("4) CC\n");
                    msg("5) BCC\n");
                    msg("6) Asunto\n");
                    msg("7) Contenido\n");
                    switch (get_int())
                    {
                        case 1:
                        {
                            struct tm tm;
                            strptime(get_string("Y-M-D H:M>").c_str(), "%Y-%m-%d %H:%M", &tm);
                            time_t time = mktime(&tm);
                            email->set_time(time);
                            break;
                        }
                        case 2:
                        {
                            email->set_from(get_string("Remitente").c_str());
                            break;
                        }
                        case 3:
                        {
                            email->set_to(get_string("Destinatario").c_str());
                            break;
                        }
                        case 4:
                        {
                            email->set_cc(get_string("CC").c_str());
                            break;
                        }
                        case 5:
                        {
                            email->set_bcc(get_string("BCC").c_str());
                            break;
                        }
                        case 6:
                        {
                            email->set_subject(get_string("Asunto").c_str());
                            break;
                        }
                        case 7:
                        {
                            email->set_content(get_text("Contenido").c_str());
                            break;
                        }
                        default:
                            msg(INVALID_OPTION);
                    }
                    email_file.write(email, email->get_id());
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPT_DELETE:
            {
                Email* email;
                size_t pos = search_by();
                if (pos != (size_t)-1 && (email = email_file.read(pos)) && email != nullptr)
                {
                    display(email);
                    if(get_bool("Seguro?"))
                    {
                        Email empty;
                        email_file.write(&empty, email->get_id());
                    }
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPT_EXPORT_CSV:
            {
                email_file.for_each(append_to_csv);
                break;
            }
            case OPT_ERROR_FILE:
            {
                msg(MSG_ERROR_FILE);
                break;
            }
            default:
            {
                msg(INVALID_OPTION);
                break;
            }
        }
        pause();
    } while(opt != OPT_EXIT);
    return 0;
}
