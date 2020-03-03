#include "gestor.h"
#define CHAR_SIZE 1

Gestor::Gestor()
{
    string  cadTmp;
    char charTmp;
    unsigned char tam;
    fstream archivo("usuarios.txt", ios::out | ios::in);
    if (!archivo.is_open())
    {
        fstream archivo_abrir("usuarios.txt", ios::out);
        archivo_abrir.close();
    }
    else
    {
        while (!archivo.eof())
            for (int i = 0; i < CANTIDAD_CAMPOS; ++i)
            {
                cadTmp = "";
                archivo.read((char*)&tam, sizeof(tam));

                if (archivo.eof())
                    break;

                while (int(tam--))
                {
                    archivo.get(charTmp);
                    cadTmp += charTmp;
                }
                if (!i && cadTmp.length())
                    m_codigos.push_back(cadTmp);
            }
    }
}

Gestor::~Gestor()
{}

void Gestor::menu()
{
    char opc;
    do
    {
        CLEAR;
        cout << " Ingrese la opción a ejecutar: " << endl
             << " " << char(OPC_CAPTURAR) << ") Capturar usuario" << endl
             << " " << char(OPC_MOSTRAR) << ") Mostrar usuarios" << endl
             << " " << char(OPC_BUSCAR) << ") Buscar usuario (por código)" << endl
             << " " << char(OPC_MODIFICAR) << ") Modificar usuario" << endl
             << " " << char(OPC_ELIMINAR) << ") Eliminar usuario" << endl
             << " " << char(OPC_SALIR) << ") Salir" << endl
             << " Opción: ";

        cin >> opc;
        CLEAR;
        cin.ignore();
        switch (opc)
        {
            case OPC_CAPTURAR:
            {
                Usuario usuarioTmp;
                capturar_datos(usuarioTmp);
                capturar(usuarioTmp);
                cin.ignore();
            }
            break;

            case OPC_MODIFICAR:
                modificar();
            break;

            case OPC_BUSCAR:
                buscar();
            break;

            case OPC_ELIMINAR:
                eliminar();
            break;

            case OPC_MOSTRAR:
                CLEAR;
                mostrar();
            break;

            case OPC_SALIR:
                cout << " Hasta la próxima" << endl;
            return;

            default:
                cout << " Dato inválido, presione ENTER para continuar..." << endl;
            break;
        }
        cin.get();
    } while (opc != OPC_SALIR);
}

void Gestor::buscar()
{
    string codigo;
    unsigned int i;
    if (!m_usuarios.size())
        cout << " Aún no se han ingresado usuarios " << endl << endl
             << " Presione ENTER para continuar..." << endl;
    else
    {
        cout << " Ingrese el código a buscar: ";
        getline(cin, codigo);
        for (i = 0; i < m_usuarios.size(); i++)
            if(m_usuarios[i].getCodigo() == codigo)
            {
                CLEAR;
                cout << endl
                    << " Usuario #" << i + 1 << endl
                    << " Código: " << m_usuarios[i].getCodigo() << endl
                    << " Nombre: " << m_usuarios[i].getNombre() << endl
                    << " Apellido: " << m_usuarios[i].getApellido() << endl
                    << " Edad: " << m_usuarios[i].getEdad() << endl
                    << " Género: " << m_usuarios[i].getGenero() << endl
                    << " Peso: " << m_usuarios[i].getPeso() << endl
                    << " Altura: " << m_usuarios[i].getAltura() << endl
                    << "----------------------------------------------"
                    << endl << endl
                    << " Presione ENTER para continuar..." << endl;
                break;
            }
        if (i == m_usuarios.size())
            cout << endl
                << " Código no encontrado, presione ENTER para continuar..." << endl;
    }
}

void Gestor::capturar(const Usuario& usuario)
{
    string aux = "";
    string aux2 = "";
    unsigned char tam;
    fstream archivo("usuarios.txt", ios::out | ios::in | ios::app);

    if (!archivo.is_open())
        cerr << "Error en el archivo de salida" << endl;
    
    tam = usuario.getCodigo().length();
    m_codigos.push_back(usuario.getCodigo());
    archivo.write((char*)&tam, sizeof(tam));
    archivo << usuario.getCodigo();

    tam = usuario.getNombre().length();
    archivo.write((char*)&tam, sizeof(tam));
    archivo << usuario.getNombre();
    
    tam = usuario.getApellido().length();
    archivo.write((char*)&tam, sizeof(tam));
    archivo << usuario.getApellido();
    
    tam = to_string(usuario.getEdad()).length();
    archivo.write((char*)&tam, sizeof(tam));
    archivo << usuario.getEdad();

    tam = CHAR_SIZE;
    archivo.write((char*)&tam, sizeof(tam));
    archivo << usuario.getGenero();

    aux = to_string(usuario.getPeso());
    tam = aux.length();
    archivo.write((char*)&tam, sizeof(tam));
    archivo << aux;

    aux2 = to_string(usuario.getAltura());
    tam = aux2.length();
    archivo.write((char*)&tam, sizeof(tam));
    archivo << aux2;
    
    archivo.close();
    cout << endl
         << " Usuario añadido exitosamente." << endl
         << " Presione ENTER para continuar..." << endl;
}

void Gestor::eliminar()
{
    bool found = false;
    unsigned int opc;
    unsigned char tam;
    char auxChar;
    string aux;
    fstream archivo("usuarios.txt", ios::in | ios::out);
    fstream tmp("usuarios.tmp", ios::out);

    mostrar();
    if (m_codigos.size())
    {
        cout << " Ingrese número del usuario a eliminar: ";
        cin >> opc;
        if (opc <= m_codigos.size() && opc)
        {
            while (!archivo.eof())
            {
                for (int i = 0; i < CANTIDAD_CAMPOS; i++)
                {
                    aux = "";
                    archivo.read((char*)&tam, sizeof(tam));
                    if (archivo.eof())
                        break;
                    
                    for (int j = 0; j < int(tam); ++j)
                    {
                        archivo.get(auxChar);
                        aux += auxChar;
                    }
                    if (!i && m_codigos[opc - 1] == aux)
                    {
                        cout << "adsf" << endl;
                        found = true;
                    }
                    else if (found && i == CANTIDAD_CAMPOS - 1)
                        found = false;
                    else if (!found)
                    {
                        tmp.write((char*)&tam, sizeof(tam));
                        tmp << aux;         
                    }
                }
            }
            m_codigos.erase(m_codigos.begin() + opc - 1);
        }
        else
        {

            cout << endl
                << " Dato inválido, presione ENTER para continuar..."
                << endl;
            cin.get();
        }
    }
    tmp.close();
    archivo.close();

    remove("usuarios.txt");
    rename("usuarios.tmp", "usuarios.txt");
}

void Gestor::modificar()
{   
    Usuario usuarioTmp;
    unsigned int i;
    char opc;

    mostrar();
    if (m_usuarios.size())
    {
        cout << " Ingrese número del usuario a modificar: ";
        cin >> i;

        if (i <= m_usuarios.size() && i)
        {   
            do
            {
                cout << endl
                     << " Seleccione el campo a modificar:" << endl;
                cout << char(OPC_CAMPO_NOM) << ") Nombre" << endl
                     << char(OPC_CAMPO_APE) << ") Apellido" << endl
                     << char(OPC_CAMPO_EDAD) << ") Edad" << endl
                     << char(OPC_CAMPO_SEXO) << ") Sexo" << endl
                     << char(OPC_CAMPO_PESO) << ") Peso" << endl
                     << char(OPC_CAMPO_ALTURA) << ") Altura" << endl
                     << char(OPC_CAMPO_CANCELAR) << ") Cancelar" << endl
                     << "Opción: ";
                cin >> opc;
            }while(opc < OPC_CAMPO_NOM || opc > OPC_CAMPO_CANCELAR);
            
            if (opc != OPC_CAMPO_CANCELAR)
            {
                usuarioTmp.setAltura((m_usuarios.begin() + i - 1)->getAltura());
                usuarioTmp.setApellido((m_usuarios.begin() + i - 1)->getApellido());
                usuarioTmp.setCodigo((m_usuarios.begin() + i - 1)->getCodigo());
                usuarioTmp.setEdad((m_usuarios.begin() + i - 1)->getEdad());
                usuarioTmp.setGenero((m_usuarios.begin() + i - 1)->getGenero());
                usuarioTmp.setNombre((m_usuarios.begin() + i - 1)->getNombre());
                usuarioTmp.setPeso((m_usuarios.begin() + i - 1)->getPeso());

                modificar_datos(usuarioTmp, opc);

                m_usuarios.erase(m_usuarios.begin() + i -1);
                m_usuarios.insert(m_usuarios.begin() + i - 1, usuarioTmp);

                escribir();

                cout << endl
                     << " Dato modificado correctamente" << endl
                     << " Presione ENTER para continuar..." << endl;
            }
        }

        else
        {
            cout << endl
                 << " Dato inválido, presione ENTER para continuar..."
                 << endl;
            cin.get();       
        }
    }
}

void Gestor::mostrar()
{
    unsigned long cont = 0;
    fstream archivo("usuarios.txt", ios::in);
    Usuario usuarioTmp;
    string cadTmp;
    char charTmp;
    unsigned char tam;

    if (!archivo.is_open())
        cout << " Error en el archivo de entrada" << endl;
    else
    {
        while (!archivo.eof())
        {

            for (int i = 0; i < CANTIDAD_CAMPOS; ++i)
            {
                cadTmp = "";
                archivo.read((char*)&tam, sizeof(tam));

                if (archivo.eof())
                    return;

                while (int(tam--))
                {
                    archivo.get(charTmp);
                    cadTmp += charTmp;
                }
                switch (i)
                {
                    case CAMPO_ALTURA:
                        usuarioTmp.setAltura(stof(cadTmp));
                    break;
                    case CAMPO_APE:
                        usuarioTmp.setApellido(cadTmp);
                    break;
                    case CAMPO_NOM:
                        usuarioTmp.setNombre(cadTmp);
                    break;
                    case CAMPO_COD:
                        if (cadTmp.length())
                            ++cont;
                        usuarioTmp.setCodigo(cadTmp);
                    break;
                    case CAMPO_PESO:
                        usuarioTmp.setPeso(stof(cadTmp));
                    break;
                    case CAMPO_SEXO:
                        usuarioTmp.setGenero(cadTmp[0]);
                    break;
                    case CAMPO_EDAD:
                        usuarioTmp.setEdad(stoi(cadTmp));
                    break;
                }
            }
            if (cont)
                cout << " Usuario #" << cont << endl
                    << " Código: " << usuarioTmp.getCodigo() << endl
                    << " Nombre: " << usuarioTmp.getNombre() << endl
                    << " Apellido: " << usuarioTmp.getApellido() << endl
                    << " Edad: "   << usuarioTmp.getEdad() << endl
                    << " Género: " << usuarioTmp.getGenero() << endl
                    << " Peso: " << usuarioTmp.getPeso() << endl
                    << " Altura: " << usuarioTmp.getAltura() << endl
                    << "----------------------------------------------"
                    << endl;
            else
                cout << " Aún no se han ingresado usuarios" << endl;
        }
        archivo.close();
    }
}

void Gestor::escribir()
{
    fstream archivo("usuarios.txt", ios::out);
    if (!archivo.is_open())
        cerr << " Error en el archivo" << endl;
    else
        for (int i = 0; i < m_usuarios.size(); i++)
            archivo << m_usuarios[i].getCodigo() << '|'
                    << m_usuarios[i].getNombre() << '|'
                    << m_usuarios[i].getApellido() << '|'
                    << m_usuarios[i].getEdad() << '|'
                    << m_usuarios[i].getGenero() << '|'
                    << m_usuarios[i].getPeso() << '|'
                    << m_usuarios[i].getAltura() << '\n';
}

void Gestor::capturar_datos(Usuario& usuario)
{
    string codigo;
    string nombre;
    string apellido;
    string genero;
    unsigned int edad;
    float altura;
    float peso;
    bool continuar = false;

    // Expresiones regulares
    regex expCodigo("([1-9]{1}[0-9]{8})$");
    regex expNombre("(?:[a-zA-ZñÑ]{4,})(?: [a-zA-ZñÑ]{4,})?{1,2}");
    regex expApellido("(?:[a-zA-ZñÑ]{4,})+(?: [a-zA-ZñÑ]{3,})$");
    regex expGenero("(?:[mMfF]){1}$");
    
    cout << " Presione ENTER para continuar e ingrese los siguientes datos"
         << endl << endl;

    // Obtención de código
    do
    {
        CLEAR;
        cout << " Código (9 dígitos): ";
        getline(cin, codigo);
        cout << codigo_usado(codigo) << endl;
        if (codigo_usado(codigo))
        {
            cout << endl
                 << " Error, código en uso. Presione ENTER para continuar";
            cin.get();
        }
        else if (!regex_match(codigo, expCodigo))
        {
            cout << endl
                 << " Error el código debe de tener 9 dígitos y no debe de"
                 << " empezar con cero" << endl
                 << " Presione ENTER para continuar";
            cin.get();
        }
        else
            continuar = true;
    } while (!continuar);
    continuar = false;

    // Obtención de nombre
    do
    {
        CLEAR;
        cout << " Nombre: ";
        getline(cin, nombre);
        if (!regex_match(nombre, expNombre))
        {
            cout << " Error, introducir mínimo un nombre y máximo tres "
                 << endl
                 << " (mínimo cuatro letras por nombre)"
                 << endl
                 << " Presione ENTER para continuar"
                 << endl;
            cin.get();
        }
        else
            continuar = true;
    } while (!continuar);
    continuar = false;

    // Obtención de apellido
    do
    {
        CLEAR;
        cout << " Apellido: ";
        getline(cin, apellido);
        if (!regex_match(apellido, expApellido))
        {
            cout << " Error, se deben de introducir dos apellidos"
                 << " separados por espacios"
                 << endl
                 << " Presione ENTER para continuar"
                 << endl;
            cin.get();
        }
        else
            continuar = true;
    }while(!continuar);

    do
    {
        CLEAR;
        cout << " Edad (de 12 a 115 años): ";
        cin >> edad;
    }while(edad > 115 || edad < 12);

    do
    {
        CLEAR;
        cout << " Género (M = masculino | F = femenino): ";
        cin >> genero;
    } while(!regex_match(genero, expGenero));

    do
    {
        CLEAR;
        cout << " Peso (de 30 a 300 kg): ";
        cin >> peso;
    }while(peso > 300 || peso < 30);

    do
    {   
        CLEAR;
        cout << " Altura (de 0.8 a 2.5 mts): ";
        cin >> altura;;
    } while (altura < 0.8 || altura > 2.5);

    usuario.setAltura(altura);
    usuario.setApellido(apellido);
    usuario.setCodigo(codigo);
    usuario.setEdad(edad);
    usuario.setGenero(genero[0]);
    usuario.setNombre(nombre);
    usuario.setPeso(peso);
}

bool Gestor::codigo_usado(const string codigo)
{
    for (int i = 0; i < m_codigos.size(); i++)
        if (codigo == m_codigos[i])
            return true;
    return false;
}

void Gestor::modificar_datos(Usuario& usuario, char i)
{
    bool continuar = false;

    // Expresiones regulares
    regex expCodigo("([1-9]{1}[0-9]{8})$");
    regex expNombre("(?:[a-zA-ZñÑ]{4,})(?: [a-zA-ZñÑ]{4,})?{1,2}");
    regex expApellido("(?:[a-zA-ZñÑ]{4,})+(?: [a-zA-ZñÑ]{3,})$");
    regex expGenero("(?:[mMfF]){1}$");

    cin.ignore();
    switch (i)
    {
        case CAMPO_NOM:
        {
            string nombre;
            do
            {
                CLEAR;
                cout << " Nombre: ";
                getline(cin, nombre);
                if (!regex_match(nombre, expNombre))
                {
                    cout << " Error, introducir mínimo un nombre y máximo tres "
                        << endl
                        << " (mínimo cuatro letras por nombre)"
                        << endl
                        << " Presione ENTER para continuar"
                        << endl;
                    cin.get();
                }
                else
                    continuar = true;
            } while (!continuar);
            usuario.setNombre(nombre);
        }
        break;

        case CAMPO_APE:
        {
            string apellido;
            do
            {
                CLEAR;
                cout << " Apellido: ";
                getline(cin, apellido);
                if (!regex_match(apellido, expApellido))
                {
                    cout << " Error, se deben de introducir dos apellidos"
                        << " separados por espacios"
                        << endl
                        << " Presione ENTER para continuar"
                        << endl;
                    cin.get();
                }
                else
                    continuar = true;
            }while(!continuar);
            usuario.setApellido(apellido);
        }
        break;

        case CAMPO_EDAD:
        {
            unsigned int edad;
            do
            {
                CLEAR;
                cout << " Edad (de 12 a 115 años): ";
                cin >> edad;
            }while(edad > 115 || edad < 12);
            usuario.setEdad(edad);
        }
        break;

        case CAMPO_PESO:
        {
            float peso;
            do
            {
                CLEAR;
                cout << " Peso (de 30 a 300 kg): ";
                cin >> peso;
            }while(peso > 300 || peso < 30);
            usuario.setPeso(peso);
        }
        break;

        case CAMPO_SEXO:
        {
            string genero;
            do
            {
                CLEAR;
                cout << " Género (M = masculino | F = femenino): ";
                cin >> genero;
            } while(!regex_match(genero, expGenero));
            usuario.setGenero(genero[0]);
        }
        break;

        case CAMPO_ALTURA:
        {
            float altura;
            do
            {   
                CLEAR;
                cout << " Altura (de 0.8 a 2.5 mts): ";
                cin >> altura;;
            } while (altura < 0.8 || altura > 2.5);
            usuario.setAltura(altura);
        }
        break;
    }
}