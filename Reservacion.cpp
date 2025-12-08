#include "Reservacion.h"

bool Reservacion::ReservarVuelo(AdministradorDeSesion& adminDeSesion, int IDUsuario)
{
    if (!adminDeSesion.IsLoggedIn(IDUsuario))
    {
        std::cout << "Debe iniciar sesión para reservar." << std::endl;
        return false;
    }

    // Mostrar vuelos disponibles
    auto vuelos = Vuelo::ObtenerVuelos();
    if (vuelos.empty())
    {
        std::cout << "No hay vuelos disponibles." << std::endl;
        return false;
    }

    std::cout << "Vuelos disponibles:\n";
    for (size_t i = 0; i < vuelos.size(); ++i)
    {
        std::cout << i + 1 << " - "
            << vuelos[i]->getOrigen() << " -> " << vuelos[i]->getDestino()
            << " Fecha: " << vuelos[i]->getFecha()
            << " Hora: " << vuelos[i]->getHora() << std::endl;
    }

    int opcionVuelo;
    std::cout << "Seleccione el vuelo: ";
    std::cin >> opcionVuelo;
    opcionVuelo -= 1;

    if (opcionVuelo < 0 || opcionVuelo >= (int)vuelos.size())
    {
        std::cout << "Opción inválida." << std::endl;
        return false;
    }

    Vuelo* vueloSeleccionado = vuelos[opcionVuelo].get();
    vueloSeleccionado->ImprimirAsientos();

    int asiento;
    std::cout << "Seleccione un asiento (1-10): ";
    std::cin >> asiento;
    if (asiento < 1 || asiento > 10 || vueloSeleccionado->getAsientos()[asiento - 1] == 1)
    {
        std::cout << "Asiento inválido o ya ocupado." << std::endl;
        return false;
    }

    // Marcar asiento como ocupado
    vueloSeleccionado->getAsientos()[asiento - 1] = 1;
    vueloSeleccionado->GuardarVuelo();

    // Preguntar si desea pagar ahora
    int opPago;
    std::cout << "Desea pagar ahora? 1 = Sí, 2 = No: ";
    std::cin >> opPago;

    bool pagado = (opPago == 1);

    // Crear reservación
    Reservacion res(0, IDUsuario, vueloSeleccionado->getId(), asiento, pagado);
    res.GuardarReservacion();

    std::cout << "Reservación " << (pagado ? "pagada" : "registrada, pendiente de pago") << std::endl;
    return true;
}

bool Reservacion::GuardarReservacion()
{
    std::ofstream outFile("reservaciones.txt", std::ios::app);
    if (!outFile.is_open())
    {
        std::cerr << "No se pudo abrir reservaciones.txt" << std::endl;
        return false;
    }

    outFile << idUsuario << " " << idVuelo << " " << asiento << " " << pagado << std::endl;
    outFile.close();
    return true;
}

void Reservacion::MostrarReservacionesUsuario(int IDUsuario)
{
    std::ifstream inFile("reservaciones.txt");
    if (!inFile.is_open())
    {
        std::cerr << "No se pudo abrir reservaciones.txt" << std::endl;
        return;
    }

    std::string line;
    std::cout << "=== Reservaciones de Usuario ID: " << IDUsuario << " ===\n";
    while (std::getline(inFile, line))
    {
        std::istringstream ss(line);
        int idU, idV, asiento;
        bool pagado;
        ss >> idU >> idV >> asiento >> pagado;

        if (idU == IDUsuario)
        {
            std::cout << "Vuelo ID: " << idV << " Asiento: " << asiento
                << " Estado: " << (pagado ? "Pagado" : "Pendiente") << std::endl;
        }
    }
    inFile.close();
}

bool Reservacion::PagarReservacion(int idUsuario, int idReservacion)
{
    std::ifstream inFile("reservaciones.txt");
    std::vector<std::string> lineas;
    bool encontrado = false;

    if (!inFile.is_open())
    {
        std::cerr << "No se pudo abrir reservaciones.txt" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        lineas.push_back(line);
    }
    inFile.close();

    std::ofstream outFile("reservaciones.txt", std::ios::trunc);
    if (!outFile.is_open())
    {
        std::cerr << "No se pudo abrir reservaciones.txt" << std::endl;
        return false;
    }

    for (auto& l : lineas)
    {
        std::istringstream ss(l);
        int idU, idV, asiento;
        bool pagado;
        ss >> idU >> idV >> asiento >> pagado;

        if (idU == idUsuario && !pagado)
        {
            pagado = true;
            encontrado = true;
        }
        outFile << idU << " " << idV << " " << asiento << " " << pagado << std::endl;
    }

    outFile.close();
    return encontrado;
}
