#include <iostream>
#include "Cliente.h"
#include "AdministradorDeSesion.h"
#include "Menu.h"
#include "Pago.h"

using namespace std;

int main()
{
    AdministradorDeSesion adminDeSesion; // Maneja sesiones activas
    Cliente cliente;
    int opcionPrincipal = 0;

    do
    {
        cout << "\n=== SISTEMA DE RESERVA DE BOLETOS DE AVION ===\n";
        cout << "1 - Iniciar sesión como Cliente\n";
        cout << "2 - Registrar Cliente\n";
        cout << "3 - Salir\n";
        cout << "Ingrese una opción: ";
        cin >> opcionPrincipal;

        switch (opcionPrincipal)
        {
        case 1:
        {
            // Login Cliente
            int IDUsuario = cliente.IniciarSesionCliente(adminDeSesion);

            if (IDUsuario)
            {
                int opcionMenu = 0;
                do
                {
                    cout << "\n=== MENU CLIENTE ===\n";
                    cout << "1 - Reservar vuelo\n";
                    cout << "2 - Pagar boleto\n";
                    cout << "3 - Ver mis reservaciones\n";
                    cout << "4 - Cerrar sesión\n";
                    cout << "Ingrese una opción: ";
                    cin >> opcionMenu;

                    switch (opcionMenu)
                    {
                    case 1:
                        cliente.ReservarVuelo(adminDeSesion, IDUsuario);
                        break;
                    case 2:
                    {
                        Pago pago;
                        string numeroTarjeta;
                        cout << "Ingrese número de tarjeta: ";
                        cin >> numeroTarjeta;

                        bool exito = pago.PagarReservacion(numeroTarjeta);
                        if (exito)
                            cout << "Pago realizado con éxito.\n";
                        else
                            cout << "Pago rechazado.\n";

                        break;
                    }
                    case 3:
                        cliente.MostrarReservaciones(adminDeSesion);
                        break;
                    case 4:
                        cout << "Cerrando sesión...\n";
                        adminDeSesion.Logout(IDUsuario);
                        break;
                    default:
                        cout << "Opción inválida.\n";
                    }
                } while (opcionMenu != 4);
            }
            else
            {
                cout << "Error al iniciar sesión.\n";
            }

            break;
        }
        case 2:
        {
            string nombre, correo, password;
            cout << "Ingrese nombre: ";
            cin >> nombre;
            cout << "Ingrese correo: ";
            cin >> correo;
            cout << "Ingrese password: ";
            cin >> password;

            // Registro de cliente
            if (cliente.Registro(nombre, correo, password))
                cout << "Cliente registrado con éxito.\n";
            else
                cout << "Error al registrar cliente.\n";

            break;
        }
        case 3:
            cout << "Saliendo del sistema...\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }

    } while (opcionPrincipal != 3);

    return 0;
}
