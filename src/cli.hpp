#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <cstring>

#include "email.hpp"

using namespace std;

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define OPC_EXIT 0
#define OPC_WRITE 1
#define OPC_READ 2
#define OPC_MODIFY 3
#define OPC_DELETE 4
#define OPC_ERROR_FILE -1

#define INVALID_OPTION "Opcion invalida\n"
#define MSG_DONE "Listo\n"
#define MSG_NOT_FOUND "No encontrado\n"
#define MSG_ERROR_FILE "Error en el archivo\n"
#define MSG_PAUSE "Presione entrar para continuar . . ."

void display_menu()
{
    cout << OPC_WRITE << ") Escribir" << endl;
    cout << OPC_READ << ") Leer" << endl;
    cout << OPC_MODIFY << ") Modificar" << endl;
    cout << OPC_DELETE << ") Eliminar" << endl;
    cout << OPC_EXIT << ") Salir" << endl;
}

int get_int(string msg = ">", int def = -1) {
    int i;
    cout << msg;
    if(!(cin >> i)) {
        cin.clear();
        cin.ignore();
        i = def;
    }
    cin.ignore();
    return i;
}

bool get_bool(string msg = ">")
{
    char c;
    cout << msg << "(y/n)";
    cin >> c;
    return c == 'y';
}

string get_string(string msg = ">")
{
    string s;
    cout << msg;
    getline(cin, s);
    return s;
}

string get_text(string msg = ">")
{
    char c;
    string s;
    cout << msg;
    while (cin.get(c))
    {
        s.append(1, c);
    }
    s.append(1, '\0');
    cin.clear();
    return s;
}

void clear_screen() {
    system(CLEAR);
}

void msg(string m) {
    cout << m;
}

void pause() {
    cout << MSG_PAUSE;
    cin.ignore();
}

string timet_to_date(time_t time)
{
    char date_c[11];
    if (!strftime(date_c, sizeof(date_c), "%Y-%m-%d", localtime(&time)))
    {
        strcpy(date_c, "1970-01-01");
    }
    string s(date_c, date_c+11);
    return s;
}

string timet_to_time(time_t time)
{
    char time_c[6];
    if (!strftime(time_c, sizeof(time_c), "%H:%M", localtime(&time)))
    {
        strcpy(time_c, "00:00");
    }
    string s(time_c, time_c+6);
    return s;
}

/* ---------------------------------------------------------------------------*/

void fill(Email* email)
{
    email->set_id(get_int("ID>"));
    email->set_from(get_string("Remitente>").c_str());
    email->set_to(get_string("Destinatario>").c_str());
    email->set_cc(get_string("CC>").c_str());
    email->set_bcc(get_string("BCC>").c_str());
    email->set_subject(get_string("Asunto>").c_str());
    email->set_content(get_text("Contenido>").c_str());
}

void display(Email* email)
{
    if (strcmp(email->get_from(), "") != 0)
    {
        cout << "ID: " << email->get_id() << endl;
        cout << "Fecha: " << timet_to_date(email->get_time()) << endl;
        cout << "Hora: " << timet_to_time(email->get_time()) << endl;
        cout << "Remitente: " << email->get_from() << endl;
        cout << "Destinatario: " << email->get_to() << endl;
        cout << "CC: " << email->get_cc() << endl;
        cout << "BCC: " << email->get_bcc() << endl;
        cout << "Asunto: " << email->get_subject() << endl;
        cout << "Contenido: " << email->get_content() << endl;
    }
    else
    {
        msg(MSG_NOT_FOUND);
    }
}


#endif
