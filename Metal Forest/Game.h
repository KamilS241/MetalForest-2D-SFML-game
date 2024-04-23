#pragma once
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <memory>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Entities.h"

class Entity;
class Enemy;
class Player;

class Game
{
private:
	
	bool startOver = true;
	//Window
	sf::RenderWindow* window;
	sf::VideoMode vm;

	//Textures
	sf::Sprite background;
	sf::Texture backgroundTex;

	//Events
	sf::Event ev;
	sf::Event ev2;
	sf::Event ev3;

	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	sf::Font font;

	sf::Text uiText;

	//Game logic
	bool endGame;
	unsigned points;
	int level; //every 4th level- boss fight
	int numberOfEnemies;
	int maxEnemies;
	bool bossFight;
	int numberOfBosses;
	std::tuple <int, int, int> tupleOfEnemies;
	float entitySpawnTimer;
	float entitySpawnTimerMax;
	bool mouseHeld;


	//Game objects
	std::unique_ptr<Entity> player;
	std::vector<std::unique_ptr<Entity>> enemies;
	std::vector <std::unique_ptr<Entity>> projectiles;
	std::vector <std::unique_ptr<Entity>> healthpacks;

	void initVariables();
	void setBackground(std::string background);
	void initFonts();
	void initText();
	void initPlayer();
	void spawnEnemies();
	void spawnHealthPack(int i);

public:
	
	Game(sf::RenderWindow* windowRecived, sf::VideoMode videoModeRecived);

	void pollEvents();
	void updateMousePosition();
	void updateText();
	void update();
	void updateLevel();

	void spawnBullet(sf::Vector2f position, int type);
	void spawnGrenade(sf::Vector2f position);
	void dealDamageToPlayer(int damage);
	void dealDamageToEnemy(int position, int damage);
	bool checkIntersectionWithPlayer(sf::Sprite bullet);
	bool checkIntersectionWithEnemy(sf::Sprite bullet);
	void deleteEnemy(int i);
	void healPlayer();
	void addPoint();

	void renderText();
	void render();
	void renderEnemies();
	void renderBullets();
	void renderHealthPacks();
	void renderWorld();

	//Getters and setters
	std::vector<sf::FloatRect> getOtherEntitiesBounds();
	size_t getEnemiesSize();
	sf::VideoMode getVM();
	sf::Sprite* getEnemyShape(int position);
	sf::Sprite* getPlayerShape();
	int getScore();
	const bool getWindowStatus() const;
	const bool getEndGame() const;

	~Game() {};
};
