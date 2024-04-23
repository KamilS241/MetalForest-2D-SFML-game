#include <iostream>
#include <ctime>
#include <utility>
#include <thread>

#include "Game.h"

//------------------------------------------------------INITIALIZATION------------------------------------------------------------------------------

Game::Game(sf::RenderWindow* windowRecived, sf::VideoMode videoModeRecived)
{
	//Zainicjuj grê
	this->window = windowRecived;
	this->vm = videoModeRecived;
	this->initVariables();
	this->setBackground("Textures/GameBackground.jpg");
	this->initFonts();
	this->initText();
	this->initPlayer();
}

void Game::setBackground(std::string background)
{
	if (!this->backgroundTex.loadFromFile(background))
	{
		std::cout << "BACKGROUNDTEX ERROR" << std::endl;
	}
	this->background.setTexture(this->backgroundTex);
}

void Game::initVariables()
{
	//Zainicjuj zmienne
	this->endGame = false;
	this->bossFight = false;
	this->points = 0;
	this->maxEnemies = 5;
	this->level = 0;
	this->entitySpawnTimerMax =100.f;
	this->entitySpawnTimer = this->entitySpawnTimerMax;
	this->mouseHeld = false;
}

void Game::initPlayer()
{
	std::unique_ptr<Entity> e = std::make_unique<Player>(this);
	player = std::move(e);
}

//------------------------------------------------------SPAWNING AND DELETING------------------------------------------------------------------------------

void Game::spawnEnemies()
{
	int glamMetals = std::get <0>(tupleOfEnemies);
	int nuMetals = std::get <1>(tupleOfEnemies);
	int deathMetals = std::get <2>(tupleOfEnemies);
	if (glamMetals != 0)
	{
		for (int i = 1; i <= glamMetals; i++)
		{
			std::unique_ptr<Entity> e = std::make_unique<Enemy>(this,0);
			enemies.push_back(std::move(e));
		}
	}
	if (nuMetals != 0)
	{
		for (int i = 1; i <= nuMetals; i++)
		{
			std::unique_ptr<Entity> e = std::make_unique<Enemy>(this,1);
			enemies.push_back(std::move(e));
		}
	}
	if (deathMetals != 0)
	{
		for (int i = 1; i <= deathMetals; i++)
		{
			std::unique_ptr<Entity> e = std::make_unique<Enemy>(this,2);
			enemies.push_back(std::move(e));
		}
	}

}

void Game::spawnHealthPack(int i)
{
	std::unique_ptr<Entity> h = std::make_unique<HealthPack>(this, enemies[i]->getPositionMethod());
	healthpacks.push_back(std::move(h));
}

void Game::deleteEnemy(int i)
{
	this->spawnHealthPack(i);
	auto temp = std::move(enemies[i]);
	this->enemies.erase(this->enemies.begin() + i);
	temp.reset();
}

void Game::spawnBullet(sf::Vector2f position, int type)
{
	std::unique_ptr<Entity> b = std::make_unique<Bullet>(this, position, type);
	projectiles.push_back(std::move(b));
}

void Game::spawnGrenade(sf::Vector2f position)
{
	std::unique_ptr<Entity> bo = std::make_unique<Grenade>(this, position);
	projectiles.push_back(std::move(bo));
}
//------------------------------------------------------UPDATING-------------------------------------------------------------------------------------

void Game::update()
{
	this->pollEvents();
	if (!this->endGame)
	{
		this->updateMousePosition();
		this->updateText();
		this->player->update();
		if (enemies.size() == 0)
		{
			this->level++;
			updateLevel();
			if (bossFight)
			{
				bossFight = false;
				for (int i = 0; i < numberOfBosses; i++)
				{
					std::unique_ptr<Entity> b = std::make_unique<Boss>(this, i);
					enemies.push_back(std::move(b));
				}
			}
			spawnEnemies();
		}
		for (auto& e : this->enemies)
		{
			e->update();
		}
		for (auto& b : this->projectiles)
		{
			b->update();
		}
		for (auto& h : this->healthpacks)
		{
			h->update();
		}
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->getToDelete())
			{
				deleteEnemy(i);
			}
		}
		for (int i = 0; i < projectiles.size(); i++)
		{
			if (projectiles[i]->getToDelete())
			{
				auto temp = std::move(projectiles[i]);
				this->projectiles.erase(this->projectiles.begin() + i);
				temp.reset();
			}
		}
		for (int i = 0; i < healthpacks.size(); i++)
		{
			if (healthpacks[i]->getToDelete())
			{
				auto temp = std::move(healthpacks[i]);
				this->healthpacks.erase(this->healthpacks.begin() + i);
				temp.reset();
			}
		}
	}
	if (this->player->getHealth() <= 0)
	{
		this->endGame = true;
	}
}

void Game::updateLevel()
{
	switch (level)
	{
	case 1: tupleOfEnemies = { 0,1,0 }; break;
	case 2: tupleOfEnemies = { 3,0,0 }; break;
	case 3: tupleOfEnemies = { 1,1,1 }; break;
	case 4: tupleOfEnemies = { 0,0,0 }; bossFight = true; numberOfBosses = 1; break;
	case 5: tupleOfEnemies = { 4,0,0 }; break;
	case 6: tupleOfEnemies = { 1,3,0 }; break;
	case 7: tupleOfEnemies = { 0,0,2 }; break;
	case 8: tupleOfEnemies = { 0,0,0 }; bossFight = true; numberOfBosses = 2; break;
	case 9: tupleOfEnemies = { 10,0,0 }; break;
	case 10: tupleOfEnemies = { 2,2,2 }; break;
	case 11: tupleOfEnemies = { 3,3,3 }; break;
	case 12: tupleOfEnemies = { 0,0,0 }; bossFight = true; numberOfBosses = 3; break;
	default: this->endGame = true;
	}
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
			{
				//todo PAUSE MENU
			}
			break;
		}
	}
}

void Game::updateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::addPoint()
{
	this->points++;
}
//------------------------------------------------------INTERACTIONS------------------------------------------------------------------------------

void Game::dealDamageToPlayer(int damage)
{
	this->player->lowerHealth(damage);
}

void Game::dealDamageToEnemy(int position, int damage)
{
	this->enemies[position]->lowerHealth(damage);
}

bool Game::checkIntersectionWithPlayer(sf::Sprite bullet)
{
	if (this->player->checkIntersection(bullet.getGlobalBounds()))
	{
		this->player->lowerHealth(1);
		return true;
	}
	else
		return false;
}
bool Game::checkIntersectionWithEnemy(sf::Sprite bullet)
{
	for (auto& e : enemies)
	{
		if (e->checkIntersection(bullet.getGlobalBounds()))
		{
			e->lowerHealth(1);
			return true;
		}
		else
			continue;
	}
	return false;
}

void Game::healPlayer()
{
	this->player->setHealth(this->player->getHealth() + 1);
}
//------------------------------------------------------GETTERS AND SETTERS------------------------------------------------------------------------------

std::vector<sf::FloatRect> Game::getOtherEntitiesBounds()
{
	std::vector<sf::FloatRect> bounds;
	if (enemies.size() != 0)
		for (auto& e : enemies)
		{
			bounds.push_back(e->getShape()->getGlobalBounds());
		}
	return bounds;
}

size_t Game::getEnemiesSize()
{
	return enemies.size();
}

sf::VideoMode Game::getVM()
{
	return vm;
}

sf::Sprite* Game::getEnemyShape(int position)
{
	return enemies[position]->getShape();
}

sf::Sprite* Game::getPlayerShape()
{
	return player->getShape();
}

int Game::getScore()
{
	return points;
}

const bool Game::getWindowStatus() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}
//------------------------------------------------------RENDERING-------------------------------------------------------------------------------------
void Game::render()
{
	//Renderuj grê
	this->window->clear();
	this->renderWorld();
	this->player->render(*this->window);
	this->renderEnemies();
	this->renderBullets();
	this->renderHealthPacks();
	this->renderText();
	this->window->display();
}

void Game::renderWorld()
{
	this->window->draw(this->background);
}

void Game::renderEnemies()
{
	for (auto& e : enemies)
	{
		e->render(*this->window);
	}
}

void Game::renderBullets()
{
	for (auto& b : projectiles)
	{
		b->render(*this->window);
	}
}

void Game::renderHealthPacks()
{
	for (auto & h : this->healthpacks)
	{
		h->render(*this->window);
	}
}
//------------------------------------------------------TEXTS AND FONTS------------------------------------------------------------------------------
void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/AmaticSC-Regular.ttf"))
	{
		std::cout << "ERROR UNABLE TO LOAD FONTS" << std::endl;
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(40);
	this->uiText.setFillColor(sf::Color::Red);
	this->uiText.setString("METAL FOREST");
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Heads collected: " << this->points << std::endl
		<< "Health: " << this->player->getHealth() << std::endl
		<< "Level: "<< this->level;
	this->uiText.setString(ss.str());
}

void Game::renderText()
{
	this->window->draw(this->uiText);
}