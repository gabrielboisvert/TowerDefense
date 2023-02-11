#include "pch.h"
#include "App.h"
#include "SFML/Window/Event.hpp"
#include "ConfigTower.h"
#include "Bullet.h"


void TD::App::init()
{   
    //Enable gl before loading texture
    this->m_window.init();
    
    //Load texture
    Cell::initCellImg(this->m_gameManager.getRenderer());
    HUBButton::initTexture(this->m_gameManager.getRenderer());
    HUBWindow::initBackgroundImg(this->m_gameManager.getRenderer());
    ConfigTower::initTowerImg(this->m_gameManager.getRenderer());
    EnemyConfig::initTexture(this->m_gameManager.getRenderer());
    Bullet::initTexture(this->m_gameManager.getRenderer());

    this->m_inputeScanner.init(&this->m_window.getWindow());
    
    this->m_gameManager.setWindow(this->m_window);
    this->m_gameManager.setInputeScanner(this->m_inputeScanner);
    this->m_gameManager.updateState();
}

void TD::App::run()
{
    while (this->m_window.isOpen())
    {
        this->m_inputeScanner.update();
        
        this->m_gameManager.update();
        
        this->m_window.clear();

        this->m_gameManager.render();
        
        this->m_window.display();
    }

    //since i cant delete texture in destructor...
    this->m_gameManager.getRenderer().freeTexture();
}