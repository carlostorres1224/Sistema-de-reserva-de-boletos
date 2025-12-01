#pragma once
#include "Usuario.h"
#include <iostream>

class Menu
{
public:
    void MostrarMenu(std::shared_ptr<Usuario> cliente)
    {
        int opcion;
        do {
            system("cls");
            std::cout << "---- MENU DE VUELOS ----\n";
            std::cout << "1 - Reservar vuelo\n";
            std::cout << "2 - Pagar boleto\n";
            std::cout << "3 - Salir\n";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1: std::cout << "Reservando vuelo...\n"; break;
            case 2: std::cout << "Pagando boleto...\n"; break;
            case 3: std::cout << "Saliendo...\n"; break;
            default: std::cout << "Opción inválida.\n"; break;
            }
            std::cin.ignore(); std::cin.get();
        } while (opcion != 3);
    }
};
