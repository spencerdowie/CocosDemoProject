/*
============================================================
	Author:
		- Daniel MacCormick - July 2017
		- Second year UOIT student, not affiliated with Cocos2D

	Display Handler:
		- Simple class to wrap some of the display / windowing calls from Cocos
		- Call the init() function at the start of the program's execution in order to create a window with the proper dimensions. You should ONLY call this ONCE
		- Call getWindowSize() to get the width and height of the window in pixels. This returns a "Size" object which is Cocos2D's data type. Size has .width and .height

	Usage:
		- You are free to use this class for the case studies and for GDW
		- You are free to edit / overwrite any or all of this class
			> It is simply here to make your life easier
		- You must leave this comment block at the top of the header

	Note:
		- This class uses the Singleton design pattern
			> Do not ever make more than one instance of this class in its current form
			> You don't ever have to call the constructor for this class. Simply start using it and it will build itself
			> There is a macro "DISPLAY->" that provides a shortcut for getting the singleton instance
============================================================
*/

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

//Core Libraries
#include <string>
#include <iostream>

//3rd Party Libraries
#include "cocos2d.h"

//Namespaces
using namespace cocos2d;

/*
	Display Handler Class:
	> Getters
		- Get the size of the window in pixels as 'Size' or as 'Vec2'
	> Methods
		- Init
*/
class DisplayHandler
{
protected:
	//--- Constructor ---//
	DisplayHandler(); //The constructor is protected so only one instance of this class can ever exist. This is called the singleton pattern.

public:
	//--- Destructor ---//
	~DisplayHandler();



	//--- Setters and Getters ---//
	//Window Size
	/*
		Get the size in pixels using a Cocos2D data type called a 'Size'. Use .width and .height to get the information

		@return Returns -> The size of the window as a Size variable. Use .width and .height to extract the information
	*/
	Size getWindowSize() const; 

	/*
		Get the size in pixels using a Cocos2D data type called a 'Vec2'. Use .x and .y to get the information. Useful tip: center objects by calling setPosition(DISPLAY->getWindowSizeAsVec2() / 2.0f)

		@return Returns -> The size of the window as a Vec2 variable. Use .x and .y to extract the information
	*/
	Vec2 getWindowSizeAsVec2() const; 



	//--- Methods ---//
	/*
		This HAS to be called ONCE in the program. This creates and initializes the window. NOTE: if fullscreen is true, the resolution parameters are overwritten by Cocos2D

		@param WindowWidth -> The size of the window horizontally, in pixels. NOTE: This is overwritten by Cocos2D if you use fullscreen
		@param WindowHeight -> The size of the window vertically, in pixels. NOTE: This is overwritten by Cocos2D if you use fullscreen
		@param WindowTitle -> The name of the window. This appears in the bar at the top when not fullscreen.
		@param UseFullscreen -> If true, Cocos2D automatically scales the window to fit the size of your screen
		@param WindowScaleFactor (optional) -> Defaulted to 1.0f. Scales the ENTIRE window. Makes the window take up a larger portion of the screen and also scales the sprites within it to match. NOTE: Ignored if fullscreen
	*/
	void init(float windowWidth, float windowHeight, const std::string windowTitle, bool useFullscreen, float windowScaleFactor = 1.0f);

	/*
		Creates a debug console window. This allows for the use of couts, printfs, logs etc to be viewed in a cmd window. By default, this function ONLY creates a window in debug mode. It can still create a window in release mode if you want though

		@param CreateInReleaseMode (optional) -> This determines if the debug window is created when the game is in release mode. By default, no window is created in release mode, only in debug mode. This parameter overrides that.
	*/
	void createDebugConsole(bool createInReleaseMode = false);



	//--- Singleton Instance ---//
	/*
		Get the instance of the singleton. You shouldn't ever need to call this directly since the macro (DISPLAY->) automatically calls it

		@return Returns -> The singleton instance of this class. This is the ONLY instance of this class so intrefacing HAS to be done through this instance.
	*/
	static DisplayHandler* getInstance();

private:
	//--- Private Class Data ---//
	Size windowSize; //Size (in pixels) of the window. .width and .height can be used to get the information within
	bool hasBeenInit; //Prevents the display from being init more than once

	//--- Singleton Instance ---//
	static DisplayHandler* inst; //The singleton instance of this class. Ie: the only instance that can ever exist

	//--- Utility Functions ---//
	void openConsoleWindow(); //Private function that creates a debug window and binds output to it. Called by createDebugConsole()
};

#define DISPLAY DisplayHandler::getInstance() //Macro to make using the class easier. Automatically gets the singleton instance for you

#endif