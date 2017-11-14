#pragma once

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
public:
	//--- Constructors and Destructors ---//
	AppDelegate();
	virtual ~AppDelegate();

	//--- Virtual Methods ---//
	virtual bool applicationDidFinishLaunching(); //The main initialization function. This is called when the game is just starting up
	virtual void applicationDidEnterBackground(); //Called when the game is minimized or put into the background
	virtual void applicationWillEnterForeground(); //Called when the game is re-enabled after being minimized
};

