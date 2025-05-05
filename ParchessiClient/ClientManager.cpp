#include "ClientManager.h"

bool ClientManager::AddClient(Client* _newClient)
{
    clients.push_back(_newClient);    
    std::cout << "Adding client with port: " << _newClient->GetSocket()->getRemotePort() << std::endl;
    return false;
}
