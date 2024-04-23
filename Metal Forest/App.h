#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <sstream>
#include <regex>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <ranges>

#include "Entities.h"

class App
{
private:

	sf::RenderWindow* appWindowPtr;
	sf::VideoMode vm;
	int gamePhase;
	bool soundtrackSet;
	int score;
	sf::Sprite background;
	sf::Texture backgroundTex;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Music soundtrack;

	void initWindow();
public:

	App();

	void mainMenuMethod();
	void gameMethod();
	void musicMethod();
	void endGameMethod();

	void saveScore();
	void uploadScoreToFile(std::string name);
	void pollEvents();
	void showScoreBoard();

	int getGamePhase();
	void setGamePhase(int phase);

	~App();
};