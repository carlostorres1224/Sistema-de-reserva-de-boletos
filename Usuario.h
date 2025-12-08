#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

class Usuario
{
protected:
    size_t id;  
    std::string nombre;
    std::string correo;
    std::string password;
    std::string rol;

public:
    Usuario() : id(0) {}
    Usuario(size_t _id, const std::string& _nombre, const std::string& _correo,
        const std::string& _password, const std::string& _rol)
        : id(_id), nombre(_nombre), correo(_correo), password(_password), rol(_rol) {
    }

    size_t getId() const { return id; }
    std::string getCorreo() const { return correo; }
    std::string getRol() const { return rol; }
    std::string getNombre() const { return nombre; }

    bool ValidarInicioSesion(const std::string& email, const std::string& pass) const
    {
        return correo == email && password == pass;
    }

    static std::vector<std::shared_ptr<Usuario>> ObtenerUsuarios(const std::string& archivo)
    {
        std::vector<std::shared_ptr<Usuario>> usuarios;
        std::ifstream inputFile(archivo);
        if (!inputFile.is_open()) return usuarios;

        std::string line;
        while (std::getline(inputFile, line))
        {
            if (line.empty()) continue;

            std::stringstream ss(line);
            size_t _id;
            std::string _nombre, _correo, _password, _rol;

            ss >> _id;
            ss.ignore(); 
            ss >> _nombre >> _correo >> _password >> _rol;

            usuarios.push_back(std::make_shared<Usuario>(_id, _nombre, _correo, _password, _rol));
        }

        inputFile.close();
        return usuarios;
    }

    static bool RegistrarUsuario(const std::string& archivo, size_t _id, const std::string& _nombre,
        const std::string& _correo, const std::string& _password, const std::string& _rol)
    {
        std::ofstream outputFile(archivo, std::ios::app);
        if (!outputFile.is_open()) return false;

        outputFile << _id << " " << _nombre << " " << _correo << " " << _password << " " << _rol << std::endl;
        outputFile.close();
        return true;
    }
};