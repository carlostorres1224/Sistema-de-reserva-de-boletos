#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <sstream>
#include "Vuelo.h"
#include "AdministradorDeSesion.h"

class Reservacion
{
private:
    int idReservacion;
    int idUsuario;
    int idVuelo;
    int asiento;
    bool pagado; // true = pagado, false = reservado

public:
    Reservacion() : idReservacion(0), idUsuario(0), idVuelo(0), asiento(0), pagado(false) {}

    Reservacion(int idR, int idU, int idV, int a, bool p)
        : idReservacion(idR), idUsuario(idU), idVuelo(idV), asiento(a), pagado(p) {
    }

    // Crear reservación
    bool ReservarVuelo(AdministradorDeSesion& adminDeSesion, int IDUsuario);

    // Guardar reservación en archivo
    bool GuardarReservacion();

    // Mostrar reservaciones del usuario
    static void MostrarReservacionesUsuario(int IDUsuario);

    // Pagar reservación
    static bool PagarReservacion(int idUsuario, int idReservacion);
};
