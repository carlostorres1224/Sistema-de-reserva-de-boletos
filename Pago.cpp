#include "Pago.h"

bool Pago::PagarReservacion(const std::string& numeroTarjeta)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Inicializar Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup falló con error: " << iResult << std::endl;
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolver la dirección del servidor
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo falló: " << iResult << std::endl;
        WSACleanup();
        return false;
    }

    // Intentar conectarse
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            continue;
        }

        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "No se pudo conectar al servidor." << std::endl;
        WSACleanup();
        return false;
    }

    // Enviar número de tarjeta
    iResult = send(ConnectSocket, numeroTarjeta.c_str(), (int)numeroTarjeta.size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Error enviando datos: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }

    // Recibir respuesta
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        std::string respuesta(recvbuf);

        if (respuesta == "APROBADO") {
            std::cout << "Pago aprobado por el servidor.\n";
            closesocket(ConnectSocket);
            WSACleanup();
            return true;
        }
        else {
            std::cout << "Pago rechazado por el servidor.\n";
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }
    }
    else {
        std::cout << "No se recibió respuesta del servidor.\n";
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }
}
