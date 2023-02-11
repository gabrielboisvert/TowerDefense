#include "pch.h"
#include "PlayerHUB.h"


void TD::PlayerHUB::clearWindow()
{
	for (unsigned int i = 0; i < this->windows.size(); i++)
	{
		this->windows[i]->clear();
		delete this->windows[i];
	}
	this->windows.clear();
}