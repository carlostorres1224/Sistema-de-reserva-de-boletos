#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include "Usuario.h"
#include "Cliente.h"
#include "AdministradorDeSesion.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define SERVER_PORT "27015"
#define SERVER_IP "127.0.0.1"
#define DEFAULT_BUFLEN 512

bool PagarViaServidor(const std::string& datosPago) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, hints;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &result) != 0) {
        WSACleanup();
        return false;
    }

    ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    if (connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    freeaddrinfo(result);

    send(ConnectSocket, datosPago.c_str(), (int)datosPago.size(), 0);

    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN - 1, 0);
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        closesocket(ConnectSocket);
        WSACleanup();
        return std::string(recvbuf) == "PAGO EXITOSO";
    }

    closesocket(ConnectSocket);
    WSACleanup();
    return false;
}

struct Vuelo {
    int numeroVuelo;
    string fecha;
    string hora;
    string origen;
    string destino;
    int numeroAvion;
    bool asientos[10]{ false };

    bool reservarAsiento(int asiento) {
        if (asiento < 1 || asiento > 10) return false;
        if (asientos[asiento - 1]) return false;
        asientos[asiento - 1] = true;
        return true;
    }
};

vector<Vuelo> cargarVuelos(const string& archivo) {
    vector<Vuelo> vuelos;
    ifstream in(archivo);
    if (!in.is_open()) return vuelos;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Vuelo v;
        ss >> v.numeroVuelo >> v.fecha >> v.hora >> v.origen >> v.destino >> v.numeroAvion;
        for (int i = 0; i < 10; i++) v.asientos[i] = false;
        vuelos.push_back(v);
    }
    in.close();
    return vuelos;
}

void guardarVuelos(const string& archivo, const vector<Vuelo>& vuelos) {
    ofstream out(archivo);
    for (const auto& v : vuelos)
        out << v.numeroVuelo << " " << v.fecha << " " << v.hora << " "
        << v.origen << " " << v.destino << " " << v.numeroAvion << "\n";
}

void CrearAdminPorDefecto() {
    ifstream archivo("admin.txt");
    if (!archivo.is_open() || archivo.peek() == ifstream::traits_type::eof()) {
        ofstream salida("admin.txt", ios::app);
        salida << "1 admin1 admin@tec.mx 1234 admin" << endl;
        salida.close();
    }
}

void marcarAsientosOcupados(vector<Vuelo>& vuelos) {
    ifstream in("reservaciones.txt");
    if (!in.is_open()) return;

    int idCliente, numVuelo, asiento, pagado;
    while (in >> idCliente >> numVuelo >> asiento >> pagado) {
        for (auto& v : vuelos) {
            if (v.numeroVuelo == numVuelo) {
                if (asiento >= 1 && asiento <= 10) v.asientos[asiento - 1] = true;
                break;
            }
        }
    }
    in.close();
}

void mostrarAsientos(const Vuelo& vuelo) {
    cout << "Asientos disponibles (O = libre, X = ocupado):\n";
    for (int i = 0; i < 10; i++) {
        cout << "Asiento " << (i + 1) << ": " << (vuelo.asientos[i] ? "X" : "O") << "\n";
    }
}

bool ReservarVueloCliente(int idCliente) {
    vector<Vuelo> vuelos = cargarVuelos("vuelos.txt");
    marcarAsientosOcupados(vuelos);

    cout << "Vuelos disponibles:\n";
    for (const auto& v : vuelos)
        cout << "Vuelo " << v.numeroVuelo
        << "  Fecha: " << v.fecha
        << "  Hora: " << v.hora
        << "  " << v.origen << " -> " << v.destino
        << "  Avion: " << v.numeroAvion << "\n";
    cout << "\n";

    int numVuelo;
    cout << "Ingrese el numero de vuelo a reservar: ";
    cin >> numVuelo;

    Vuelo* vueloSeleccionado = nullptr;
    for (auto& v : vuelos) {
        if (v.numeroVuelo == numVuelo) {
            vueloSeleccionado = &v;
            break;
        }
    }

    if (!vueloSeleccionado) {
        cout << "Vuelo no encontrado.\n";
        system("pause");
        return false;
    }

    int numAsiento;
    do {
        mostrarAsientos(*vueloSeleccionado);
        cout << "Ingrese numero de asiento (1-10): ";
        cin >> numAsiento;

        if (!vueloSeleccionado->reservarAsiento(numAsiento)) {
            cout << "Asiento ocupado o invalido. Por favor elija otro.\n";
        }
        else {
            break;
        }
    } while (true);

    cout << "Desea pagar ahora o despues? (1 = ahora, 2 = despues): ";
    int opcionPago;
    cin >> opcionPago;

    ofstream out("reservaciones.txt", ios::app);
    out << idCliente << " " << vueloSeleccionado->numeroVuelo << " " << numAsiento << " "
        << (opcionPago == 1 ? 1 : 0) << "\n";
    out.close();

    if (opcionPago == 1) {
        string tarjeta;
        cout << "Ingrese numero de tarjeta: ";
        cin >> tarjeta;

        string datosPago = to_string(idCliente) + " " + to_string(vueloSeleccionado->numeroVuelo)
            + " " + to_string(numAsiento) + " " + tarjeta;

        if (PagarViaServidor(datosPago)) {
            cout << "Pago realizado exitosamente!\n";
            system("pause");
        }
        else {
            cout << "Error en el pago. Puede intentar más tarde.\n";
            system("pause");
            vector<string> lineas;
            ifstream in("reservaciones.txt");
            string line;
            while (getline(in, line)) lineas.push_back(line);
            in.close();

            for (auto& l : lineas) {
                stringstream ss(l);
                int userId, vueloNum, asiento, pagado;
                ss >> userId >> vueloNum >> asiento >> pagado;
                if (userId == idCliente && vueloNum == vueloSeleccionado->numeroVuelo && asiento == numAsiento) {
                    l = to_string(userId) + " " + to_string(vueloNum) + " " + to_string(asiento) + " 0";
                    break;
                }
            }

            ofstream out2("reservaciones.txt");
            for (auto& l : lineas) out2 << l << "\n";
        }
    }
    else {
        cout << "Reservacion guardada, puede pagar mas tarde.\n";
        system("pause");
    }

    guardarVuelos("vuelos.txt", vuelos);
    return true;
}

void PagarReservacion(int idCliente) {
    vector<string> lineas;
    ifstream in("reservaciones.txt");
    string line;
    while (getline(in, line)) lineas.push_back(line);
    in.close();

    vector<Vuelo> vuelos = cargarVuelos("vuelos.txt");
    marcarAsientosOcupados(vuelos);

    vector<int> indicesPendientes;
    cout << "Reservaciones pendientes:\n";
    for (size_t i = 0; i < lineas.size(); i++) {
        stringstream ss(lineas[i]);
        int userId, vueloNum, asiento, pagado;
        ss >> userId >> vueloNum >> asiento >> pagado;
        if (userId == idCliente && pagado == 0) {
            for (const auto& v : vuelos) {
                if (v.numeroVuelo == vueloNum) {
                    cout << indicesPendientes.size() + 1 << " - Vuelo " << v.numeroVuelo
                        << "  Fecha: " << v.fecha
                        << "  Hora: " << v.hora
                        << "  " << v.origen << " -> " << v.destino
                        << "  Avion: " << v.numeroAvion
                        << "  Asiento: " << asiento << "\n";
                    break;
                }
            }
            indicesPendientes.push_back(i);
        }
    }

    if (indicesPendientes.empty()) {
        cout << "No hay reservaciones pendientes.\n";
        return;
    }

    int opcion;
    cout << "Ingrese el numero de reservación a pagar: ";
    cin >> opcion;

    if (opcion < 1 || opcion >(int)indicesPendientes.size()) {
        cout << "Opcion invalida.\n";
        system("pause");
        return;
    }

    int idx = indicesPendientes[opcion - 1];
    stringstream ss(lineas[idx]);
    int userId, vueloNum, asiento, pagado;
    ss >> userId >> vueloNum >> asiento >> pagado;

    string tarjeta;
    cout << "Ingrese numero de tarjeta: ";
    cin >> tarjeta;

    string datosPago = to_string(userId) + " " + to_string(vueloNum) + " " + to_string(asiento) + " " + tarjeta;

    if (PagarViaServidor(datosPago)) {
        cout << "Pago exitoso!\n";
        system("pause");
        lineas[idx] = to_string(userId) + " " + to_string(vueloNum) + " " + to_string(asiento) + " 1";
    }
    else {
        cout << "Error en el pago. Intente mas tarde.\n";
        system("pause");
    }

    ofstream out("reservaciones.txt");
    for (auto& l : lineas) out << l << "\n";
}
int main() {
    CrearAdminPorDefecto();
    AdministradorDeSesion adminDeSesion;

    int opcion;
    do {
        system("cls");
        cout << "1 - Iniciar sesion\n2 - Salir\nAdmin predefinido: admin@tec.mx 1234\n";
        cin >> opcion;

        if (opcion == 1) {
            int tipoUsuario;
            cout << "1 - Administrador\n2 - Cliente\n";
            cin >> tipoUsuario;

            if (tipoUsuario == 1) {
                string correo, password;
                cout << "Correo: "; cin >> correo;
                cout << "Password: "; cin >> password;

                auto admins = Usuario::ObtenerUsuarios("admin.txt");
                if (adminDeSesion.login(correo, password, admins)) {
                    auto adminLogueado = adminDeSesion.getSesionUsuario(admins.back()->getId());

                    int opcionAdmin;
                    do {
                        system("cls");
                        cout << "1 - Registrar administrador\n2 - Registrar cliente\n3 - Crear vuelo\n4 - Cerrar sesión\n";
                        cin >> opcionAdmin;

                        if (opcionAdmin == 1) {
                            string nombre, correoN, passwordN;
                            cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                            cout << "Correo: "; cin >> correoN;
                            cout << "Password: "; cin >> passwordN;
                            size_t nuevoId = admins.size() + 1;
                            Usuario::RegistrarUsuario("admin.txt", nuevoId, nombre, correoN, passwordN, "admin");
                        }
                        else if (opcionAdmin == 2) {
                            string nombre, correoN, passwordN;
                            cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                            cout << "Correo: "; cin >> correoN;
                            cout << "Password: "; cin >> passwordN;
                            auto clientesList = Usuario::ObtenerUsuarios("clientes.txt");
                            size_t nuevoId = clientesList.size() + 1;
                            Usuario::RegistrarUsuario("clientes.txt", nuevoId, nombre, correoN, passwordN, "cliente");
                        }
                        else if (opcionAdmin == 3) {
                            int numVuelo, numAvion;
                            string fecha, hora, origen, destino;
                            cout << "Número de vuelo: "; cin >> numVuelo;
                            cout << "Número de avion: "; cin >> numAvion;
                            cout << "Fecha: "; cin >> fecha;
                            cout << "Hora: "; cin >> hora;
                            cout << "Origen: "; cin >> origen;
                            cout << "Destino: "; cin >> destino;

                            vector<Vuelo> vuelos = cargarVuelos("vuelos.txt");
                            Vuelo nuevo = { numVuelo, fecha, hora, origen, destino, numAvion };
                            vuelos.push_back(nuevo);
                            guardarVuelos("vuelos.txt", vuelos);
                            cout << "Vuelo creado!\n";
                            system("pause");
                            cin.ignore(); cin.get();
                        }
                    } while (opcionAdmin != 4);

                    adminDeSesion.logout(adminLogueado->getId());
                }
                else {
                    cout << "Error: usuario o password incorrectos.\n";
                    system("pause");
                    cin.ignore(); cin.get();
                }
            }
            else if (tipoUsuario == 2) {
                auto clientes = Usuario::ObtenerUsuarios("clientes.txt");
                string correo, password;
                cout << "Correo: "; cin >> correo;
                cout << "Password: "; cin >> password;

                bool logueado = false;
                size_t idCliente = 0;
                for (auto& c : clientes) {
                    if (c->ValidarInicioSesion(correo, password)) {
                        logueado = true;
                        idCliente = c->getId();
                        adminDeSesion.login(correo, password, clientes);
                        break;
                    }
                }

                if (logueado) {
                    int opcionCliente;
                    do {
                        system("cls");
                        cout << "1 - Reservar vuelo\n2 - Pagar boleto\n3 - Salir\n";
                        cin >> opcionCliente;

                        if (opcionCliente == 1) ReservarVueloCliente(idCliente);
                        else if (opcionCliente == 2) PagarReservacion(idCliente);

                    } while (opcionCliente != 3);

                    adminDeSesion.logout(idCliente);
                }
                else {
                    cout << "Error: usuario o password incorrectos.\n";
                    system("pause");
                    cin.ignore(); cin.get();
                }
            }
        }
    } while (opcion != 2);

    return 0;
}
