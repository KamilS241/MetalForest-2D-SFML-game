#pragma once
#include <utility>

#include "Game.h"

class Game;

class Entity
{
private:
	Game* gamePtr;
	int health;
	int maxEnemies;
	float speed;
	sf::VideoMode vm;
	bool toDelete = false;
	int fireRateMax = 0;
	int fireRate = 0;

	sf::Sprite entity;
	sf::Texture entityTex;
	sf::Texture entityTexDamage;
	int damageTexCountdown = 0;

	//Square move variables
	int direction = 0;//1-left, 2-right, 3-up, 4-down, 5-upleft, 6-upright, 7-downleft, 8-downright
	float randX = 0;
	float randY = 0;
	sf::Vector2f movementVector;
	//Diagonal move variables
	float diagonalVectorLength;
	sf::Vector2f velocity = { 1.f,1.f };
	bool targetSet=false;

public:
	
	virtual void initEntity() = 0;
	virtual void update()=0;
	virtual void move() = 0;
	virtual void attack()=0;
	virtual void setPositionMethod()=0;
	
	void render(sf::RenderTarget& Target);
	void setTextureMethod(int type);
	void setScaleMethod(float scale);
	void setSpeed(float speedToSet);
	void setHealth(int healthToSet);
	void setFireRateMax(int value);
	void restoreFireRate();
	void lowerFireRate();
	void setVideoMode();
	void setGamePtr(Game* gameRecived);
	void setToDelete();
	void loadTextures(std::string normal, std::string damage); //1- normal texture, 2-damage texture

	sf::Sprite* getShape();
	sf::Vector2f getPositionMethod();
	sf::VideoMode getVideoMode();
	float getSpeed();
	Game* getGamePtr();
	int getToDelete();
	int getHealth();
	int getFireRate();
	
	void randomMove();
	void moveSquare(sf::Vector2f targetPoint);
	void moveDiagonallyToPlayer(sf::Vector2f targetPoint);
	void moveSquareToVoid();
	
	void manageDamageTex();
	void lowerHealth(int damage);
	bool checkIntersection(sf::FloatRect bullet);

	~Entity() {};
};

class Player :public Entity
{
private:
	bool weaponRotated = false;
	int count;
public:
	Player(Game* gameRecived);
	void initEntity() override;
	void update() override;
	void move()override;
	void attack()override;
	void setPositionMethod()override;
};

class Enemy : public Entity
{
private:
	int damage;
	int type;
public:
	Enemy(Game* gameRecived, int typeRecived);
	void initEntity() override;
	void setPositionMethod()override;
	void update()override;
	void move()override;
	void attack() override;
	
	void setDamage(int value);
};

class Boss : public Entity
{
private:
	int position;
	int bombFireRate=180;
	int bombFireRateMax=0;
public:
	Boss(Game* gameRecived, int positionRecived);
	void initEntity() override;
	void setPositionMethod()override;
	void update()override;
	void move()override;
	void attack() override;
};

class Bullet : public Entity
{
private:
	int type; //1- bullet, 2-guitar
	sf::Vector2f playerPosition;
	sf::Vector2f ownerPosition;
public:
	Bullet(Game* gameRecived, sf::Vector2f position, int type);
	void initEntity()override;
	void setPositionMethod()override;
	void update()override;
	void move()override;
	void attack()override {};

	void checkWorldCollision();
};

class Grenade : public Entity
{
private:
	sf::Vector2f ownerPosition;
	int detonationCountdown;
	int explosionCountdown;
	bool detonated = false;
public:
	Grenade(Game* gameRecived, sf::Vector2f position);
	void initEntity()override;
	void setPositionMethod()override;
	void update()override;
	void move()override {};
	void attack()override {};
};

class HealthPack : public Entity
{
private:
	sf::Vector2f spawnPosition;
	int countMax;
	int count;
public:
	HealthPack(Game* gameRecived, sf::Vector2f position);
	void initEntity() override;
	void setPositionMethod()override;
	void update()override;
	void move()override {};
	void attack() override {};
};