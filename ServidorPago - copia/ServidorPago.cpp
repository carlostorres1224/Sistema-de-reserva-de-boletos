#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;

    std::cout << "=== SERVIDOR DE PAGO INICIADO ===\n";

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Error al iniciar Winsock.\n";
        return 1;
    }

    // Crear socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "Error creando socket.\n";
        WSACleanup();
        return 1;
    }

    // Datos del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);   // mismo puerto que usa el cliente
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Enlazar socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Error en bind.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Escuchar conexiones
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Error en listen.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Servidor escuchando en puerto 8080...\n";

    int clientSize = sizeof(clientAddr);

    while (true)
    {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cout << "Error en accept.\n";
            continue;
        }

        // Recibir número de tarjeta
        char buffer[256];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Error recibiendo datos.\n";
            closesocket(clientSocket);
            continue;
        }

        buffer[bytesReceived] = '\0';
        std::string tarjeta(buffer);

        std::cout << "Tarjeta recibida: " << tarjeta << "\n";

        // Lógica simple: si empieza con '4' lo aprobamos
        if (!tarjeta.empty() && tarjeta[0] == '4')
        {
            send(clientSocket, "APROBADO", 8, 0);
            std::cout << "Pago aprobado.\n";
        }
        else
        {
            send(clientSocket, "RECHAZADO", 9, 0);
            std::cout << "Pago rechazado.\n";
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
