#include <functional>

#include "cli.hpp"
#include "email.hpp"
#include "fixed_file_manager.hpp"
#include "csv_manager.hpp"
#include "dimenssion_file_manager.hpp"
#include "priority_list.hpp"
#include "avl.hpp"
#include "index.hpp"
#include <list>

using namespace std;

#define FIXED_PATH "email.db"
#define CSV_PATH "backup.csv"
#define DIM_PATH "backup.email"
#define INDEX_PATH "primary.idx"
#define FROM_INDEX_PATH "from.idx"

FixedFileManager<Email> fixed_file(FIXED_PATH);
std::ofstream index_file;
std::ofstream from_index_file;
CSV_Manager<Email> csv_file(CSV_PATH);
DimenssionFileManager<Email> dim_file(DIM_PATH, 9);

int compare_int_index(Index<int, int> &a, Index<int, int> &b)
{
    return a.key - b.key;
}

int compare_char_index(Index<std::string, std::list<int> >  &a, Index<std::string, std::list<int> >  &b)
{
    return strcmp(a.key.c_str(), b.key.c_str());
}

PriorityList<Email, Email::cmp_from> priority_list;
Avl<Index<int, int>, compare_int_index> avl;
Avl<Index<std::string, std::list<int> > , compare_char_index> avl_from;

auto append_to_csv = std::bind(&CSV_Manager<Email>::append, &csv_file, std::placeholders::_1);
auto append_to_dim = std::bind(&DimenssionFileManager<Email>::append, &dim_file, std::placeholders::_1);
auto write_to_db = std::bind(&FixedFileManager<Email>::append, &fixed_file, std::placeholders::_1);

void to_ram(Email email)
{
    priority_list.push(email);
}

void to_avl(Email email)
{
    Index<int, int> index(email.get_id(), email.get_id());
    avl.insert(index);
}

void to_avl_from(Email email)
{
    // Index<std::string, std::list<int> > index(email.get_from());
    // AvlNode<Index<std::string, std::list<int> > > *aux = avl_from.get(index);
    // if (aux)
    // {
    //     aux->data.address.push_back(email.get_id());
    //     std::cout << "repetido" << std::endl;
    //     std::cout << "email " << email.get_from() << std::endl;
    //     std::cout << "aux " << aux->data.key << std::endl << std::endl;
    // }
    // else
    // {
    //     std::list<int> id_list;
    //     id_list.push_back(email.get_id());
    //     Index<std::string, std::list<int> > *index2 = new Index<std::string, std::list<int> >(email.get_from(), id_list);
    //     avl_from.insert(*index2);
    //     std::cout << "insert" << std::endl;
    //     std::cout << "email " << email.get_from() << std::endl << std::endl;
    // }
    Index<std::string, std::list<int> > index(email.get_from());
    index.address.push_back(email.get_id());
    avl_from.insert(index);
}

void index_to_avl(Index<int, int> index)
{
    avl.insert(index);
}

void index_to_avl_from(Index<std::string, std::list<int> >  index)
{
    avl_from.insert(index);
}

void avl_to_file(Index<int, int> index)
{
    index_file.write((char*)&index, sizeof(Index<int, int>));
}

void multiple_addres_to_file(int data)
{
    from_index_file.write((char*)&data, sizeof(int));
}

void avl_from_to_file(Index<std::string, std::list<int> >  index)
{
    from_index_file.write(index.key.c_str(), 256);
    unsigned int nodes = (index.address.size());
    from_index_file.write((char*)&nodes, sizeof(unsigned));
    for (auto n : index.address) {
        multiple_addres_to_file(n);
    }
    // index.address->for_each(multiple_addres_to_file);
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

void index_manage()
{
    msg("1) Crear\n");
    avl.empty() ? msg("2) Consultar (DESACTIVADO)\n") : msg("2) Consultar\n");
    avl.empty() ? msg("3) Guardar (DESACTIVADO)\n") : msg("3) Guardar\n");
    msg("4) Cargar\n");
    msg("5) Eliminar algun indice\n");
    switch (get_int())
    {
        case 1:
        {
            avl.trim(avl.root());
            fixed_file.for_each(to_avl);
            break;
        }
        case 2:
        {
            if (avl.empty())
            {
                msg(INVALID_OPTION);
                break;
            }
            Index<int, int> index(get_int("ID>"), 1);
            AvlNode<Index<int, int>>* node = avl.get(index);
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
            FixedFileManager<Index<int, int>> aux(INDEX_PATH);
            aux.for_each(index_to_avl);
            break;
        }
        case 5:
	{
            Index<int, int> index(get_int("ID>"), 1);
            AvlNode<Index<int, int>>* node = avl.get(index);
            if (node)
            {
                display(fixed_file.read(node->data.address));
                if (get_bool("Seguro"))
                {
                    avl.remove(node);
                }
            }
            else
            {
                msg(MSG_NOT_FOUND);
            }
	}
        default:
        {
            break;
        }
    }
}

void display_node_from_file(SLN<int>* node)
{
    display(fixed_file.read(node->data));
    msg("\n");
}

void print_from(Index<std::string, std::list<int> >  index)
{
    msg(index.key);
}

void from_index_manage()
{
    msg("1) Crear\n");
    avl_from.empty() ? msg("2) Consultar (DESACTIVADO)\n") : msg("2) Consultar\n");
    avl_from.empty() ? msg("3) Guardar (DESACTIVADO)\n") : msg("3) Guardar\n");
    msg("4) Cargar\n");
    // msg("5) Eliminar algun indice\n");
    switch (get_int())
    {
        case 1:
        {
            avl_from.trim(avl_from.root());
            fixed_file.for_each(to_avl_from);
            break;
        }
        case 2:
        {
            if (avl_from.empty())
            {
                msg(INVALID_OPTION);
                break;
            }
            // Index<std::string, std::list<int> >  index(get_char("Remitente>"));
            // AvlNode<Index<std::string, std::list<int> > >* node = avl_from.get(index);
            avl_from.preorder(avl_from.root(), print_from);
            // if (node)
            // {
            //     //TODO display(fixed_file.read(node->data.address));
            //     node->data.address->for_each(display_node_from_file);
            // }
            // else
            // {
            //     msg(MSG_NOT_FOUND);
            // }
            break;
        }
        case 3:
        {
            if (avl_from.empty())
            {
                msg(INVALID_OPTION);
                break;
            }
            index_file.open(FROM_INDEX_PATH, std::ios::in | std::ios::binary | std::ios::trunc);
            avl_from.preorder(avl_from.root(), avl_from_to_file);
            index_file.close();
            break;
        }
        case 4:
        {
            avl_from.trim(avl_from.root());
            FixedFileManager<Index<std::string, std::list<int> > > aux(FROM_INDEX_PATH);
            aux.for_each(index_to_avl_from);
            break;
        }
    //     case 5:
    //     {
    //         Index<std::string, std::list<int> >  index(get_char("Remitente>"));
    //         AvlNode<Index<std::string, std::list<int> > >* node = avl_from.get(index);
    //         if (node)
    //         {
    //             //TODO display(fixed_file.read(node->data.address));
    //             if (get_bool("Seguro"))
    //             {
    //                 avl_from.remove(node);
    //             }
    //         }
    //         else
    //         {
    //             msg(MSG_NOT_FOUND);
    //         }
    //    }
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
                    SLN<Email>* node = priority_list.find(from);
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
                    priority_list.clear();
                }
                break;
            }
            case OPT_MANAGE_INDEX:
            {
                index_manage();
                break;
            }
            case OPT_MANAGE_INDEX_FROM:
            {
                from_index_manage();
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
