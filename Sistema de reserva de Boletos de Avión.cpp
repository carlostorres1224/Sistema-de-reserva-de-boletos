#include <iostream>
#include <fstream>
#include "Usuario.h"
#include "Cliente.h"
#include "AdministradorDeSesion.h"
#include "Menu.h"

using namespace std;

void CrearAdminPorDefecto() {
    ifstream archivo("admin.txt");
    if (!archivo.is_open() || archivo.peek() == ifstream::traits_type::eof()) {
        ofstream salida("admin.txt", ios::app);
        salida << "1 admin1 admin@tec.mx 1234 admin" << endl;
        salida.close();
    }
}

int main()
{
    CrearAdminPorDefecto(); 

    AdministradorDeSesion adminDeSesion;
    Menu menu;
    Cliente cliente;

    int opcion;
    do {
        system("cls");
        cout << "1 - Iniciar sesión\n";
        cout << "2 - Salir\n";
        cout << "Admin predefinido: admin@tec.mx 1234\n";
        cin >> opcion;

        if (opcion == 1)
        {
            system("cls");
            int tipoUsuario;
            cout << "1 - Administrador\n2 - Cliente\n";
            cin >> tipoUsuario;

            if (tipoUsuario == 1)
            {
                string correo, password;
                cout << "Correo: "; cin >> correo;
                cout << "Password: "; cin >> password;

                auto admins = Usuario::ObtenerUsuarios("admin.txt");

                if (adminDeSesion.login(correo, password, admins))
                {
                    auto adminLogueado = adminDeSesion.getSesionUsuario(admins.back()->getId());

                    int opcionAdmin;
                    do {
                        system("cls");
                        cout << "---- PANEL ADMIN ----\n";
                        cout << "1 - Registrar administrador\n";
                        cout << "2 - Registrar cliente\n";
                        cout << "3 - Cerrar sesión\n";
                        cin >> opcionAdmin;

                        if (opcionAdmin == 1)
                        {
                            string nombre, correoN, passwordN;
                            cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                            cout << "Correo: "; cin >> correoN;
                            cout << "Password: "; cin >> passwordN;
                            size_t nuevoId = admins.size() + 1;
                            Usuario::RegistrarUsuario("admin.txt", nuevoId, nombre, correoN, passwordN, "admin");
                        }
                        else if (opcionAdmin == 2)
                        {
                            string nombre, correoN, passwordN;
                            cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                            cout << "Correo: "; cin >> correoN;
                            cout << "Password: "; cin >> passwordN;
                            auto clientesList = Usuario::ObtenerUsuarios("clientes.txt");
                            size_t nuevoId = clientesList.size() + 1;
                            Usuario::RegistrarUsuario("clientes.txt", nuevoId, nombre, correoN, passwordN, "cliente");
                        }
                    } while (opcionAdmin != 3);

                    adminDeSesion.logout(adminLogueado->getId());
                }
                else
                {
                    cout << "Error: usuario o password incorrectos.\n";
                    cin.ignore(); cin.get();
                }
            }
            else if (tipoUsuario == 2)
            {
                int clienteId = cliente.IniciarSesionCliente(adminDeSesion);
                if (clienteId != -1)
                {
                    auto clienteLogueado = adminDeSesion.getSesionUsuario(clienteId);
                    menu.MostrarMenu(clienteLogueado);
                    adminDeSesion.logout(clienteId);
                }
                else
                {
                    cout << "Error: usuario o password incorrectos.\n";
                    cin.ignore(); cin.get();
                }
            }
        }

    } while (opcion != 2);

    return 0;
}
