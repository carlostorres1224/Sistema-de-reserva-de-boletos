#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "Usuario.h"

class AdministradorDeSesion
{
private:
    std::map<size_t, std::shared_ptr<Usuario>> sesionesActivas;

public:
    bool login(const std::string& email, const std::string& password,
        const std::vector<std::shared_ptr<Usuario>>& usuarios)
    {
        for (const auto& usuario : usuarios)
        {
            if (usuario->ValidarInicioSesion(email, password))
            {
                sesionesActivas[usuario->getId()] = usuario;
                return true;
            }
        }
        return false;
    }

    void logout(size_t userId)
    {
        sesionesActivas.erase(userId);
    }

    bool isLoggedIn(size_t userId) const
    {
        return sesionesActivas.find(userId) != sesionesActivas.end();
    }

    std::shared_ptr<Usuario> getSesionUsuario(size_t idUsuario) const
    {
        auto it = sesionesActivas.find(idUsuario);
        if (it != sesionesActivas.end())
        {
            return it->second;
        }
        return nullptr;
    }
};
