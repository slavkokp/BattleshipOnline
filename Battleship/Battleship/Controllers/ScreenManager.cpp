#include "ScreenManager.h"

namespace Battleship
{
	ScreenManager::ScreenManager() : replace(false), remove(false), add(false) {}

	ScreenManager::~ScreenManager()
	{
		while (!screens.empty())
		{
			delete screens.top();
			screens.pop();
		}
	}

	void ScreenManager::handleScreenSwitches()
	{
		if (this->remove && !this->screens.empty())
		{
			delete this->screens.top();
			this->screens.pop();
		}
		this->remove = false;
		if (this->add)
		{
			if (this->replace && !this->screens.empty())
			{
				delete this->screens.top();
				this->screens.pop();
			}
			this->replace = false;
			this->screens.push(newScreenPtr);
			this->add = false;
		}
	}

	void ScreenManager::removeScreen()
	{
		this->remove = true;
	}

	void ScreenManager::addScreen(Screen* screen, bool replaceCurrent)
	{
		this->replace = replaceCurrent;
		this->add = true;
		this->newScreenPtr = screen;
	}

	Screen& ScreenManager::getCurrentScreen()
	{
		return *this->screens.top();
	}
}