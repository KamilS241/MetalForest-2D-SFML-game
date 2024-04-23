#include <iostream>
#include <utility>
#include <cmath>

#include "Entities.h"
//---------------------------------------------------------------------------------ENTITY---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

sf::Sprite* Entity::getShape()
{
	sf::Sprite* eptr = & entity;
	return eptr;
}

void Entity::setTextureMethod(int type)
{
	switch (type)
	{
	case 1: this->entity.setTexture(this->entityTex); break;
	case 2: this->entity.setTexture(this->entityTexDamage); break;
	}
}

void Entity::setScaleMethod(float scale)
{
	this->entity.setScale(scale, scale);
}

void Entity::setVideoMode()
{
	vm = this->gamePtr->getVM();
}

void Entity::setGamePtr(Game* gameRecived)
{
	gamePtr = gameRecived;
}

void Entity::setToDelete()
{
	this->toDelete = true;
}

void Entity::loadTextures(std::string normal, std::string damage)
{
	if (!this->entityTex.loadFromFile(normal))
	{
		std::cout << normal << " ERROR" << std::endl;
	}
	if (!this->entityTexDamage.loadFromFile(damage))
	{
		std::cout << damage << " ERROR" << std::endl;
	}
}

void Entity::randomMove()
{
	if (movementVector.x <= 0 and movementVector.y <= 0)
	{
		randX = static_cast<float>(rand() % static_cast<int> (this->vm.width));
		randY = static_cast<float>(rand() % static_cast<int> (this->vm.height));
	}
	sf::Vector2f randPos = { randX,randY };
	moveSquare(randPos);
}

void Entity::moveSquare(sf::Vector2f targetPoint)
{
	if (movementVector.x <= 0 and movementVector.y <= 0)
	{
		sf::Vector2f ePos = this->entity.getPosition();
		auto tempX = ePos.x - targetPoint.x;
		auto tempY = ePos.y - targetPoint.y;
		if (tempX > 0 and tempY == 0)
		{
			this->direction = 1;
		}
		else if (tempX < 0 and tempY == 0)
		{
			this->direction = 2;
		}
		else if (tempX == 0 and tempY > 0)
		{
			this->direction = 3;
		}
		else if (tempX == 0 and tempY < 0)
		{
			this->direction = 4;
		}
		else if (tempX > 0 and tempY > 0)
		{
			this->direction = 5;
		}
		else if (tempX < 0 and tempY > 0)
		{
			this->direction = 6;
		}
		else if (tempX > 0 and tempY < 0)
		{
			this->direction = 7;
		}
		else if (tempX < 0 and tempY < 0)
		{
			this->direction = 8;
		}
		movementVector.x = abs(ePos.x - targetPoint.x);
		movementVector.y = abs(ePos.y - targetPoint.y);
	}
	else
	{
		if (direction > 4 and (movementVector.x <= 0 or movementVector.y <= 0))
		{
			if ((direction == 5 and movementVector.x <= 0))
				direction = 3;
			if ((direction == 5 and movementVector.y <= 0))
				direction = 1;
			if ((direction == 6 and movementVector.x <= 0))
				direction = 3;
			if ((direction == 6 and movementVector.y <= 0))
				direction = 2;
			if ((direction == 7 and movementVector.x <= 0))
				direction = 4;
			if ((direction == 7 and movementVector.y <= 0))
				direction = 1;
			if ((direction == 8 and movementVector.x <= 0))
				direction = 4;
			if ((direction == 8 and movementVector.y <= 0))
				direction = 2;
		}
		else
		{
			switch (direction)
			{
			case 1: //Left
				this->entity.move(-speed, 0); 
				movementVector.x -= speed;
				break;
			case 2: //Right
				this->entity.move(speed, 0); 
				movementVector.x -= speed;
				break;
			case 3: //Up
				this->entity.move(0, -speed); 
				movementVector.y -= speed;
				break;
			case 4: //Down
				this->entity.move(0, speed); 
				movementVector.y -= speed;
				break;
			case 5: //Upleft
				this->entity.move(-speed, -speed); 
				movementVector.x -= speed;
				movementVector.y -= speed;
				break;
			case 6: //Upright
				this->entity.move(speed, -speed); 
				movementVector.x -= speed;
				movementVector.y -= speed;
				break;
			case 7: //Downleft
				this->entity.move(-speed, speed); 
				movementVector.x -= speed;
				movementVector.y -= speed;
				break;
			case 8: //Downright
				this->entity.move(speed, speed); 
				movementVector.x -= speed;
				movementVector.y -= speed;
				break;
			}
		}
	}
}

void Entity::moveDiagonallyToPlayer(sf::Vector2f targetPoint)
{
	if (!targetSet)
	{
		auto epos = this->entity.getPosition();
		movementVector.x = epos.x - targetPoint.x;
		movementVector.y = epos.y - targetPoint.y;
		diagonalVectorLength = std::sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);
		velocity.x = movementVector.x / diagonalVectorLength;
		velocity.y = movementVector.y / diagonalVectorLength;
		targetSet = true;
	}
	else
	{
		this->entity.move(-velocity * speed);
	}
}

void Entity::moveSquareToVoid()
{
	if (!targetSet)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 3;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::W) 
			and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 4;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and !sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			this->direction = 2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 5;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 6;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) and sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 7;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) and sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
			and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			this->direction = 8;
		targetSet = true;
		return;
	}
	else if (targetSet)
	{
		switch (direction)
		{
		case 0: //When player is not moving, he shoots to the right		
			this->entity.move(speed, 0);
			break;
		case 1: //Left
			this->entity.move(-speed, 0);
			break;
		case 2: //Right
			this->entity.move(speed, 0);
			break;
		case 3: //Up
			this->entity.move(0, -speed);
			break;
		case 4: //Down
			this->entity.move(0, speed);
			break;
		case 5: //Upleft
			this->entity.move(-speed, -speed);
			break;
		case 6: //Upright
			this->entity.move(speed, -speed);
			break;
		case 7: //Downleft
			this->entity.move(-speed, speed);
			break;
		case 8: //Downright
			this->entity.move(speed, speed);
			break;
		}
		this->entity.rotate(10);
	}
}

void Entity::setSpeed(float speedToSet)
{
	this->speed = speedToSet;
}

void Entity::setHealth(int healthToSet)
{
	this->health = healthToSet;
}

void Entity::setFireRateMax(int value)
{
	this->fireRateMax = value;
}

void Entity::restoreFireRate()
{
	fireRate = fireRateMax;
}

void Entity::lowerFireRate()
{
	fireRate--;
}

void Entity::manageDamageTex()
{
	if (this->damageTexCountdown <= 0)
	{
		this->setTextureMethod(1);
	}
	else
	{
		this->damageTexCountdown--;
	}
}

void Entity::lowerHealth(int damage)
{
	health-= damage;
	if (this->health <= 0)
	{
		this->setToDelete();
	}
	this->setTextureMethod(2);
	damageTexCountdown = 20;
}

bool Entity::checkIntersection(sf::FloatRect bullet)
{
	if (bullet.intersects(this->entity.getGlobalBounds()))
		return true;
	else
		return false;
}

sf::Vector2f Entity::getPositionMethod()
{
	return entity.getPosition();
}

sf::VideoMode Entity::getVideoMode()
{
	return vm;
}

float Entity::getSpeed()
{
	return speed;
}

Game* Entity::getGamePtr()
{
	return gamePtr;
}

int Entity::getToDelete()
{
	return toDelete;
}

int Entity::getHealth()
{
	return health;
}

int Entity::getFireRate()
{
	return fireRate;
}

void Entity::render(sf::RenderTarget& target)
{
	target.draw(this->entity);
}

//---------------------------------------------------------------------------------PLAYER-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Player::Player(Game* gameRecived)
{
	this->setGamePtr(gameRecived);
	this->setVideoMode();
	this->initEntity();
}

void Player::initEntity()
{
	this->setHealth(10);
	this->loadTextures("Textures/Player.png", "Textures/PlayerDamage.png");
	this->setTextureMethod(1);
	this->setPositionMethod();
	this->setScaleMethod(0.25);
	this->setSpeed(5);
	this->setFireRateMax(60);
	this->getShape()->setOrigin(this->getShape()->getLocalBounds().width / 2, this->getShape()->getLocalBounds().height / 2);
}

void Player::setPositionMethod()
{
	auto entity = this->getShape();
	sf::VideoMode vm = this->getVideoMode();
	entity->setPosition(vm.width/2, vm.height/2);
}

void Player::update()
{
	this->move();
	this->attack();
	this->manageDamageTex();
}

void Player::move()
{
	auto playerSpeed = this->getSpeed();
	auto player = this->getShape();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(0, -playerSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(0, playerSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(-playerSpeed, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		player->move(playerSpeed, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) and sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(-playerSpeed, -playerSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and !sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(playerSpeed, -playerSpeed);
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) and sf::Keyboard::isKeyPressed(sf::Keyboard::S) and sf::Keyboard::isKeyPressed(sf::Keyboard::A) and !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(-playerSpeed, playerSpeed);
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) and sf::Keyboard::isKeyPressed(sf::Keyboard::S) and !sf::Keyboard::isKeyPressed(sf::Keyboard::A) and sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->move(playerSpeed, playerSpeed);
}

void Player::attack()
{
	auto game = this->getGamePtr();
	auto size = game->getEnemiesSize();
	if (this->getFireRate() == 0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			this->getGamePtr()->spawnBullet(this->getPositionMethod(), 2);
			restoreFireRate();
		}
	}
	else
	{
		lowerFireRate();
	}
}

//---------------------------------------------------------------------------------Enemy----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Enemy::Enemy(Game* gameRecived, int typeRecived)
{
	type = typeRecived;
	this->setGamePtr(gameRecived);
	setVideoMode();
	initEntity();
}

void Enemy::initEntity()
{
	switch (type)
	{
	case 0:
		this->loadTextures("Textures/GlamMetal.png", "Textures/GlamMetalDamage.png");
		this->setTextureMethod(1);
		this->setScaleMethod(0.25);
		this->setSpeed(7);
		this->setFireRateMax(120);
		this->setHealth(1);
		break;
	case 1:
		this->loadTextures("Textures/NuMetal.png", "Textures/NuMetalDamage.png");
		this->setTextureMethod(1);
		this->setScaleMethod(0.2);
		this->setSpeed(3);
		this->setFireRateMax(80);
		this->setHealth(2);
		break;
	case 2:
		this->loadTextures("Textures/DeathMetal.png", "Textures/DeathMetalDamage.png");
		this->setTextureMethod(1);
		this->setScaleMethod(0.25);
		this->setSpeed(2);
		this->setFireRateMax(60);
		this->setHealth(3);
		break;
	default:
		break;
	}
	this->setPositionMethod();
	this->getShape()->setOrigin(this->getShape()->getLocalBounds().width / 2, this->getShape()->getLocalBounds().height / 2);
}

void Enemy::setPositionMethod()
{
	auto vm = this->getVideoMode();
	auto entity = this->getShape();
	float randPosX;
	float randPosY;
	bool loopEnd1 = false;
	bool loopEnd2 = false;
	while (!loopEnd1)
	{
		int position = rand() % 4;
		if (position == 0)//dol
		{
			entity->setPosition(static_cast<float>(rand() % static_cast<int>(vm.width - entity->getGlobalBounds().width)),
				vm.height - entity->getGlobalBounds().height + 300.f);
			return;
		}
		if (position == 1) //góra
		{
			entity->setPosition(static_cast<float>(rand() % static_cast<int>(vm.width - entity->getGlobalBounds().width)), -300.f);
			return;
		}
		if (position == 2) //lewo
		{
			entity->setPosition(-300.f, static_cast<float>(rand() % static_cast<int>(vm.height - entity->getGlobalBounds().height)));
			return;
		}
		if (position == 3)//prawo
		{
			entity->setPosition(vm.width - entity->getGlobalBounds().width + 300.f,
				static_cast<float>(rand() % static_cast<int>(vm.height - entity->getGlobalBounds().height)));
			return;
		}
	}
}

void Enemy::update()
{
	if (this->getHealth() <= 0)
	{
		this->setToDelete();
		return;
	}
	else
	{
		this->attack();
		this->move();
		this->manageDamageTex();
	}
}

void Enemy::move()
{
	this->randomMove();
}

void Enemy::attack()
{
	if (this->getFireRate() == 0)
	{
		this->getGamePtr()->spawnBullet(this->getPositionMethod(), 1);
		this->restoreFireRate();
	}
	else
	{
		this->lowerFireRate();
	}
}

void Enemy::setDamage(int value)
{
	this->damage = value;
}
//-----------------------------------------------------BOSS--------------------------------------------------------------------------------------------

Boss::Boss(Game* gameRecived, int positionRecived)
{
	this->setGamePtr(gameRecived);
	this->position = positionRecived;
	setVideoMode();
	initEntity();
}

void Boss::initEntity()
{
	this->loadTextures("Textures/Boss.png", "Textures/BossDamage.png");
	this->setTextureMethod(1);
	this->setScaleMethod(0.5);
	this->setSpeed(2);
	this->setFireRateMax(30);
	this->bombFireRateMax = 120;
	this->setHealth(10);
	this->setPositionMethod();
	this->getShape()->setOrigin(this->getShape()->getLocalBounds().width / 2, this->getShape()->getLocalBounds().height / 2);
}

void Boss::setPositionMethod()
{
	switch (this->position)
	{
	case 0: this->getShape()->setPosition(this->getVideoMode().width + 100, this->getVideoMode().height / 2); break;
	case 1: this->getShape()->setPosition(-100, this->getVideoMode().height / 2); break;
	case 2: this->getShape()->setPosition(this->getVideoMode().width /2, this->getVideoMode().height +100); break;
	}
}

void Boss::update()
{
	if (this->getHealth() <= 0)
	{
		this->setToDelete();
		return;
	}
	else
	{
		this->attack();
		this->move();
		this->manageDamageTex();
	}
}

void Boss::move()
{
	this->randomMove();
}

void Boss::attack()
{
	if (this->getFireRate() == 0)
	{
		this->getGamePtr()->spawnBullet(this->getPositionMethod(), 1);
		this->restoreFireRate();
	}
	else
	{
		this->lowerFireRate();
	}
	if (bombFireRate == 0)
	{
		this->getGamePtr()->spawnGrenade(this->getPositionMethod());
		this->bombFireRate = this->bombFireRateMax;
	}
	else
	{
		this->bombFireRate--;
	}
}


//---------------------------------------------------------------------------------BULLET----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Bullet::Bullet(Game* gameRecived, sf::Vector2f position, int type)
{
	this->setGamePtr(gameRecived);
	this->type = type;
	setVideoMode();
	this->ownerPosition = position;
	this->initEntity();
}

void Bullet::initEntity()
{
	if (type == 1)
	{
		this->loadTextures("Textures/Bullet.png", "Textures/Bullet.png");
		this->setTextureMethod(1);
		this->setScaleMethod(0.03);
		this->setSpeed(8);
		this->setPositionMethod();
	}
	else if (type == 2)
	{
		this->loadTextures("Textures/Guitar.png", "Textures/Guitar.png");
		this->setTextureMethod(1);
		this->setScaleMethod(0.15);
		this->setSpeed(10);
		this->setPositionMethod();
	}
	this->getShape()->setOrigin(this->getShape()->getLocalBounds().width / 2, this->getShape()->getLocalBounds().height / 2);
}

void Bullet::setPositionMethod()
{
	this->getShape()->setPosition(ownerPosition);
}

void Bullet::update()
{
	if (this->type == 1)
	{
		if (this->getGamePtr()->checkIntersectionWithPlayer(*this->getShape()))
		{
			this->setToDelete();
			return;
		}

	}
	else if (this->type == 2)
	{
		if (this->getGamePtr()->checkIntersectionWithEnemy(*this->getShape()))
		{
			this->setToDelete();
			return;
		}
	}
	this->move();
	this->checkWorldCollision();
}

void Bullet::move()
{
	if (type == 1)
		this->moveDiagonallyToPlayer(this->getGamePtr()->getPlayerShape()->getPosition());
	else if (type == 2)
	{
		this->moveSquareToVoid();
	}
}

void Bullet::checkWorldCollision()
{
	if (this->getShape()->getPosition().x < 0 or this->getShape()->getPosition().x > this->getGamePtr()->getVM().width
		or this->getShape()->getPosition().y < 0 or this->getShape()->getPosition().y > this->getGamePtr()->getVM().height)
	{
		this->setToDelete();
	}
}
//-------------------------------------------------------BOMB---------------------------------------------------------------------------------------------

Grenade::Grenade(Game* gameRecived, sf::Vector2f position)
{
	this->setGamePtr(gameRecived);
	setVideoMode();
	this->ownerPosition = position;
	this->initEntity();
}

void Grenade::initEntity()
{
	this->loadTextures("Textures/Grenade.png", "Textures/Explosion.png");
	this->setTextureMethod(1);
	this->setScaleMethod(0.08);
	this->setPositionMethod();
	this->detonationCountdown = 60;
	this->explosionCountdown = 180;
}

void Grenade::setPositionMethod()
{
	this->getShape()->setPosition(ownerPosition);
	this->getShape()->setOrigin(this->getShape()->getLocalBounds().width / 2, this->getShape()->getLocalBounds().height / 2);
}

void Grenade::update()
{
	if (!detonated)
	{
		if (this->detonationCountdown == 0)
		{
			this->setTextureMethod(2);
			this->setScaleMethod(0.5);
			detonated = true;
		}
		else
		{
			detonationCountdown--;
		}
	}
	else
	{
		if (explosionCountdown == 0)
		{
			this->setToDelete();
			return;
		}
		else
		{
			explosionCountdown--;
			this->getGamePtr()->checkIntersectionWithPlayer(*this->getShape());
		}
	}
}

//---------------------------------------------------------------------------------HEALTHPACK--------------------------------------------------------

HealthPack::HealthPack(Game* gameRecived, sf::Vector2f position)
{
	this->spawnPosition = position;
	this->setGamePtr(gameRecived);
	this->initEntity();
}

void HealthPack::initEntity()
{
	this->loadTextures("Textures/HealthPack.png", "Textures/HealthPack.png");
	this->setTextureMethod(1);
	this->setScaleMethod(0.1);
	this->getShape()->setOrigin(this->getShape()->getLocalBounds().width / 2, this->getShape()->getLocalBounds().height / 2);
	this->setPositionMethod();
	this->countMax = 180;
}

void HealthPack::setPositionMethod()
{
	this->getShape()->setPosition(spawnPosition);
}

void HealthPack::update()
{
	if (this->getShape()->getGlobalBounds().intersects(this->getGamePtr()->getPlayerShape()->getGlobalBounds()))
	{
		this->getGamePtr()->healPlayer();
		this->getGamePtr()->addPoint();
		this->setToDelete();
	}
}