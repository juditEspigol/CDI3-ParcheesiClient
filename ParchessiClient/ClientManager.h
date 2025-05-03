#pragma once
#include "Client.h"

#define CLIENT_MANAGER ClientManager::Instance()
#define MAX_CLIENTS 2

class ClientManager
{
private:

    unsigned int selfID;
    std::vector<Client*> clients{};

    ClientManager() = default;
    ClientManager(const ClientManager&) = delete;
    ClientManager& operator =(const ClientManager&) = delete;

public:

    inline static ClientManager& Instance()
    {
        static ClientManager manager;
        return manager;
    }

    bool AddClient(Client* _newClient);
    inline std::vector<Client*> GetClients() const { return clients; }
    inline int GetSizeClients() const { return clients.size(); }
    inline void SetSelfID(const unsigned int _selfID) { selfID = _selfID; }
};