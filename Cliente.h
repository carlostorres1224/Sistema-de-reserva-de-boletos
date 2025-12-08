#pragma once
#include "Usuario.h"
#include "AdministradorDeSesion.h"
#include "Reservacion.h"

class Cliente : protected Usuario
{
private:
    int permisos;

public:
    Cliente() { permisos = 2; }

    int getPermisos() const { return permisos; }

    int IniciarSesionCliente(AdministradorDeSesion& adminDeSesion);
    bool ReservarVuelo(AdministradorDeSesion& adminDeSesion, int IDUsuario);
    bool MostrarReservaciones(AdministradorDeSesion& adminDeSesion);
};
