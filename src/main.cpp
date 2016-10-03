#include <functional>

#include "cli.hpp"
#include "email.hpp"
#include "fixed_file_manager.hpp"
#include "csv_manager.hpp"

using namespace std;

#define FIXED_PATH "email.db"
#define CSV_PATH "backup.csv"

FixedFileManager<Email> fixed_file(FIXED_PATH);
CSV_Manager<Email> csv_file(CSV_PATH);


auto append_to_csv = std::bind(&CSV_Manager<Email>::append, &csv_file, std::placeholders::_1);
auto write_to_db = std::bind(&FixedFileManager<Email>::append, &fixed_file, std::placeholders::_1);

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
            pos = fixed_file.find(from, Email::cmp_from);
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
        clear_screen();
        display_menu();
        opt = get_int();
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
                fixed_file.write(email, email->get_id());
                delete email;
                msg(MSG_DONE);
                break;
            }
            case OPT_READ:
            {
                Email* email;
                size_t pos = search_by();
                if (pos != (size_t)-1 && (email = fixed_file.read(pos)) && email != nullptr)
                {
                    display(email);
                    delete email;
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
                if (pos != (size_t)-1 && (email = fixed_file.read(pos)) && email != nullptr)
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
                            email->set_from(get_string("Remitente>").c_str());
                            break;
                        }
                        case 3:
                        {
                            email->set_to(get_string("Destinatario>").c_str());
                            break;
                        }
                        case 4:
                        {
                            email->set_cc(get_string("CC>").c_str());
                            break;
                        }
                        case 5:
                        {
                            email->set_bcc(get_string("BCC>").c_str());
                            break;
                        }
                        case 6:
                        {
                            email->set_subject(get_string("Asunto>").c_str());
                            break;
                        }
                        case 7:
                        {
                            email->set_content(get_text("Contenido>").c_str());
                            break;
                        }
                        default:
                            msg(INVALID_OPTION);
                    }
                    fixed_file.write(email, email->get_id());
                    delete email;
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
                if (pos != (size_t)-1 && (email = fixed_file.read(pos)) && email != nullptr)
                {
                    display(email);
                    if(get_bool("Seguro?"))
                    {
                        Email empty;
                        fixed_file.write(&empty, email->get_id());
                    }
                    delete email;
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPT_EXPORT_CSV:
            {
                fixed_file.for_each(append_to_csv);
                break;
            }
            case OPT_IMPORT_CSV:
            {
                if (get_bool("Seguro? esto borrara el archivo anterior"))
                {
                    fixed_file.clean();
                    csv_file.for_each(write_to_db);
                }
            }
            case OPT_READ_CSV:
            {
                Email* email;
                size_t* num_row = new size_t;
                *num_row = 0;
                email = csv_file.find(get_int("ID>"), Email::compare_id, num_row);
                if (email != nullptr)
                {
                    display(email);
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPT_MODIFY_CSV:
            {
                Email* email;
                size_t* num_row = new size_t;
                *num_row = 0;
                email = csv_file.find(get_int("ID>"), Email::compare_id, num_row);
                if (email != nullptr)
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
                            email->set_from(get_string("Remitente>").c_str());
                            break;
                        }
                        case 3:
                        {
                            email->set_to(get_string("Destinatario>").c_str());
                            break;
                        }
                        case 4:
                        {
                            email->set_cc(get_string("CC>").c_str());
                            break;
                        }
                        case 5:
                        {
                            email->set_bcc(get_string("BCC>").c_str());
                            break;
                        }
                        case 6:
                        {
                            email->set_subject(get_string("Asunto>").c_str());
                            break;
                        }
                        case 7:
                        {
                            email->set_content(get_text("Contenido>").c_str());
                            break;
                        }
                        default:
                            msg(INVALID_OPTION);
                    }
                    csv_file.write_in(email, *num_row);
                    delete num_row;
                    delete email;
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPT_DELETE_CSV:
            {
                Email* email;
                size_t* num_row = new size_t;
                *num_row = 0;
                email = csv_file.find(get_int("ID>"), Email::compare_id, num_row);
                if (email != nullptr)
                {
                    display(email);
                    if(get_bool("Seguro?"))
                    {
                        csv_file.write_in(nullptr, *num_row);
                        delete num_row;
                        delete email;
                    }
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
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
