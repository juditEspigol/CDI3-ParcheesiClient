#pragma once

#define GC GameConfig::instance()
#define WIDTH 960	
#define HEIGHT 960

class GameConfig
{
private:
    GameConfig();
  
public:
    static GameConfig& instance()
    {
        static GameConfig INSTANCE;
        return INSTANCE;
    }
};