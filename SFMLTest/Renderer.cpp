#include "pch.h"
#include "Renderer.h"


TD::Renderer::Renderer()
{
	this->window = nullptr;
	this->font = nullptr;
}

TD::Sprite& TD::Renderer::CreateSprite(const sf::Texture& my_Texture, sf::Vector2i position, Sprite::Layer zLayer)
{
	Sprite* sprite = new Sprite(zLayer);
	sprite->setTexture(my_Texture);
	sprite->setPosition((float)position.x, (float)position.y);

	if (this->sprites.size() <= zLayer) {
		std::list<TD::Sprite*> list;
		this->sprites.push_back(list);
	}
	
	this->sprites[zLayer].push_back(sprite);

	return *sprite;
}

void TD::Renderer::RemoveSprite(TD::Sprite& my_Sprite)
{
	for (unsigned int i = 0; i < this->sprites.size(); i++)
		this->sprites[i].remove(&my_Sprite);
}

void TD::Renderer::removeText(sf::Text& text)
{
	std::vector<sf::Text*>::iterator it = std::find(this->texts.begin(), this->texts.end(), &text);
	if (it != this->texts.end())
		this->texts.erase(it);
}

void TD::Renderer::addSprite(TD::Sprite& my_Sprite, Sprite::Layer zLayer)
{
	if (this->sprites.size() <= zLayer) {
		std::list<TD::Sprite*> list;
		this->sprites.push_back(list);
	}

	this->sprites[zLayer].push_back(&my_Sprite);
}

void TD::Renderer::clear()
{
	for (unsigned int i = 0; i < this->sprites.size(); i++)
		this->sprites[i].clear();
	this->sprites.clear();

	for (unsigned int i = 0; i < this->texts.size(); i++)
		delete this->texts[i];
	this->texts.clear();
}

const sf::Texture* TD::Renderer::LoadTextureFile(const std::string& name)
{
	sf::Texture* texture = new sf::Texture;
	texture->loadFromFile(name);
	texture->setSmooth(false);
	
	this->textures.insert(std::pair<std::string, sf::Texture*>(name, texture));
	return this->textures[name];
}

void TD::Renderer::loadFont(std::string path)
{
	this->font = new sf::Font;
	this->font->loadFromFile(path);
}

sf::Text* TD::Renderer::createText(unsigned int size, std::string str, sf::Color color, sf::Vector2f position)
{
	sf::Text* text = new sf::Text(str, *this->font, size);
	text->setFillColor(color);
	text->setPosition(position);

	this->texts.push_back(text);
	return text;
}

const sf::Texture* TD::Renderer::GetTexture(const std::string& name)
{
	if (this->textures.find(name) == this->textures.end())
		return nullptr;

	return this->textures[name];
}

void TD::Renderer::DrawSprites()
{
	for (unsigned int i = 0; i < this->sprites.size(); i++)
		for (std::list<TD::Sprite*>::iterator it = this->sprites[i].begin(); it != this->sprites[i].end(); it++)
		{
			if (*it == nullptr)
				continue;

			if ((*it)->shape != nullptr && (*it)->drawShape)
				this->window->render(*(*it)->shape);
			this->window->render(**it);
		}

	for (unsigned int i = 0; i < this->texts.size(); i++)
		this->window->render(*this->texts[i]);
}

void TD::Renderer::setWindow(TD::Window& window)
{
	this->window = &window;
}

TD::Window* TD::Renderer::getWindow()
{
	return this->window;
}

void TD::Renderer::freeTexture()
{
	if (this->textures.size() != 0)
	{
		for (std::map<std::string, sf::Texture*>::iterator it = this->textures.begin(); it != this->textures.end(); it++)
			delete it->second;
		this->textures.clear();
	}

	delete this->font;
}



TD::Renderer::~Renderer()
{
	for (unsigned int i = 0; i < this->texts.size(); i++)
		delete this->texts[i];
	this->texts.clear();

	for (unsigned int i = 0; i < this->sprites.size(); i++)
	{
		for (std::list<TD::Sprite*>::iterator it = this->sprites[i].begin(); it != this->sprites[i].end(); it++)
			delete *it;
		this->sprites[i].clear();
	}
	this->sprites.clear();
	

}
