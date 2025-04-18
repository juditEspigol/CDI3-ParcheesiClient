#include "Table.h"
#include <fstream>
#include <iostream>

// Asegúrate de que "nlohmann/json.hpp" está incluido en algún lugar de tu proyecto
// En este ejemplo se asume que ya está incluido en Table.h o en otro header global





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
        std::cerr << "El archivo JSON no contiene una sección 'layers' válida." << std::endl;
        return;
    }

    // Recorrer las capas y buscar la que tenga el nombre "Cells"
    for (auto& layer : j["layers"]) {
        if (layer.contains("name") && layer["name"] == "Cells") {
            // Verificar que la capa tenga un array "objects"
            if (!layer.contains("objects") || !layer["objects"].is_array()) {
                std::cerr << "La capa 'Cells' no contiene objetos válidos." << std::endl;
                continue;
            }

            // Recorrer cada objeto de la capa "Cells"
            for (auto& obj : layer["objects"]) {
                int cellId = 0;
                bool isHorizontal = false;

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
                        }
                    }
                }

                // Extraer las coordenadas x e y
                float x = obj["x"].get<float>();
                float y = obj["y"].get<float>();

                // Crear una nueva Cell y agregarla al mapa _cells si no existe aún
                if (_cells.find(cellId) != _cells.end()) {
                    std::cout << "Repe" << std::endl;
                    continue; // Salta esta celda si ya fue insertada
                }

                _cells.emplace(cellId, new Cell(cellId, sf::Vector2f(x, y), isHorizontal));
            }
        }
    }

    std::cout << "Total de celdas cargadas: " << _cells.size() << std::endl;

    
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Token* token = new Token(i, 2);
            token->UpdateIdPosition(1000 + token->GetPlayerId());
            GetCell(token->GetIdPosition())->AddToken(token);
            _tokens.push_back(token);
        }
    }
    

}

void Table::Draw(sf::RenderWindow& window)
{
    /*for (const std::pair<int, Cell*> pair : _cells)
    {
        Cell* cell = pair.second;
        if (cell)
        {
            cell->Draw(window);
        }
        else
        {
            std::cout << "No TIENE cell" << std::endl;
        }
    }*/

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

    for (Token* token : _tokens)
    {
        if (token->GetIsMoving())
        {
            //Lo sacamos de la casilla anterior
            GetCell(token->GetIdPosition())->RemoveToken(token);
            //Actualizamos a la nueva casilla

            for (int i = token->GetIdPosition(); i <= newPos; i++)
            {
                if (GetCell(i)->GetTokens().size() == 2)
                {
                    _newPosition = i - 1;
                    continue;
                }
            }

            //Comprobamos si hay otra ficha
            if (GetCell(_newPosition)->GetTokens().size() != 0)
            {
                //Comprobamos si es ficha de otro jugador 
                if (GetCell(_newPosition)->GetTokens()[0]->GetPlayerId() != token->GetPlayerId())
                {
                    //Si es de otra jugador la eliminamos 
                    GetCell(_newPosition)->GetTokens()[0]->SetPosition
                    (GetCell(1000 + GetCell(_newPosition)->GetTokens()[0]->GetPlayerId())->GetPosition(), 1000 + GetCell(newPos)->GetTokens()[0]->GetPlayerId());

                    GetCell(_newPosition)->RemoveToken(GetCell(_newPosition)->GetTokens()[0]);
                }
            }

            token->UpdateIdPosition(_newPosition);

            //Lo metemos en la misma casilla
            GetCell(token->GetIdPosition())->AddToken(token);

            token->EndMove();
        }
    }
}


