#include "pch.h"
#include "ScreenManager.h"

namespace Battleship
{
	ScreenManager::ScreenManager() {}

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
		if (!this->requests.empty())
		{
			ScreenManagerRequest& request = requests.front();
			if (request.remove && !this->screens.empty())
			{
				delete this->screens.top();
				this->screens.pop();
			}
			if (request.add)
			{
				if (request.replace && !this->screens.empty())
				{
					delete this->screens.top();
					this->screens.pop();
				}
				this->screens.push(request.newScreenPtr);
			}
			screens.top()->restartClock();
			requests.pop();
		}
	}

	void ScreenManager::removeScreen()
	{
		this->requests.push(ScreenManagerRequest(nullptr, false, true, false));
	}

	void ScreenManager::addScreen(Screen* screen, bool replaceCurrent)
	{
		this->requests.push(ScreenManagerRequest(screen, replaceCurrent, false, true));
	}

	Screen& ScreenManager::getCurrentScreen()
	{
		return *this->screens.top();
	}
}