#ifndef GESTOR_H
#define GESTOR_H

#if defined(_WIN32) | defined(_WIN64)
    #define CLEAR system("cls")
#else
    #define CLEAR system("clear")
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include "usuario.h"
using namespace std;

class Gestor
{
public:
    Gestor();
    ~Gestor();

    void menu();
    void capturar(const Usuario& usuario);
    void mostrar();
    void modificar();
    void buscar();
    void eliminar();
    void escribir();
    void capturar_datos(Usuario& usuario);
    bool codigo_usado(const string codigo);
    void modificar_datos(Usuario& usuario, char i);
    unsigned char eliminar_ceros(string &cad);

private:
    vector<Usuario> m_usuarios;
    enum OPC
    {
        OPC_CAPTURAR = '1',
        OPC_MOSTRAR,
        OPC_BUSCAR,
        OPC_MODIFICAR,
        OPC_ELIMINAR,
        OPC_SALIR
    };

    enum CAMPOS
    {
        CAMPO_NOM = '1',
        CAMPO_APE,
        CAMPO_EDAD,
        CAMPO_SEXO,
        CAMPO_PESO,
        CAMPO_ALTURA,
        CAMPO_CANCELAR
    };
};

#endif