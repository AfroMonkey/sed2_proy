#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
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

#define INVALID_OPTION "Opcion invalida\n"
#define MSG_DONE "Listo\n"
#define MSG_NOT_FOUND "No encontrado\n"
#define FILE_ERROR "Error en el archivo\n"
#define MSG_PAUSE "Presione entrar para continuar . . ."

void display_menu()
{
    cout << OPC_WRITE << ") Escribir" << endl;
    cout << OPC_READ << ") Leer" << endl;
    cout << OPC_MODIFY << ") Modificar" << endl;
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

string get_sring(string msg = ">")
{
    string s;
    std::cout << msg;
    cin >> s;
    return s;
}

string get_text(string msg = ">")
{
    string s, buffer;
    std::cout << msg << endl;
    int n = get_int("  Lienas>");
    for (int i = 0; i < n; i++)
    {
        getline(cin, buffer);
        s += buffer + '\n';
    }
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

/* ---------------------------------------------------------------------------*/

void fill(Email& email)
{
    email.set_id(get_int("ID>"));
    email.set_date(get_sring("Fecha (yyyymmdd)>").c_str());
    email.set_time(get_sring("Hora (hhmm)>").c_str());
    email.set_from(get_sring("Remitente>").c_str());
    email.set_to(get_sring("Destinatario>").c_str());
    email.set_cc(get_sring("CC>").c_str());
    email.set_bcc(get_sring("BCC>").c_str());
    email.set_subject(get_sring("Asunto>").c_str());
    email.set_content(get_text("Contenido>").c_str());
}

void display(Email& email)
{
    cout << "ID: " << email.get_id() << endl;
    cout << "Fecha: " << email.get_date() << endl;
    cout << "Hora: " << email.get_time() << endl;
    cout << "Remitente: " << email.get_from() << endl;
    cout << "Destinatario: " << email.get_to() << endl;
    cout << "CC: " << email.get_cc() << endl;
    cout << "BCC: " << email.get_bcc() << endl;
    cout << "Asunto: " << email.get_subject() << endl;
    cout << "Contenido: " << email.get_content() << endl;
}

#endif
