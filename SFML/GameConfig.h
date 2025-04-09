#pragma once

#define GC GameConfig::instance()
#define WIDTH 950	
#define HEIGHT 950

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