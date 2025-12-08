#include "Vuelo.h"

// Guardar vuelo en archivo vuelos.txt
bool Vuelo::GuardarVuelo()
{
    std::ofstream outFile("vuelos.txt", std::ios::app);
    if (!outFile.is_open())
    {
        std::cerr << "No se pudo abrir el archivo vuelos.txt" << std::endl;
        return false;
    }

    outFile << idVuelo << " " << fecha << " " << hora << " "
        << origen << " " << destino << " "
        << numeroVuelo << " " << costo << " ";

    // Guardar asientos separados por coma
    for (size_t i = 0; i < asientos.size(); ++i)
    {
        outFile << asientos[i];
        if (i != asientos.size() - 1)
            outFile << ",";
    }

    outFile << std::endl;
    outFile.close();
    return true;
}

// Leer todos los vuelos
std::vector<std::shared_ptr<Vuelo>> Vuelo::ObtenerVuelos()
{
    std::vector<std::shared_ptr<Vuelo>> vuelos;
    std::ifstream inFile("vuelos.txt");
    if (!inFile.is_open())
    {
        std::cerr << "No se pudo abrir vuelos.txt" << std::endl;
        return vuelos;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream ss(line);
        int id, numVuelo;
        std::string f, h, o, d;
        double c;
        std::string asientosStr;

        ss >> id >> f >> h >> o >> d >> numVuelo >> c >> asientosStr;

        std::shared_ptr<Vuelo> vuelo = std::make_shared<Vuelo>(id, f, h, o, d, numVuelo, c);

        // Parsear asientos
        std::vector<int> asientosVec;
        std::stringstream sa(asientosStr);
        std::string asiento;
        while (std::getline(sa, asiento, ','))
            asientosVec.push_back(std::stoi(asiento));

        vuelo->getAsientos() = asientosVec;
        vuelos.push_back(vuelo);
    }

    inFile.close();
    return vuelos;
}

// Imprimir asientos en consola
void Vuelo::ImprimirAsientos() const
{
    std::cout << "Asientos: (0 = libre, 1 = ocupado)\n";
    for (size_t i = 0; i < asientos.size(); ++i)
    {
        std::cout << "[" << asientos[i] << "]";
        if ((i + 1) % 5 == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}
