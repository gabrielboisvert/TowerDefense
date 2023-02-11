#include "pch.h"
#include "TowerDefenseGameManager.h"
#include "ConfigTower.h"
#include "RegularTower.h"
#include "ExplosiveTower.h"
#include "StunTower.h"
#include <SFML\Graphics\RectangleShape.hpp>
#include "PathFinding.h"


using namespace TD;

TowerDefenseGameManager TowerDefenseGameManager::instance;

TowerDefenseGameManager::TowerDefenseGameManager()
{
	this->currentState = GameState::MAIN_MENU;
	this->renderer.loadFont("Assets/fonts/GROBOLD.ttf");
	
	this->scanner = nullptr;
	this->mapChoosed = "";
	
	this->choosedTower = nullptr;
	
	this->upgradeButton = nullptr;
	this->towerToUpgrade = nullptr;
	this->warmupText = nullptr;
}

void TowerDefenseGameManager::init()
{	this->choosedTower = nullptr;
	this->towerToUpgrade = nullptr;
	
	this->mapChoosed = "";
	
	this->map.reset();
	this->enemyArmy.reset();
	this->player.reset();
	
	this->initMainMenuKey();
	this->initMainMenuButton();
}

void TD::TowerDefenseGameManager::update()
{
	this->updateDeltaTime();

	if (this->currentState == TD::GameState::RUNNING)
	{
		//Check player health
		if (this->player.baseHealth == 0 || (this->map.getWaveTimer().size() == this->enemyArmy.getWave() && this->enemyArmy.getArmy().size() == 0) )
		{
			this->currentState = TD::GameState::GAME_OVER;
			this->updateState();
			return;
		}
		//update enemyArmy
		std::vector<std::shared_ptr<TD::Enemy*>>& army = this->enemyArmy.getArmy();
		for (unsigned int i = 0; i < army.size(); i++)
		{
			(*army[i].get())->addTime(this->deltaTime);
			if ((*army[i].get())->getSpawnCounter() >= (*army[i].get())->getSpawnTimer() && !(*army[i].get())->isSpawn())
				this->enemyArmy.initSprite(this->map, this->renderer, this->player, i);
			else if ((*army[i].get())->isSpawn())
				(*army[i].get())->update(this->deltaTime);
		}
		//Update tower
		for (unsigned int i = 0; i < this->map.getTowers().size(); i++)
			this->map.getTowers()[i]->update(this->deltaTime, this->enemyArmy.getArmy(), this->renderer, this->player);
		//Check if new wave
		if (this->map.getWaveTimer().size() > this->enemyArmy.getWave())
		{	
			this->currentWaveTimer += sf::seconds(this->deltaTime);
			if (this->currentWaveTimer >= this->map.getWaveTimer()[this->enemyArmy.getWave()])
				this->enemyArmy.updateWave();
			
			if (this->enemyArmy.getWave() != 0 && this->warmupText != nullptr)
			{
				this->renderer.removeText(*this->warmupText);
				delete this->warmupText;
				this->warmupText = nullptr;
			}
		}
	}
}

void TowerDefenseGameManager::updateState()
{
	this->renderer.getWindow()->changeCursor(TD::Cursor::Default);
	this->renderer.clear();
	this->player.clear();
	this->scanner->clearInputActions();
	this->clock.restart();

	if (this->currentState == GameState::MAIN_MENU)
		this->init();
	else if (this->currentState == GameState::RUNNING)
	{
		if (this->map.getCells().size() == 0)
		{
			this->currentWaveTimer = sf::Time();
			this->map.buildFromFile(this->mapChoosed, this->renderer, this->enemyArmy);
			this->player.money = this->map.getStartBudget();
			this->player.baseHealth = this->map.getdefaultHQLife();
			
			this->map.findPath();
		}
		else
		{
			this->map.fillSprite(this->renderer);
			this->enemyArmy.fillSprite(this->renderer);
		}
		this->initGameMenuKey();
		this->initGameMenuButton();
	}
	else if (this->currentState == GameState::PAUSED)
	{
		if (this->towerToUpgrade != nullptr)
			this->towerToUpgrade->getSprite()->drawShape = false;
		this->initPauseMenuKey();
		this->initPauseMenuButton();
	}
	else if (this->currentState == GameState::GAME_OVER)
	{
		this->initGameOverMenuKey();
		this->initGameOverMenuButton();
		this->map.reset();
	}
	else if (this->currentState == GameState::QUIT)
	{
		this->renderer.getWindow()->getWindow().close();
		this->map.reset();
	}
}

void TowerDefenseGameManager::render()
{
	this->renderer.DrawSprites();
}

TowerDefenseGameManager& TowerDefenseGameManager::getInstance()
{
	return TowerDefenseGameManager::instance;
}

TD::Renderer& TD::TowerDefenseGameManager::getRenderer()
{
	return this->renderer;
}

void TD::TowerDefenseGameManager::setState(TD::GameState state)
{
	this->currentState = state;
}

void TD::TowerDefenseGameManager::setInputeScanner(InputScanner& inputeScanner)
{
	this->scanner = &inputeScanner;
}

bool TD::TowerDefenseGameManager::isValidPosition(const sf::Rect<float>& hitBox)
{
	sf::Vector2u sizeScreen = this->renderer.getWindow()->getWindow().getSize();
	if (hitBox.left < 0 || hitBox.top < 0 || hitBox.left + hitBox.width > sizeScreen.x || hitBox.top + hitBox.height > sizeScreen.y)
		return false;

	for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
		if (this->player.hub.windows[i]->sprite->getGlobalBounds().intersects(hitBox))
			return false;
	
	std::vector<TD::ITower*>& towers = this->map.getTowers();
	for (unsigned int i = 0; i < towers.size(); i++)
		if (towers[i]->getSprite()->getGlobalBounds().intersects(hitBox) && towers[i] != this->choosedTower)
			return false;

	std::vector<std::vector<Cell*>>& cells = this->map.getCells();
	for (unsigned int i = 0; i < cells.size(); i++)
		for (unsigned int j = 0; j < cells[i].size(); j++)
			if (cells[i][j]->getSprite().getGlobalBounds().intersects(hitBox))
				if (cells[i][j]->get_terrain() != TerrainType::Grass)
					return false;
	return true;
}

TD::HUBWindow* TD::TowerDefenseGameManager::createHub(const char* spritePath, sf::Vector2f pos, sf::Vector2f size, unsigned int layer)
{
	TD::HUBWindow* window = new HUBWindow(layer);
	this->player.hub.windows.push_back(window);

	TD::Sprite& spHub = renderer.CreateSprite(*renderer.GetTexture(spritePath), (sf::Vector2i)pos, layer);
	spHub.setScale(size.x / spHub.getGlobalBounds().width, size.y / spHub.getGlobalBounds().height);
	
	window->sprite = &spHub;
	window->position = (sf::Vector2i)pos;

	return window;
}

TD::HUBButton* TD::TowerDefenseGameManager::createButton(const char* spritePath[3], TD::HUBWindow* hubWindow, sf::Rect<float> rect, std::function<bool()> action, unsigned int hoverAlpha)
{
	TD::HUBButton* button1 = hubWindow->addButton<HUBButton>();
	button1->setRelativePosition(hubWindow->position, sf::Vector2i(int(rect.left), int(rect.top)));

	TD::Sprite& spButton = this->renderer.CreateSprite(*this->renderer.GetTexture(spritePath[0]), sf::Vector2i(button1->relativePosition.x, button1->relativePosition.y), hubWindow->layer + 1);
	spButton.setScale(rect.width / spButton.getGlobalBounds().width, rect.height / spButton.getGlobalBounds().height);

	button1->sprite = &spButton;

	button1->clickedSprite->setTexture(*renderer.GetTexture(spritePath[1]));
	button1->clickedSprite->setPosition(spButton.getPosition());
	button1->clickedSprite->setScale(spButton.getScale());

	button1->hoveredSprite->setTexture(*renderer.GetTexture(spritePath[2]));
	button1->hoveredSprite->setPosition(spButton.getPosition());
	button1->hoveredSprite->setScale(spButton.getScale());
	button1->hoveredSprite->setColor(sf::Color::Color(255, 255, 255, hoverAlpha));

	button1->defaultSp->setTexture(*renderer.GetTexture(spritePath[0]));

	button1->action = action;
	return button1;
}

//Main menu
void TD::TowerDefenseGameManager::initMainMenuKey()
{
	TD::KeyBoardEvent eventKeyboard = KeyBoardEvent(sf::Keyboard::Escape, sf::Event::EventType::KeyPressed);
	TD::InputScanner::KeyBoardAction actionKeyboard = [this]() 
	{ 
		this->currentState = GameState::QUIT; 
		this->updateState();
	};
	scanner->addKeyBoardAction(eventKeyboard, actionKeyboard);

	TD::MouseClickEvent eventClick = MouseClickEvent(sf::Mouse::Button::Right, sf::Event::EventType::MouseButtonPressed);
	TD::InputScanner::MouseClickAction actionClick = [this](sf::Mouse::Button button, int x, int y)
	{ 
		bool mapChoosed = false;
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *this->player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y))) {
					button.sprite->setTexture(*button.clickedSprite->getTexture());
					mapChoosed = true;
					if (button.action())
						return;
				}
				else
					button.sprite->setTexture(*button.defaultSp->getTexture());
			}
		if (!mapChoosed)
			this->mapChoosed = "";
	};
	scanner->addMouseAction(eventClick, actionClick);

	TD::InputScanner::MouseMoveAction move = [this](int x, int y)
	{
		bool isOverButton = false;
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *this->player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y)))
				{
					if (button.sprite->getTexture() != button.clickedSprite->getTexture())
						button.sprite->setTexture(*button.hoveredSprite->getTexture());	
					button.sprite->setColor(button.hoveredSprite->getColor());
					this->renderer.getWindow()->changeCursor(TD::Cursor::Action);
					isOverButton = true;
					continue;
				}
				else if (button.sprite->getTexture() != button.clickedSprite->getTexture())
					button.sprite->setTexture(*button.defaultSp->getTexture());
				button.sprite->setColor(button.defaultSp->getColor());
			}
		if (!isOverButton)
			this->renderer.getWindow()->changeCursor(TD::Cursor::Default);
	};
	scanner->addMouseMoveAction(move);
}

void TD::TowerDefenseGameManager::initMainMenuButton()
{
	sf::Vector2f screenSize = (sf::Vector2f)this->renderer.getWindow()->getWindow().getSize();
	//Background
	this->createHub(BACKGROUND_TEXTURES[0], sf::Vector2f(0, 0), (sf::Vector2f)screenSize, 0);
	//logo hub
	this->createHub(BACKGROUND_TEXTURES[1], sf::Vector2f((screenSize.x / 16), (screenSize.y / 8)), sf::Vector2f(screenSize.x / 8, screenSize.y / 8), 1);
	//map menu
	TD::HUBWindow* hub1 = this->createHub(BACKGROUND_TEXTURES[2], sf::Vector2f(screenSize.x / 3, screenSize.y / 6), sf::Vector2f((screenSize.x / 3), ((screenSize.y / 6) * 4)), 1);
	unsigned int textSize = unsigned int(hub1->sprite->getGlobalBounds().height / 10);
	sf::Vector2i pos = hub1->position;
	//Text title
	this->renderer.createText(textSize,
		"Map",
		sf::Color(255, 255, 255),
		sf::Vector2f(float(pos.x + hub1->sprite->getGlobalBounds().width / 10), float(pos.y + textSize))
	);
	//map button
	const char* path[3]
	{
		BUTTON_TEXTURES[0],
		BUTTON_TEXTURES[1],
		BUTTON_TEXTURES[2]
	};
	const char* MAP_TITLE[3]
	{
		"Simple Road",
		"Double Road",
		"Sand storm"
	};
	for (unsigned int i = 0; i < 3; i++)
	{
		std::function<bool()> buttonAction = [this, i]() { this->mapChoosed = "Assets/maps/level" + std::to_string(i + 1) + ".cfg"; return false;};
		TD::HUBButton* button = this->createButton(path, hub1,	sf::Rect<float>(float(hub1->position.x / 10), float((screenSize.y / 8) * (2 + i)), float(hub1->position.x / 16), float(hub1->position.y / 4)), buttonAction);

		this->renderer.createText(	textSize / 2,
									MAP_TITLE[i],
									sf::Color(0, 0, 0),
									sf::Vector2f(button->sprite->getPosition().x + hub1->sprite->getGlobalBounds().width / 10, button->sprite->getPosition().y)
								);
	}
	//start button
	const char* path2[3]
	{
		BUTTON_TEXTURES[3],
		BUTTON_TEXTURES[3],
		BUTTON_TEXTURES[3]
	};
	std::function<bool()> buttonAction = [this]() 
	{ 
		if (this->mapChoosed.empty())
			return false;

		this->currentState = TD::GameState::RUNNING;
		this->updateState();
		return true;
	};
	TD::HUBButton* button = this->createButton(path2, hub1, sf::Rect<float>(float(hub1->position.x / 2 - hub1->position.x / 8), float((screenSize.y / 8) * 4.5), float(hub1->position.x / 4), float(hub1->position.y / 4)), buttonAction, 150);
	//Start text
	this->renderer.createText(textSize / 2,
		"Launch",
		sf::Color(0, 0, 0),
		sf::Vector2f(button->sprite->getPosition().x + (button->sprite->getGlobalBounds().width / 20), button->sprite->getPosition().y)
	);
	
}

//Game running
void TD::TowerDefenseGameManager::initGameMenuKey()
{
	//Keyboard
	TD::KeyBoardEvent eventKeyEsc = KeyBoardEvent(sf::Keyboard::Escape, sf::Event::EventType::KeyPressed);
	TD::InputScanner::KeyBoardAction actionKeyboard = [this]() 
	{ 
		this->currentState = TD::GameState::PAUSED;
		if (this->towerToUpgrade != nullptr)
		{
			this->towerToUpgrade->getSprite()->drawShape = false;
			this->towerToUpgrade = nullptr;
		}
		this->updateState();
	};
	scanner->addKeyBoardAction(eventKeyEsc, actionKeyboard);

	TD::KeyBoardEvent eventKeyP = KeyBoardEvent(sf::Keyboard::P, sf::Event::EventType::KeyPressed);
	scanner->addKeyBoardAction(eventKeyP, actionKeyboard);
	
	//Mouse click
	TD::MouseClickEvent eventClick = MouseClickEvent(sf::Mouse::Button::Right, sf::Event::EventType::MouseButtonPressed);
	TD::InputScanner::MouseClickAction actionClick = [this](sf::Mouse::Button button, int x, int y)
	{
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton* button = this->player.hub.windows[i]->buttons[j];
				if (button->sprite->getGlobalBounds().contains(float(x), float(y)))
				{
					button->action();
					return;
				}
			}
		
		std::vector<TD::ITower*>& towers = this->map.getTowers();
		bool hasClickOnTower = false;
		for (unsigned int i = 0; i < towers.size(); i++)
			if (towers[i]->getSprite()->getGlobalBounds().contains(float(x), float(y)))
			{
				hasClickOnTower = true;
				this->towerToUpgrade = towers[i];
				this->towerToUpgrade->getSprite()->drawShape = true;
			}
			else
				towers[i]->getSprite()->drawShape = false;
		if (hasClickOnTower)
			this->renderer.addSprite(*this->upgradeButton->sprite);
		else
		{
			this->renderer.RemoveSprite(*this->upgradeButton->sprite);
			this->towerToUpgrade = nullptr;
		}
	};
	scanner->addMouseAction(eventClick, actionClick);

	//Mouse click release
	TD::MouseClickEvent eventClickRelease = MouseClickEvent(sf::Mouse::Button::Right, sf::Event::EventType::MouseButtonReleased);
	TD::InputScanner::MouseClickAction actionClickRelease = [this](sf::Mouse::Button button, int x, int y) 
	{ 
		if (this->choosedTower == nullptr)
			return;

		if (!this->isValidPosition(this->choosedTower->getSprite()->getGlobalBounds()))
		{
			this->renderer.RemoveSprite(*this->choosedTower->getSprite());
			this->map.removeTower(this->choosedTower);
			this->player.updateMoney(this->choosedTower->price);
			delete this->choosedTower;
			this->choosedTower = nullptr;
			return;
		}

		this->choosedTower->getSprite()->setOrigin(this->choosedTower->getSprite()->getLocalBounds().width / 2,
													this->choosedTower->getSprite()->getLocalBounds().height / 2);
		this->choosedTower->getSprite()->move(this->choosedTower->getSprite()->getGlobalBounds().width / 2,
												this->choosedTower->getSprite()->getGlobalBounds().height / 2);
		
		this->choosedTower->getSprite()->drawShape = false;
		this->choosedTower->activate();
		this->choosedTower = nullptr;
	};
	scanner->addMouseAction(eventClickRelease, actionClickRelease);

	//Mouse movement
	TD::InputScanner::MouseMoveAction move = [this](int x, int y)
	{
		bool isOverButton = false;
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *this->player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y)) && this->choosedTower == nullptr)
				{
					if (&button == this->upgradeButton && this->towerToUpgrade == nullptr)
						continue;

					button.sprite->setColor(button.hoveredSprite->getColor());
					isOverButton = true;
				}
				else
					button.sprite->setColor(button.defaultSp->getColor());
			}

		std::vector<TD::ITower*>& towers = this->map.getTowers();
		for (unsigned int i = 0; i < towers.size(); i++)
			if (towers[i]->getSprite()->getGlobalBounds().contains(float(x), float(y)))
				isOverButton = true;

		if (!isOverButton)
			this->renderer.getWindow()->changeCursor(TD::Cursor::Default);
		else
			this->renderer.getWindow()->changeCursor(TD::Cursor::Action);

		if (this->choosedTower != nullptr)
		{
			sf::Vector2f cellSize = this->map.getCellSize();
			
			sf::Rect<float> spPos = this->choosedTower->getSprite()->getGlobalBounds();
			sf::Rect<float> shapePos = this->choosedTower->getSprite()->shape->getGlobalBounds();
			
			this->choosedTower->getSprite()->setPosition(sf::Vector2f(x - spPos.width / 2, y - spPos.height / 2));
			this->choosedTower->getSprite()->shape->setPosition(spPos.left - (shapePos.width / 2) + spPos.width / 2, 
																spPos.top - (shapePos.height / 2) + spPos.height / 2);
			
			if (!this->isValidPosition(this->choosedTower->getSprite()->getGlobalBounds()))
				this->choosedTower->getSprite()->setColor(sf::Color(255, 0, 0, 150));
			else
				this->choosedTower->getSprite()->setColor(sf::Color(255, 255, 255, 255));
		}
	};
	scanner->addMouseMoveAction(move);
}

void TD::TowerDefenseGameManager::initGameMenuButton()
{
	sf::Vector2f screenSize = (sf::Vector2f)this->renderer.getWindow()->getWindow().getSize();
	sf::Vector2f cellSize = this->map.getCellSize();
	//tower hud
	TD::HUBWindow* hudTower = this->createHub(BACKGROUND_TEXTURES[3], sf::Vector2f(0, 0), sf::Vector2f(screenSize.x, cellSize.y), 0);
	unsigned int textSize = unsigned int(hudTower->sprite->getGlobalBounds().height / 4);
	sf::Vector2i pos = hudTower->position;
	//map button
	for (unsigned int i = 0; i < 3; i++)
	{
		unsigned int xPadding = 25;
		const char* path[3]
		{
			TD::TOWER_PATH[0 + i],
			TD::TOWER_PATH[0 + i],
			TD::TOWER_PATH[0 + i]
		};
		std::function<bool()> buttonAction = [this, i, path, xPadding, cellSize]() 
		{ 
			sf::Vector2i cursorPos = sf::Mouse::getPosition(this->renderer.getWindow()->getWindow());
			
			TD::TowerType type = static_cast<TowerType>(i);
			if (type == TowerType::Regular)
				this->choosedTower = this->map.addTower<TD::RegularTower>();
			else if (type == TowerType::Explosive)
				this->choosedTower = this->map.addTower<TD::ExplosiveTower>();
			else if (type == TowerType::Stun)
				this->choosedTower = this->map.addTower<TD::StunTower>();

			if (this->player.money < this->choosedTower->price)
			{
				this->map.removeTower(this->choosedTower);
				this->choosedTower = nullptr;
				return false;
			}
			this->player.updateMoney(0 - (this->choosedTower->price));

			TD::Sprite& sp = this->renderer.CreateSprite(
				*this->renderer.GetTexture(path[0]),
				sf::Vector2i(int(cursorPos.x - ((cellSize.x / 2) * 0.6)), int(cursorPos.y - ((cellSize.y / 2) * 0.6))),
				1
			);

			this->choosedTower->setSprite(&sp);
			
			this->choosedTower->initRadius();
			sf::Rect<float> pos = this->choosedTower->getSprite()->shape->getGlobalBounds();
			sf::Vector2i cursor = sf::Mouse::getPosition(this->renderer.getWindow()->getWindow());
			this->choosedTower->getSprite()->shape->setPosition(cursor.x - (pos.width / 2), cursor.y - (pos.height / 2));
			this->choosedTower->getSprite()->drawShape = true;

			this->choosedTower->getSprite()->setScale(cellSize.x / this->choosedTower->getSprite()->getGlobalBounds().width * 0.6f, cellSize.y / this->choosedTower->getSprite()->getGlobalBounds().height * 0.6f);
			return false;
		};
		this->createButton(path, hudTower, sf::Rect<float>(xPadding + (cellSize.x * i) + (xPadding * i), 0, cellSize.x, cellSize.y), buttonAction, 150);
	}
	//Pause button
	const char* path[3]
	{
		BUTTON_TEXTURES[6],
		BUTTON_TEXTURES[6],
		BUTTON_TEXTURES[6]
	};
	std::function<bool()> buttonAction = [this]()
	{
		this->currentState = TD::GameState::PAUSED;
		if (this->towerToUpgrade != nullptr)
		{
			this->towerToUpgrade->getSprite()->drawShape = false;
			this->towerToUpgrade = nullptr;
		}
		this->updateState();
		return false;
	};
	this->createButton(path, hudTower, sf::Rect<float>(hudTower->sprite->getGlobalBounds().width / 2 - cellSize.x / 2,
														0,
														cellSize.x,
														cellSize.y),
														buttonAction, 150);
	//Upgrade button
	const char* path_but[3]
	{
		BUTTON_TEXTURES[7],
		BUTTON_TEXTURES[7],
		BUTTON_TEXTURES[7]
	};
	std::function<bool()> buttonUpgrade = [this]()
	{
		if (this->towerToUpgrade != nullptr)
			this->towerToUpgrade->levelUp(this->renderer, this->player);
		return false;
	};
	this->upgradeButton = this->createButton(path_but, hudTower, sf::Rect<float>(hudTower->sprite->getGlobalBounds().width / 4 - cellSize.x / 8,
																					0,
																					cellSize.x,
																					cellSize.y),
																					buttonUpgrade, 150);
	this->renderer.RemoveSprite(*this->upgradeButton->sprite);

	//Life text
	sf::Text* lifeText = this->renderer.createText(textSize,
		std::to_string(this->player.baseHealth) + " HP",
		sf::Color(255, 0, 0),
		sf::Vector2f(hudTower->sprite->getGlobalBounds().width / 2.0f + hudTower->sprite->getGlobalBounds().width / 4.0f, pos.y + textSize / 2.0f)
	);

	this->player.setLifeText(lifeText);
	//Cash text
	sf::Text* cashText = this->renderer.createText(textSize,
		std::to_string(this->player.money) + " $",
		sf::Color(0, 255, 0),
		sf::Vector2f(hudTower->sprite->getGlobalBounds().width / 2.0f + hudTower->sprite->getGlobalBounds().width / 4.0f, pos.y + textSize * 2.0f)
	);
	this->player.setCashText(cashText);
	//Wave text
	sf::Text* waveText = this->renderer.createText(textSize,
		"Wave " + std::to_string(this->enemyArmy.getWave()),
		sf::Color(0, 0, 0),
		sf::Vector2f(hudTower->sprite->getGlobalBounds().width - hudTower->sprite->getGlobalBounds().width / 8, lifeText->getGlobalBounds().top)
	);
	this->enemyArmy.setWaveText(waveText);
	//Score text
	sf::Text* scoreText = this->renderer.createText(textSize,
		std::to_string(this->player.score) + " Points",
		sf::Color(0, 0, 255),
		sf::Vector2f(hudTower->sprite->getGlobalBounds().width - hudTower->sprite->getGlobalBounds().width / 8, cashText->getGlobalBounds().top)
	);
	this->player.setScoreText(scoreText);
	//Warmup text
	this->warmupText = this->renderer.createText(textSize * 2,
		"WARM UP",
		sf::Color(255, 0, 255),
		sf::Vector2f(hudTower->sprite->getGlobalBounds().width / 2, hudTower->sprite->getGlobalBounds().height + textSize)
	);

	this->warmupText->move(0 - this->warmupText->getGlobalBounds().width / 2, 0);
}

//Pause menu
void TD::TowerDefenseGameManager::initPauseMenuKey()
{
	TD::KeyBoardEvent eventKeyESC = KeyBoardEvent(sf::Keyboard::Escape, sf::Event::EventType::KeyPressed);
	TD::InputScanner::KeyBoardAction actionKeyboard = [this]()
	{
		this->currentState = TD::GameState::RUNNING;
		this->updateState();
	};
	scanner->addKeyBoardAction(eventKeyESC, actionKeyboard);

	TD::KeyBoardEvent eventKeyP = KeyBoardEvent(sf::Keyboard::P, sf::Event::EventType::KeyPressed);
	scanner->addKeyBoardAction(eventKeyP, actionKeyboard);

	TD::MouseClickEvent eventClick = MouseClickEvent(sf::Mouse::Button::Right, sf::Event::EventType::MouseButtonPressed);
	TD::Player& player = this->player;
	TD::InputScanner::MouseClickAction actionClick = [&player](sf::Mouse::Button button, int x, int y)
	{
		for (unsigned int i = 0; i < player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y))) {
					button.sprite->setTexture(*button.clickedSprite->getTexture());
					if (button.action())
						return;
				}
				else
					button.sprite->setTexture(*button.defaultSp->getTexture());
			}
	};
	scanner->addMouseAction(eventClick, actionClick);

	TD::InputScanner::MouseMoveAction move = [this](int x, int y)
	{
		bool isOverButton = false;
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *this->player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y)))
				{
					button.sprite->setColor(button.hoveredSprite->getColor());
					this->renderer.getWindow()->changeCursor(TD::Cursor::Action);
					isOverButton = true;
				}
				else
					button.sprite->setColor(button.defaultSp->getColor());
			}
		if (!isOverButton)
			this->renderer.getWindow()->changeCursor(TD::Cursor::Default);
	};
	scanner->addMouseMoveAction(move);
}

void TD::TowerDefenseGameManager::initPauseMenuButton()
{
	sf::Vector2f screenSize = (sf::Vector2f)this->renderer.getWindow()->getWindow().getSize();
	//Background
	this->createHub(BACKGROUND_TEXTURES[0], sf::Vector2f(0, 0), (sf::Vector2f)screenSize, 0);
	//logo hub
	this->createHub(BACKGROUND_TEXTURES[1], sf::Vector2f((screenSize.x / 16), (screenSize.y / 8)), sf::Vector2f(screenSize.x / 8, screenSize.y / 8), 1);
	//map menu
	TD::HUBWindow* hub1 = this->createHub(BACKGROUND_TEXTURES[2], sf::Vector2f(screenSize.x / 3, screenSize.y / 6), sf::Vector2f((screenSize.x / 3), ((screenSize.y / 6) * 4)), 1);
	unsigned int textSize = unsigned int(hub1->sprite->getGlobalBounds().height / 10);
	sf::Vector2i pos = hub1->position;
	//Text title
	this->renderer.createText(textSize,
		"Pause",
		sf::Color(255, 255, 255),
		sf::Vector2f(pos.x + hub1->sprite->getGlobalBounds().width / 10.0f, float(pos.y + textSize))
	);
	//Continue button
	const char* path[3]
	{
		BUTTON_TEXTURES[3],
		BUTTON_TEXTURES[3],
		BUTTON_TEXTURES[3]
	};
	std::function<bool()> buttonContinueAction = [this]()
	{
		this->currentState = TD::GameState::RUNNING;
		this->updateState();
		return true;
	};
	TD::HUBButton* buttonContinue = this->createButton(path, hub1,	sf::Rect<float>(hub1->sprite->getGlobalBounds().width / 2.0f - hub1->position.x / 6.0f,
																			hub1->sprite->getGlobalBounds().height / 2.0f - hub1->sprite->getGlobalBounds().width / 6.0f,
																			hub1->position.x / 3.0f,
																			hub1->position.y / 4.0f),
																			buttonContinueAction, 150);
	//Continue text
	this->renderer.createText(textSize / 2,
		"Continue",
		sf::Color(0, 0, 0),
		sf::Vector2f(buttonContinue->sprite->getPosition().x + (buttonContinue->sprite->getGlobalBounds().width / 10.0f), float(buttonContinue->sprite->getPosition().y))
	);

	//Restart button
	const char* path2[3]
	{
		BUTTON_TEXTURES[4],
		BUTTON_TEXTURES[4],
		BUTTON_TEXTURES[4]
	};
	std::function<bool()> buttonRestartAction = [this]()
	{
		this->currentState = TD::GameState::RUNNING;
		this->map.reset();
		this->enemyArmy.reset();
		this->player.reset();
		this->updateState();
		return true;
	};
	TD::HUBButton* buttonRestart = this->createButton(path2, hub1, sf::Rect<float>(hub1->sprite->getGlobalBounds().width / 2.0f - hub1->position.x / 6.0f,
																			hub1->sprite->getGlobalBounds().height / 2.0f,
																			hub1->position.x / 3.0f,
																			hub1->position.y / 4.0f),
																			buttonRestartAction, 150);
	//Restart text
	this->renderer.createText(textSize / 2,
		"Restart",
		sf::Color(0, 0, 0),
		sf::Vector2f(buttonRestart->sprite->getPosition().x + (buttonRestart->sprite->getGlobalBounds().width / 6.0f), float(buttonRestart->sprite->getPosition().y))
	);

	//Main menu button
	const char* path3[3]
	{
		BUTTON_TEXTURES[5],
		BUTTON_TEXTURES[5],
		BUTTON_TEXTURES[5]
	};
	std::function<bool()> buttonQuitAction = [this]()
	{
		this->currentState = TD::GameState::MAIN_MENU;
		this->updateState();
		return true;
	};
	TD::HUBButton* buttonQuit = this->createButton(path3, hub1, sf::Rect<float>(hub1->sprite->getGlobalBounds().width / 2 - hub1->position.x / 6.0f,
																			hub1->sprite->getGlobalBounds().height / 2 + hub1->sprite->getGlobalBounds().width / 6.0f,
																			hub1->position.x / 3.0f,
																			hub1->position.y / 4.0f),
																			buttonQuitAction, 150);
	//Quit text
	this->renderer.createText(textSize / 2,
		"Main",
		sf::Color(0, 0, 0),
		sf::Vector2f(buttonQuit->sprite->getPosition().x + (buttonQuit->sprite->getGlobalBounds().width / 4.0f), float(buttonQuit->sprite->getPosition().y))
	);
}

//Game over menu
void TD::TowerDefenseGameManager::initGameOverMenuKey()
{
	TD::KeyBoardEvent eventKeyESC = KeyBoardEvent(sf::Keyboard::Escape, sf::Event::EventType::KeyPressed);
	TD::InputScanner::KeyBoardAction actionKeyboard = [this]()
	{
		this->currentState = TD::GameState::MAIN_MENU;
		this->updateState();
	};
	scanner->addKeyBoardAction(eventKeyESC, actionKeyboard);

	TD::KeyBoardEvent eventKeySpace = KeyBoardEvent(sf::Keyboard::Space, sf::Event::EventType::KeyPressed);
	scanner->addKeyBoardAction(eventKeySpace, actionKeyboard);


	TD::MouseClickEvent eventClick = MouseClickEvent(sf::Mouse::Button::Right, sf::Event::EventType::MouseButtonPressed);
	TD::InputScanner::MouseClickAction actionClick = [this](sf::Mouse::Button button, int x, int y)
	{
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *this->player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y))) {
					button.sprite->setTexture(*button.clickedSprite->getTexture());
					if (button.action())
						return;
				}
				else
					button.sprite->setTexture(*button.defaultSp->getTexture());
			}
	};
	scanner->addMouseAction(eventClick, actionClick);

	TD::InputScanner::MouseMoveAction move = [this](int x, int y)
	{
		bool isOverButton = false;
		for (unsigned int i = 0; i < this->player.hub.windows.size(); i++)
			for (unsigned int j = 0; j < this->player.hub.windows[i]->buttons.size(); j++)
			{
				TD::HUBButton& button = *this->player.hub.windows[i]->buttons[j];
				if (button.sprite->getGlobalBounds().contains(float(x), float(y)))
				{
					if (button.sprite->getTexture() != button.clickedSprite->getTexture())
						button.sprite->setTexture(*button.hoveredSprite->getTexture());
					button.sprite->setColor(button.hoveredSprite->getColor());
					this->renderer.getWindow()->changeCursor(TD::Cursor::Action);
					isOverButton = true;
					continue;
				}
				else if (button.sprite->getTexture() != button.clickedSprite->getTexture())
					button.sprite->setTexture(*button.defaultSp->getTexture());
				button.sprite->setColor(button.defaultSp->getColor());
			}
		if (!isOverButton)
			this->renderer.getWindow()->changeCursor(TD::Cursor::Default);
	};
	scanner->addMouseMoveAction(move);
}

void TD::TowerDefenseGameManager::initGameOverMenuButton()
{
	sf::Vector2f screenSize = (sf::Vector2f)this->renderer.getWindow()->getWindow().getSize();
	//Background
	this->createHub(BACKGROUND_TEXTURES[0], sf::Vector2f(0, 0), (sf::Vector2f)screenSize, 0);
	//logo hub
	this->createHub(BACKGROUND_TEXTURES[1], sf::Vector2f((screenSize.x / 16), (screenSize.y / 8)), sf::Vector2f(screenSize.x / 8, screenSize.y / 8), 1);
	//map menu
	TD::HUBWindow* hub1 = this->createHub(BACKGROUND_TEXTURES[2], sf::Vector2f(screenSize.x / 3, screenSize.y / 6), sf::Vector2f((screenSize.x / 3), ((screenSize.y / 6) * 4)), 1);
	unsigned int textSize = unsigned int(hub1->sprite->getGlobalBounds().height / 10);
	sf::Vector2i pos = hub1->position;
	//Text title
	this->renderer.createText(textSize / 2,
		"Game Over",
		sf::Color(255, 255, 255),
		sf::Vector2f(pos.x + hub1->sprite->getGlobalBounds().width / 10.0f, float(pos.y + textSize))
	);
	//Wave text
	this->renderer.createText(	textSize / 2,
								"Wave: " + std::to_string(this->enemyArmy.getWave()),
								sf::Color(0, 0, 0),
								sf::Vector2f(	hub1->sprite->getGlobalBounds().left + hub1->sprite->getGlobalBounds().width / 2.0f - hub1->position.x / 5.0f, 
												hub1->sprite->getGlobalBounds().top + hub1->sprite->getGlobalBounds().height / 3.0f));
	//Score text
	this->renderer.createText(	textSize / 2,
								"Score: " + std::to_string(this->player.score) + " Points",
								sf::Color(0, 0, 0),
								sf::Vector2f(	hub1->sprite->getGlobalBounds().left + hub1->sprite->getGlobalBounds().width / 2.0f - hub1->position.x / 5.0f,
												hub1->sprite->getGlobalBounds().top + hub1->sprite->getGlobalBounds().height / 2.5f));

	//Restart button
	const char* path[3]
	{
		BUTTON_TEXTURES[3],
		BUTTON_TEXTURES[3],
		BUTTON_TEXTURES[3]
	};
	std::function<bool()> buttonRestartAction = [this]()
	{
		this->currentState = TD::GameState::RUNNING;
		this->map.reset();
		this->enemyArmy.reset();
		this->player.reset();
		this->updateState();
		return true;
	};
	TD::HUBButton* buttonRestart = this->createButton(path, hub1, sf::Rect<float>(hub1->sprite->getGlobalBounds().width / 2.0f - hub1->position.x / 6.0f,
																					hub1->sprite->getGlobalBounds().height / 2.0f,
																					hub1->position.x / 3.0f,
																					hub1->position.y / 4.0f),
																					buttonRestartAction, 150);
	//Restart text
	this->renderer.createText(textSize / 2,
		"Restart",
		sf::Color(0, 0, 0),
		sf::Vector2f(buttonRestart->sprite->getPosition().x + (buttonRestart->sprite->getGlobalBounds().width / 6), buttonRestart->sprite->getPosition().y)
	);
	//Main menu button
	const char* path2[3]
	{
		BUTTON_TEXTURES[5],
		BUTTON_TEXTURES[5],
		BUTTON_TEXTURES[5]
	};
	std::function<bool()> buttonQuitAction = [this]()
	{
		this->currentState = TD::GameState::MAIN_MENU;
		this->updateState();
		return true;
	};
	TD::HUBButton* buttonQuit = this->createButton(path2, hub1, sf::Rect<float>(hub1->sprite->getGlobalBounds().width / 2.0f - hub1->position.x / 6.0f,
																				hub1->sprite->getGlobalBounds().height / 2.0f + hub1->sprite->getGlobalBounds().width / 6.0f,
																				hub1->position.x / 3.0f,
																				hub1->position.y / 4.0f),
																				buttonQuitAction, 150);
	//Quit text
	this->renderer.createText(textSize / 2,
		"Main",
		sf::Color(0, 0, 0),
		sf::Vector2f(buttonQuit->sprite->getPosition().x + (buttonQuit->sprite->getGlobalBounds().width / 4), buttonQuit->sprite->getPosition().y)
	);

	if (this->enemyArmy.getArmy().size() == 0)
	{
		//Won text
		sf::Text* text = this->renderer.createText(textSize / 2,
			"You Won",
			sf::Color(0, 200, 0),
			sf::Vector2f(buttonQuit->sprite->getPosition().x + (buttonQuit->sprite->getGlobalBounds().width / 4), buttonQuit->sprite->getPosition().y + textSize)
		);
		text->move(0 - text->getGlobalBounds().width / 4, 0);
	}
	else
	{
		//Won text
		sf::Text* text = this->renderer.createText(textSize / 2,
			"You Lost",
			sf::Color(200, 0, 0),
			sf::Vector2f(buttonQuit->sprite->getPosition().x + (buttonQuit->sprite->getGlobalBounds().width / 4), buttonQuit->sprite->getPosition().y + textSize)
		);
		text->move(0 - text->getGlobalBounds().width / 4, 0);
	}
}

TD::TowerDefenseGameManager::~TowerDefenseGameManager()
{
	this->towerToUpgrade = nullptr;
	if (this->upgradeButton != nullptr)
		this->renderer.RemoveSprite(*this->upgradeButton->sprite);
}
