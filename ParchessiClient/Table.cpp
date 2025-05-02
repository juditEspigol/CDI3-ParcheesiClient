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

        std::cout << "Total de celdas cargadas: " << _cells.size() << std::endl;

    
        //Creamos 4 tokens por cada jugador
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
        //Llamamos al draw de cada token
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
        //Creamos variable que será la posicion nueva al final
        int _newPosition = newPos;

        //Recorremos todos los tokens para ver cual se mueve
        for (Token* token : _tokens)
        {
            if (token->GetIsMoving())
            {
                // Lo sacamos de la casilla anterior
                GetCell(token->GetIdPosition())->RemoveToken(token);

                // Recorremos desde la posición actual hasta la nueva
                for (int i = token->GetIdPosition(); i <= newPos; i++)
                {
                    if (i >= (token->GetPlayerId() * 100) + 7)
                    {
                        _newPosition = (token->GetPlayerId() * 100) + 7;
                        break;
                    }

                    // Si llegamos a la última celda del recorrido principal hacemos que suba por el color
                    if (i == token->GetFinalCellId())
                    {
                        token->ArriveLastZone();
                        _newPosition = token->GetPlayerId() * 100;

                        int newLoop = (newPos - i) + _newPosition;
                        std::cout << "NewLoop = " << newLoop << std::endl;

                        for (int j = _newPosition; j <= newLoop; j++)
                        {
                            newPos = j;
                            _newPosition = j;                    
                        
                        }
                        break;
                    }

                    //Si llegan a la celda final del tablero -68- Volvemos a contar desde el 1 
                    if (_newPosition > 68 && !token->GetIsLastZone() && token->GetPlayerId() !=4)
                    {
                        _newPosition = 1;

                        int newLoop = newPos - 68;
                        std::cout << "NewLoop = " << newLoop << std::endl;

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

                    //Si hay una barrera nos colocamos en la posicion de antes y acabamos
                    if (GetCell(i)->GetTokens().size() == 2)
                    {
                        _newPosition = i - 1;
                        continue;
                    }
                }          

                // Comprobamos si hay otra ficha en la nueva posición
                if (GetCell(_newPosition)->GetTokens().size() != 0)
                {
                    std::cout << GetCell(_newPosition)->GetTokens().size() << std::endl;

                    // Comprobamos si es ficha de otro jugador
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

                        // Moverlo 20 posiciones más
                        token->UpdateIdPosition(_newPosition);

                        UpdatePositions(token->Move(20));
                        return; //Acabamos ejecución
                    }
                }

                // Actualizamos la posición del token y lo añadimos a la nueva celda
                token->UpdateIdPosition(_newPosition);
                GetCell(token->GetIdPosition())->AddToken(token);
                token->EndMove();
            }
        }
    }


