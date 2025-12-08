#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

class Vuelo
{
private:
    int idVuelo;
    std::string fecha;
    std::string hora;
    std::string origen;
    std::string destino;
    std::vector<int> asientos; // 0 = libre, 1 = ocupado
    int numeroVuelo;
    double costo;

public:
    Vuelo() : idVuelo(0), fecha(""), hora(""), origen(""), destino(""),
        asientos(10, 0), numeroVuelo(0), costo(0.0) {
    }

    Vuelo(int id, std::string f, std::string h, std::string o, std::string d, int numVuelo, double c)
        : idVuelo(id), fecha(f), hora(h), origen(o), destino(d),
        asientos(10, 0), numeroVuelo(numVuelo), costo(c) {
    }

    int getId() const { return idVuelo; }
    std::string getFecha() const { return fecha; }
    std::string getHora() const { return hora; }
    std::string getOrigen() const { return origen; }
    std::string getDestino() const { return destino; }
    std::vector<int>& getAsientos() { return asientos; }
    int getNumeroVuelo() const { return numeroVuelo; }
    double getCosto() const { return costo; }

    // Guardar vuelo en archivo
    bool GuardarVuelo();

    // Leer todos los vuelos
    static std::vector<std::shared_ptr<Vuelo>> ObtenerVuelos();

    // Imprimir asientos
    void ImprimirAsientos() const;
};
