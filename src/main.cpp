#include <fstream>

#include "cli.hpp"
#include "email.hpp"

using namespace std;

#define EMAIL_FILE_PATH "email.bd"

int main(int argc, char const *argv[])
{
    short opc;
    do
    {
        clear_screen();
        display_menu();
        opc = get_int();
        switch (opc)
        {
            case OPC_EXIT:
            {
                break;
            }
            case OPC_WRITE:
            {
                Email email;
                fill(email);
                email.set_time(time(nullptr));
                ofstream file(EMAIL_FILE_PATH, ios::out);
                if (!file.is_open())
                {
                    msg(FILE_ERROR);
                    break;
                }
                file.seekp(email.get_id() * sizeof(Email), ios_base::beg);
                email.write(file);
                file.close();
                msg(MSG_DONE);
                break;
            }
            case OPC_READ:
            {
                Email email;
                int id = get_int("ID>");
                ifstream file(EMAIL_FILE_PATH, ios::in);
                if (!file.is_open())
                {
                    msg(FILE_ERROR);
                    break;
                }
                file.seekg(id * sizeof(Email), ios_base::beg);
                email.read(file);
                file.close();
                if (!file.eof() && email.get_id() != 0)
                {
                    display(email);
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            case OPC_MODIFY:
            {
                Email email;
                int id = get_int("ID>");
                fstream file(EMAIL_FILE_PATH, ios::in | ios::out);
                if (!file.is_open())
                {
                    msg(FILE_ERROR);
                    break;
                }
                file.seekg(id * sizeof(Email), ios_base::beg);
                email.read(file);
                if (!file.eof() && email.get_id() != 0)
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
                            email.set_time(time);
                            break;
                        }
                        case 2:
                        {
                            email.set_from(get_string().c_str());
                            break;
                        }
                        case 3:
                        {
                            email.set_to(get_string().c_str());
                            break;
                        }
                        case 4:
                        {
                            email.set_cc(get_string().c_str());
                            break;
                        }
                        case 5:
                        {
                            email.set_bcc(get_string().c_str());
                            break;
                        }
                        case 6:
                        {
                            email.set_subject(get_string().c_str());
                            break;
                        }
                        case 7:
                        {
                            email.set_content(get_text().c_str());
                            break;
                        }
                        default:
                            msg(INVALID_OPTION);
                    }
                    file.seekp(email.get_id() * sizeof(Email), ios_base::beg);
                    email.write(file);
                    file.write((char*)&email, sizeof(Email));
                    file.close();
                }
                else
                {
                    msg(MSG_NOT_FOUND);
                }
                break;
            }
            default:
            {
                msg(INVALID_OPTION);
                break;
            }
        }
        pause();
    } while(opc != OPC_EXIT);
    return 0;
}
