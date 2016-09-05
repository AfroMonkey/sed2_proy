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
                ofstream file(EMAIL_FILE_PATH, ios::out);
                if (!file.is_open())
                {
                    msg(FILE_ERROR);
                    break;
                }
                file.seekp(email.get_id() * sizeof(Email), ios_base::beg);
                file.write((char*)&email, sizeof(Email));
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
                file.read((char*)&email, sizeof(Email));
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
                file.read((char*)&email, sizeof(Email));
                if (!file.eof() && email.get_id() != 0)
                {
                    display(email);
                    msg("1) Fecha\n");
                    msg("2) Hora\n");
                    msg("3) Remitente\n");
                    msg("4) Destinatario\n");
                    msg("5) CC\n");
                    msg("6) BCC\n");
                    msg("7) Asunto\n");
                    msg("8) Contenido\n");
                    switch (get_int())
                    {
                        case 1:
                            email.set_date(get_sring().c_str());
                            break;
                        case 2:
                            email.set_time(get_sring().c_str());
                            break;
                        case 3:
                            email.set_from(get_sring().c_str());
                            break;
                        case 4:
                            email.set_to(get_sring().c_str());
                            break;
                        case 5:
                            email.set_cc(get_sring().c_str());
                            break;
                        case 6:
                            email.set_bcc(get_sring().c_str());
                            break;
                        case 7:
                            email.set_subject(get_sring().c_str());
                            break;
                        case 8:
                            email.set_content(get_text().c_str());
                            break;
                        default:
                            msg(INVALID_OPTION);
                    }
                    file.seekp(email.get_id() * sizeof(Email), ios_base::beg);
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
