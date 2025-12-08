#include "Menu.h"
#include <iostream>
using namespace std;

void Menu::MostrarMenu(AdministradorDeSesion& admin, int IDUsuario)
{
    Cliente cliente;
    int opcion;
    do
    {
        cout << "\n--- MENU CLIENTE ---\n";
        cout << "1 - Reservar vuelo\n";
        cout << "2 - Ver mis reservaciones\n";
        cout << "3 - Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            cliente.ReservarVuelo(admin, IDUsuario);
            break;
        case 2:
            cliente.MostrarReservaciones(admin, IDUsuario); // ← CORREGIDO
            break;
        case 3:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opcion invalida.\n";
            break;
        }
    } while (opcion != 3);
}
