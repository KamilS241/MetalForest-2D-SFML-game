#include <iostream>
#include <thread>
#include <memory>

#include "Game.h"
#include "Entities.h"
#include "App.h"

//Objaśnienia:
//Gamephases: 1-mainMenu, 2-game, 3-endGameMenu, 4-closeGame
//Pliki App: zawierają klasę App, odpowiedzialną za kontrolowanie UI, tworzenie gry, ekrany przed i po grze, zapisywanie wyników
//Pliki Game: zawierają klasę Game, odpowiedzialną za logikę samej gry
//Pliki Entities: zawierają klasę wirtualną Entity, po której dziedziczą wszystkie inne obecne tam klasy, odpowiadają za wszystkie obiekty
//znajdujące się w grze

int main()
{
    App MetalForest;
    std::thread musicThread([&MetalForest]() {MetalForest.musicMethod();});
    while (MetalForest.getGamePhase() != 4)
    {
        switch (MetalForest.getGamePhase())
        {
        case 1: MetalForest.mainMenuMethod(); break;
        case 2: MetalForest.gameMethod(); break;
        case 3: MetalForest.endGameMethod(); break;
        default: std::cout << "GAMEPHASE ERROR" << std::endl; break;
        }
    }
    musicThread.join();
    return 0;
}
