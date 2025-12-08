#pragma once
#include "Usuario.h"
#include "AdministradorDeSesion.h"
#include <iostream>
#include <vector>
#include <memory>

class Cliente : public Usuario
{
public:
    Cliente() {}
    Cliente(size_t _id, const std::string& _nombre, const std::string& _correo,
        const std::string& _password, const std::string& _rol)
        : Usuario(_id, _nombre, _correo, _password, _rol) {
    }

    int IniciarSesionCliente(AdministradorDeSesion& adminDeSesion)
    {
        system("cls");
        std::string correoLogin, passwordLogin;
        std::cout << "Correo: "; std::cin >> correoLogin;
        std::cout << "Password: "; std::cin >> passwordLogin;

        auto clientes = Usuario::ObtenerUsuarios("clientes.txt");

        for (auto& cliente : clientes)
        {
            if (cliente->ValidarInicioSesion(correoLogin, passwordLogin))
            {
                adminDeSesion.login(correoLogin, passwordLogin, clientes);
                return static_cast<int>(cliente->getId());
            }
        }
        return -1;
    }
};