#include "Table.h"
#include <fstream>
#include <iostream>

// Asegúrate de que "nlohmann/json.hpp" está incluido en algún lugar de tu proyecto
// En este ejemplo se asume que ya está incluido en Table.h o en otro header global

void Table::Init()
{

    Token* token = new Token(1);

    _tokens.push_back(token);

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
                // Buscar la propiedad "id" dentro de "properties"
                if (obj.contains("properties") && obj["properties"].is_array()) {
                    for (auto& prop : obj["properties"]) {
                        if (prop.contains("name") && prop["name"] == "id") {
                            cellId = prop["value"].get<int>();
                            break;
                        }
                    }
                }
                // Extraer las coordenadas x e y
                float x = obj["x"].get<float>();
                float y = obj["y"].get<float>();

                // Crear una nueva Cell y agregarla al vector de _cells
                if (_cells.find(cellId) != _cells.end()) {
                    std::cout << "Repe" << std::endl;
                    continue; // Salta esta celda si ya fue insertada
                }

                _cells.emplace(cellId, new Cell(cellId, sf::Vector2f(x, y)));
            }
        }
    }
    std::cout << "Total de celdas cargadas: " << _cells.size() << std::endl;
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

void Table::Update()
{
    for (Token* token : _tokens)
    {
        token->SetPosition(GetCell(token->GetIdPosition())->GetPosition());
    }
}


