#include "ClientManager.h"

bool ClientManager::AddClient(Client* _newClient)
{
    clients.push_back(_newClient);
    return false;
}
