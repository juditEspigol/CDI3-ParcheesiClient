#include "Table.h"
#include <fstream>
#include <iostream>

// Aseg�rate de que "nlohmann/json.hpp" est� incluido en alg�n lugar de tu proyecto
// En este ejemplo se asume que ya est� incluido en Table.h o en otro header global

Table::Table()
{
    // Abrir el archivo JSON exportado desde Tiled
    std::ifstream file("../Assets/Tiled/ParchisMap.json");
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir ParchisMap.json" << std::endl;
        return;
    }

    // Parsear el JSON usando nlohmann::json
    nlohmann::json j;

    try {
        file >> j;
    }
    catch (const std::exception& e) {
        std::cerr << "Error al parsear JSON: " << e.what() << std::endl;
        return;
    }

    // Verificar que el JSON contiene la clave "layers"
    if (!j.contains("layers") || !j["layers"].is_array()) {
        std::cerr << "El archivo JSON no contiene una secci�n 'layers' v�lida." << std::endl;
        return;
    }

    // Recorrer las capas y buscar la que tenga el nombre "Cells"
    for (auto& layer : j["layers"]) {
        if (layer.contains("name") && layer["name"] == "Cells") {
            // Verificar que la capa tenga un array "objects"
            if (!layer.contains("objects") || !layer["objects"].is_array()) {
                std::cerr << "La capa 'Cells' no contiene objetos v�lidos." << std::endl;
                continue;
            }

            // Recorrer cada objeto de la capa "Cells"
            for (auto& obj : layer["objects"]) {
                int cellId = 0;
                bool isHorizontal = false;
                bool isFinalCell = false;

                // Buscar las propiedades "id" y "horizontal" dentro de "properties"
                if (obj.contains("properties") && obj["properties"].is_array()) {
                    for (auto& prop : obj["properties"]) {
                        if (prop.contains("name")) {
                            if (prop["name"] == "id") {
                                cellId = prop["value"].get<int>();
                            }
                            else if (prop["name"] == "horizontal") {
                                isHorizontal = prop["value"].get<bool>();
                            }
                            if (prop["name"] == "finalCell") {
								isFinalCell = prop["value"].get<bool>();
                            }
                        }
                    }
                }

                // Extraer las coordenadas x e y
                float x = obj["x"].get<float>();
                float y = obj["y"].get<float>();

                // Crear una nueva Cell y agregarla al mapa _cells si no existe a�n
                if (_cells.find(cellId) != _cells.end()) {
                    std::cout << "Repe" << std::endl;
                    continue; // Salta esta celda si ya fue insertada
                }

                _cells.emplace(cellId, new Cell(cellId, sf::Vector2f(x, y), isHorizontal, isFinalCell));
            }
        }
    }


}

void Table::InitTokens(IGameStateProvider* provider)
{
    std::cout << "Total de celdas cargadas: " << _cells.size() << std::endl;

    int count = 0;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            count++;
            Token* token = new Token(i, 2, count, provider);
            token->UpdateIdPosition(1000 + token->GetPlayerId());
            GetCell(token->GetIdPosition())->AddToken(token);
            _tokens.push_back(token);
        }
    }
}

void Table::Draw(sf::RenderWindow& window)
{
    for (Token* token : _tokens)
    {
        token->Draw(window);
    }
}

Cell* Table::GetCell(int id)
{
    Cell* value = _cells[id];
    return value;
}

void Table::UpdatePositions(int newPos)
{
    int _newPosition = newPos;
    std::cout << "[DEBUG] Iniciando UpdatePositions con newPos = " << newPos << std::endl;

    for (Token* token : _tokens)
    {
        std::cout << "[DEBUG] Token con id " << token->GetTokenId() << " IS MOVING: " << token->GetIsMoving() << std::endl;
        if (token->GetIsMoving())
        {
            if (newPos >= 1000)
            {
                _newPosition = token->GetExitCell();
                newPos = 0;
            }

            std::cout << "[DEBUG] Moviendo token ID " << token->GetTokenId()
                << " del jugador " << token->GetPlayerId()
                << " desde casilla " << token->GetIdPosition() << std::endl;

            GetCell(token->GetIdPosition())->RemoveToken(token);

            for (int i = token->GetIdPosition(); i <= newPos; i++)
            {
                std::cout << "[DEBUG] Recorriendo casilla " << i << std::endl;

                if (i >= (token->GetPlayerId() * 100) + 7)
                {
                    std::cout << "[DEBUG] Llegamos al límite del recorrido especial, forzando _newPosition a "
                        << (token->GetPlayerId() * 100) + 7 << std::endl;
                    _newPosition = (token->GetPlayerId() * 100) + 7;
                    break;
                }

                if (i == token->GetFinalCellId())
                {
                    std::cout << "[DEBUG] Entrando en zona final desde casilla " << i << std::endl;
                    token->ArriveLastZone();
                    _newPosition = token->GetPlayerId() * 100;

                    int newLoop = (newPos - i) + _newPosition;
                    std::cout << "[DEBUG] newLoop = " << newLoop << std::endl;

                    for (int j = _newPosition; j <= newLoop; j++)
                    {
                        newPos = j;
                        _newPosition = j;
                        std::cout << "[DEBUG] Avanzando por zona final: nueva pos = " << _newPosition << std::endl;
                    }
                    break;
                }

                if (_newPosition > 68 && !token->GetIsLastZone())
                {
                    std::cout << "[DEBUG] Se pasó de 68 sin estar en zona final. Reiniciando a 1." << std::endl;
                    _newPosition = 1;
                    int newLoop = newPos - 68;
                    std::cout << "[DEBUG] newLoop = " << newLoop << std::endl;

                    for (int j = _newPosition; j <= newLoop; j++)
                    {
                        std::cout << "[DEBUG] Posible celda en rebote: " << j << std::endl;
                        if (GetCell(j)->GetTokens().size() == 2)
                        {
                            std::cout << "[DEBUG] Celda " << j << " ocupada por 2 tokens, retrocedemos." << std::endl;
                            _newPosition = j - 1;
                            continue;
                        }
                        else
                        {
                            newPos = j;
                            _newPosition = j;
                            std::cout << "[DEBUG] Avanzamos a celda " << j << std::endl;
                        }
                    }
                }
                else if (_newPosition < 1)
                {
                    std::cout << "[DEBUG] newPosition < 1. Lo fijamos a 68." << std::endl;
                    _newPosition = 68;
                }

                if (GetCell(i)->GetTokens().size() == 2)
                {
                    std::cout << "[DEBUG] Celda " << i << " tiene 2 tokens, detenemos avance." << std::endl;
                    _newPosition = i - 1;
                    continue;
                }
            }

            std::cout << "[DEBUG] Final de recorrido. Posición destino tentativa: " << _newPosition << std::endl;

            if (GetCell(_newPosition)->GetTokens().size() != 0)
            {
                std::cout << "[DEBUG] Celda " << _newPosition << " contiene "
                    << GetCell(_newPosition)->GetTokens().size() << " tokens." << std::endl;

                if (GetCell(_newPosition)->GetTokens()[0]->GetPlayerId() != token->GetPlayerId())
                {
                    std::cout << "[DEBUG] Se encontró un token rival. Iniciando captura." << std::endl;

                    Token* rivalToken = GetCell(_newPosition)->GetTokens()[0];
                    rivalToken->SetPosition(
                        GetCell(1000 + rivalToken->GetPlayerId())->GetPosition(),
                        1000 + rivalToken->GetPlayerId()
                    );
                    GetCell(1000 + rivalToken->GetPlayerId())->AddToken(rivalToken);

                    rivalToken->SetSelectable(false);
                    rivalToken->SetIsInBase(true);
                    GetCell(_newPosition)->RemoveToken(rivalToken);

                    std::cout << "[DEBUG] Rival enviado a base. Relanzando movimiento +20." << std::endl;
                    token->UpdateIdPosition(_newPosition);
                    UpdatePositions(token->Move(20));
                    return;
                }
            }

            std::cout << "[DEBUG] Posicionando token ID " << token->GetTokenId()
                << " en celda " << _newPosition << std::endl;

            token->UpdateIdPosition(_newPosition);
            GetCell(token->GetIdPosition())->AddToken(token);
            token->EndMove();
        }
    }

    std::cout << "[DEBUG] Fin de UpdatePositions" << std::endl;
}
