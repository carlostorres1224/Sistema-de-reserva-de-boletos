#include "Cliente.h"
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

int Cliente::IniciarSesionCliente(AdministradorDeSesion& adminDeSesion)
{
    string correo, password;
    int IDUsuario = 0;

    system("cls");

    cout << "Usuario: ";
    cin >> correo;

    cout << "Password: ";
    cin >> password;

    vector<shared_ptr<Usuario>> usuarios; // Aquí deberías cargar desde archivo

    for (const auto& usuario : usuarios)
    {
        if (usuario->ValidarID(correo, password))
        {
            IDUsuario = usuario->getID();
            adminDeSesion.ActivarSesion(IDUsuario);
            cout << "Sesion iniciada con exito!\n";
            break;
        }
    }
    return IDUsuario;
}

bool Cliente::ReservarVuelo(AdministradorDeSesion& adminDeSesion, int IDUsuario)
{
    if (adminDeSesion.IsLoggedIn(IDUsuario))
    {
        cout << "Sesión activa\n";
        Reservacion reservacion;
        reservacion.ReservarVuelo(adminDeSesion, IDUsuario);
        return true;
    }
    return false;
}

bool Cliente::MostrarReservaciones(AdministradorDeSesion& adminDeSesion)
{
    // Implementa lectura del archivo de reservaciones por IDUsuario
    return true;
}
