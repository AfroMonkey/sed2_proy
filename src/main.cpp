#include <functional>

#include "cli.hpp"
#include "email.hpp"
#include "fixed_file_manager.hpp"
#include "csv_manager.hpp"
#include "dimenssion_file_manager.hpp"
#include "priority_list.hpp"
#include "avl.hpp"
#include "index.hpp"

using namespace std;

#define FIXED_PATH "email.db"
#define CSV_PATH "backup.csv"
#define DIM_PATH "backup.email"
#define INDEX_PATH "index.db"

FixedFileManager<Email> fixed_file(FIXED_PATH);
std::ofstream index_file;
CSV_Manager<Email> csv_file(CSV_PATH);
DimenssionFileManager<Email> dim_file(DIM_PATH, 9);

PriorityList<Email, Email::cmp_from> list;
Avl<Index, Index::compare> avl;

auto append_to_csv = std::bind(&CSV_Manager<Email>::append, &csv_file, std::placeholders::_1);
auto append_to_dim = std::bind(&DimenssionFileManager<Email>::append, &dim_file, std::placeholders::_1);
auto write_to_db = std::bind(&FixedFileManager<Email>::append, &fixed_file, std::placeholders::_1);

void to_ram(Email email)
{
    list.push(email);
}

void to_avl(Email email)
{
    Index index(email.get_id(), email.get_id());
    avl.insert(index);
}

void index_to_avl(Index index)
{
    avl.insert(index);
}

void avl_to_file(Index index)
{
    index_file.write((char*)&index, sizeof(Index));
}

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
            Email aux;
            aux.set_from(from);
            pos = fixed_file.find(aux, Email::cmp_from);
            return pos;
        }

    }
    return -1;
}

void csv_manage()
{
    display_bkp_submenu();
    switch(get_int())
    {
        case OPT_EXPORT_BKP:
        {
            if (get_bool(MSG_CONFIRM_DELETE))
            {
                remove(CSV_PATH);
                fixed_file.for_each(append_to_csv);
            }
            break;
        }
        case OPT_IMPORT_BKP:
        {
            if (get_bool(MSG_CONFIRM_DELETE))
            {
                fixed_file.clean();
                csv_file.for_each(write_to_db);
            }
            break;
        }
        case OPT_READ_BKP:
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
        case OPT_MODIFY_BKP:
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
        case OPT_DELETE_BKP:
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
    }
}

void dim_manage()
{
    display_bkp_submenu();
    switch(get_int())
    {
        case OPT_EXPORT_BKP:
        {
            if (get_bool(MSG_CONFIRM_DELETE))
            {
                remove(DIM_PATH);
                fixed_file.for_each(append_to_dim);
            }
            break;
        }
        case OPT_IMPORT_BKP:
        {
            if (get_bool(MSG_CONFIRM_DELETE))
            {
                fixed_file.clean();
                dim_file.for_each(write_to_db);
            }
            break;
        }
        case OPT_READ_BKP:
        {
            Email* email;
            size_t* num_row = new size_t;
            *num_row = 0;
            email = dim_file.find(get_int("ID>"), Email::compare_id, num_row);
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
        case OPT_MODIFY_BKP:
        {
            Email* email;
            size_t* num_row = new size_t;
            *num_row = 0;
            email = dim_file.find(get_int("ID>"), Email::compare_id, num_row);
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
                dim_file.write_in(email, *num_row);
                delete num_row;
                delete email;
            }
            else
            {
                msg(MSG_NOT_FOUND);
            }
            break;
        }
        case OPT_DELETE_BKP:
        {
            Email* email;
            size_t* num_row = new size_t;
            *num_row = 0;
            email = dim_file.find(get_int("ID>"), Email::compare_id, num_row);
            if (email != nullptr)
            {
                display(email);
                if(get_bool("Seguro?"))
                {
                    dim_file.write_in(nullptr, *num_row);
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
    }
}

void print_index(Index index)
{
    std::cout << index.key << "->" << index.address << std::endl;
}

void index_manage()
{
    msg("1) Crear\n");
    avl.empty() ? msg("2) Consultar (DESACTIVADO)\n") : msg("2) Consultar\n");
    avl.empty() ? msg("3) Guardar (DESACTIVADO)\n") : msg("2) Guardar\n");
    msg("4) Cargar\n");
    switch (get_int())
    {
        case 1:
        {
            avl.trim(avl.root());
            fixed_file.for_each(to_avl);
            avl.preorder(avl.root(), print_index);
            break;
        }
        case 2:
        {
            if (avl.empty())
            {
                msg(INVALID_OPTION);
                break;
            }
            Index index(get_int("ID>"), 1);
            AvlNode<Index>* node = avl.get(index);
            if (node)
            {
                display(fixed_file.read(node->data.address));
            }
            else
            {
                msg(MSG_NOT_FOUND);
            }
            break;
        }
        case 3:
        {
            if (avl.empty())
            {
                msg(INVALID_OPTION);
                break;
            }
            index_file.open(INDEX_PATH, std::ios::in | std::ios::binary | std::ios::trunc);
            avl.preorder(avl.root(), avl_to_file);
            index_file.close();
            break;
        }
        case 4:
        {
            avl.trim(avl.root());
            FixedFileManager<Index> aux(INDEX_PATH);
            aux.for_each(index_to_avl);
            break;
        }
        default:
        {
            break;
        }
    }
}

int main()
{
    short opt;
    bool in_ram = false;
    do
    {
        clear_screen();
        display_menu(in_ram);
        opt = get_int();
        switch (opt)
        {
            case OPT_EXIT:
            {
                break;
            }
            case OPT_WRITE:
            {
                if (in_ram)
                {
                    msg(ERROR_RAM);
                    break;
                }
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
                if (in_ram)
                {
                    char c_from[256];
                    strcpy(c_from, get_string("Remitente>").c_str());
                    Email from;
                    from.set_from(c_from);
                    SLN<Email>* node = list.find(from);
                    if (node)
                    {
                        display(&(node->data));
                    }
                    else
                    {
                        msg(MSG_NOT_FOUND);
                    }
                }
                else
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
                }
                break;
            }
            case OPT_MODIFY:
            {
                if (in_ram)
                {
                    msg(ERROR_RAM);
                    break;
                }
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
                if (in_ram)
                {
                    msg(ERROR_RAM);
                    break;
                }
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
            case OPT_MANAGE_BKP:
            {
                display_bkp_menu();
                switch (get_int())
                {
                    case OPT_MANAGE_CSV:
                    {
                        csv_manage();
                        break;
                    }
                    case OPT_MANAGE_DIM:
                    {
                        dim_manage();
                        break;
                    }
                    default:
                    {
                        msg(INVALID_OPTION);
                        break;
                    }
                }
                break;
            }
            case OPT_TOGGLE_RAM:
            {
                in_ram = !in_ram;
                if (in_ram)
                {
                    fixed_file.for_each(to_ram);
                }
                else
                {
                    list.clear();
                }
                break;
            }
            case OPT_MANAGE_INDEX:
            {
                index_manage();
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
