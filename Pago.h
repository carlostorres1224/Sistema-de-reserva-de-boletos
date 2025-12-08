#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "8080"
#define DEFAULT_BUFLEN 512

class Pago
{
public:
    // Envía el número de tarjeta al servidor de pagos y retorna true si fue aprobado
    bool PagarReservacion(const std::string& numeroTarjeta);
};
