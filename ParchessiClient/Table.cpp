#include "Table.h"
#include <fstream>
#include <iostream>

// Aseg�rate de que "nlohmann/json.hpp" est� incluido en alg�n lugar de tu proyecto
// En este ejemplo se asume que ya est� incluido en Table.h o en otro header global

// HECHO CON CHATGPT

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
                    std::cout << "Celda Repetida" << std::endl;
                    continue; // Salta esta celda si ya fue insertada
                }

                _cells.emplace(cellId, new Cell(cellId, sf::Vector2f(x, y), isHorizontal, isFinalCell));
            }
        }
    }


}
// Inicializa las fichas en su base

void Table::InitTokens(IGameStateProvider* provider)
{
    int count = 0;
    // Crear 4 fichas para cada uno de los 4 jugadores
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            count++;
            Token* token = new Token(i, 2, count, provider);

            // Colocar la ficha en su celda base (1000 + playerId)
            token->UpdateIdPosition(1000 + token->GetPlayerId());
            GetCell(token->GetIdPosition())->AddToken(token);

            // Guardar ficha en lista general
            _tokens.push_back(token);
        }
    }
}

// Dibuja todas las fichas en pantalla
void Table::Draw(sf::RenderWindow& window)
{
    for (Token* token : _tokens)
    {
        token->Draw(window);
    }
}

// Devuelve el puntero a una celda a partir de su ID
Cell* Table::GetCell(int id)
{
    Cell* value = _cells[id];
    return value;
}

// Actualiza la posición de todas las fichas que están en movimiento
void Table::UpdatePositions(int newPos)
{
    int _newPosition = newPos;

    for (Token* token : _tokens)
    {
        // Solo mover si la ficha está marcada como "en movimiento"
        if (token->GetIsMoving())
        {
            // Si viene de la base (id >= 1000), salta a la casilla de salida
            if (newPos >= 1000)
            {
                _newPosition = token->GetExitCell();
                newPos = 0;
            }

            // Quitar ficha de la celda actual
            GetCell(token->GetIdPosition())->RemoveToken(token);

            // Recorrer desde la celda actual hasta la nueva posición
            for (int i = token->GetIdPosition(); i <= newPos; i++)
            {
                // Si supera su zona final privada, lo dejamos en la última celda de su recorrido
                if (i >= (token->GetPlayerId() * 100) + 7)
                {
                    _newPosition = (token->GetPlayerId() * 100) + 7;
                    break;
                }
                // Si llega a su celda final del recorrido general, entra en la zona final
                if (i == token->GetFinalCellId())
                {
                    token->ArriveLastZone();
                    _newPosition = token->GetPlayerId() * 100;

                    // Continuar el movimiento dentro de la zona final
                    int newLoop = (newPos - i) + _newPosition;
                    for (int j = _newPosition; j <= newLoop; j++)
                    {
                        newPos = j;
                        _newPosition = j;
                    }
                    break;
                }

                // Si se pasa de la última casilla del recorrido general
                if (_newPosition > 68 && !token->GetIsLastZone())
                {
                    _newPosition = 1;
                    int newLoop = newPos - 68;

                    // Avanzar desde el inicio, evitando casillas llenas
                    for (int j = _newPosition; j <= newLoop; j++)
                    {
                        if (GetCell(j)->GetTokens().size() == 2)
                        {
                            _newPosition = j - 1;
                            continue;
                        }
                        else
                        {
                            newPos = j;
                            _newPosition = j;
                        }
                    }
                }
                else if (_newPosition < 1)
                {
                    _newPosition = 68;
                }

                // Si una casilla está llena (2 fichas), parar ahí
                if (GetCell(i)->GetTokens().size() == 2)
                {
                    _newPosition = i - 1;
                    continue;
                }
            }

            // Si hay otra ficha en la nueva posición
            if (GetCell(_newPosition)->GetTokens().size() != 0)
            {
                // Si es del rival, se la come y la manda a su base
                if (GetCell(_newPosition)->GetTokens()[0]->GetPlayerId() != token->GetPlayerId())
                {
                    Token* rivalToken = GetCell(_newPosition)->GetTokens()[0];
                    rivalToken->SetPosition(
                        GetCell(1000 + rivalToken->GetPlayerId())->GetPosition(),
                        1000 + rivalToken->GetPlayerId()
                    );
                    GetCell(1000 + rivalToken->GetPlayerId())->AddToken(rivalToken);

                    rivalToken->SetSelectable(false);
                    rivalToken->SetIsInBase(true);
                    GetCell(_newPosition)->RemoveToken(rivalToken);

                    // Después de comerse una ficha, avanza 20 más
                    token->UpdateIdPosition(_newPosition);
                    UpdatePositions(token->Move(20));
                    return;
                }
            }

            // Colocar ficha en su nueva celda
            token->UpdateIdPosition(_newPosition);
            GetCell(token->GetIdPosition())->AddToken(token);
            token->EndMove();
        }
    }
}
