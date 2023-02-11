#include "pch.h"
#include "HUBWindow.h"


void TD::HUBWindow::clear()
{
	for (unsigned int i = 0; i < this->buttons.size(); i++)
		delete this->buttons[i];
	this->buttons.clear();
}

TD::HUBWindow::~HUBWindow() 
{
	for (unsigned int i = 0; i < this->buttons.size(); i++)
		delete this->buttons[i];
}

void TD::HUBWindow::initBackgroundImg(TD::Renderer& renderer)
{
	for (unsigned int i = 0; i < 4; i++)
		renderer.LoadTextureFile(BACKGROUND_TEXTURES[i]);
}