#pragma once
#include "Client.h"

#define MAX_CLIENTS 1

#define CLIENT_MANAGER ClientManager::Instance()

class ClientManager
{
private:

    unsigned int selfID = 0;
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
    inline int GetSelfID() const { return selfID; }
};