#include "DisplayHandler.h"

//--- Static Variables ---//
DisplayHandler* DisplayHandler::inst = nullptr;



//--- Constructor and Destructor ---//
DisplayHandler::DisplayHandler()
{
	//Init the private data
	hasBeenInit = false;
	windowSize = Size(0.0f, 0.0f);
}

DisplayHandler::~DisplayHandler()
{
	//Delete the singleton instance
	if (inst)
		delete inst;

	//Clean up the pointer
	inst = nullptr;
}



//--- Setters and Getters ---//
//Window Size
Size DisplayHandler::getWindowSize() const
{
	//Return the window size in pixels. Use .width and .height to get the dimensions
	return windowSize;
}

Vec2 DisplayHandler::getWindowSizeAsVec2() const
{
	//Return the window size in pixels. Use .x and .y to get the dimensions
	return Vec2(windowSize.width, windowSize.height);
}



//--- Methods ---//
void DisplayHandler::init(float windowWidth, float windowHeight, const std::string windowTitle, bool useFullscreen, float windowScaleFactor)
{
	//Initialize the display if it hasn't already been. Create a window with the given dimensions and title. If it has already been init, output a warning message indicating improper usage
	if (!hasBeenInit)
	{
		//Get the director singleton instance
		auto director = Director::getInstance();

		//Get the OpenGL view object from the director
		auto glview = director->getOpenGLView();

		//If the gl view hasn't already been set, we'll set it
		if (!glview)
		{
			//Create the window using the correct Cocos2D function such that the window size are all synced. Use the fullscreen version if requested
			if (useFullscreen)
				glview = GLViewImpl::createWithFullScreen(windowTitle);
			else
				glview = GLViewImpl::createWithRect(windowTitle, Rect(0.0f, 0.0f, windowWidth, windowHeight), windowScaleFactor, false);

			//Pass the director singleton the new opengl window
			director->setOpenGLView(glview);

			//Store the window size in the class so it is available externally
			windowSize = glview->getVisibleSize();

			//If everything worked, set the flag so the display is never re-init
			hasBeenInit = true;
		}
	}
	else //If the display has already been init
	{
		//Since the display has already been initialized, output a warning message. Since it doesn't re-init, it is not a problem but calling this function more than once is a waste and it is improper
		std::cout << "WARNING: The init() function for the display handler has been called more than once. It should only be called once!" << std::endl;
	}
}

void DisplayHandler::createDebugConsole(bool createInReleaseMode)
{
#if _DEBUG

	//If in debug mode, always create the console
	openConsoleWindow();

#else
	
	//If in release mode, only create the console if the override flag is true in the parameters
	if (createInReleaseMode)
		openConsoleWindow();

#endif
}



//--- Singleton Instance ---//
DisplayHandler* DisplayHandler::getInstance()
{
	//Create the singleton instance if it hasn't already been created
	if (!inst)
		inst = new DisplayHandler();

	//Return the singleton instance
	return inst;
}



//--- Utility Functions ---//
void DisplayHandler::openConsoleWindow()
{
	//Create the console window
	AllocConsole();

	//Bind the window so that outputs go to it
	freopen("CONOUT$", "w", stdout);
}