#include "InputHandler.h"
#include "DisplayHandler.h"

//--- Static Variables ---//
InputHandler* InputHandler::inst = 0;


//--- Constructor and Destructor ---//
InputHandler::InputHandler()
	: Node()
{
	//Init the engine variables
	windowDimensions = DISPLAY->getWindowSize();
	exitOnEscape = true;

	//Init the mouse variables
	mousePosition = Vec2(0.0f, 0.0f);
	scrollValue = 0.0f;
	horizontalScrollValue = 0.0f;
}

InputHandler::~InputHandler()
{
	//Delete the singleton instance
	if (inst)
		delete inst;

	//Clean up the singleton instance pointer
	inst = nullptr;

	//Tell the event dispatcher to clean up the mouse and keyboard event handlers 
	_eventDispatcher->removeAllEventListeners();

	//Clean up the mouse and keyboard listener pointers
	mouseListener = nullptr;
	keyboardListener = nullptr;
}



//--- Setters and Getters ---//
//Engine
void InputHandler::setExitOnEscape(bool _exitOnEscape) 
{
	//Set the flag that determines if the program should exit when escape is pressed. If true, pressing escape will automatically exit the program
	exitOnEscape = _exitOnEscape;
}


//Mouse
Vec2 InputHandler::getMousePosition() const 
{
	//Return the position of the mouse cursor, from the BOTTOM LEFT! Flipped on the y-axis from the value that Cocos returns
	return mousePosition;
}

bool InputHandler::getMouseButtonPress(MouseButton button) const
{
	//If the mouse button requested is set to pressed, it was pressed this exact frame. +1 since the first mouse button is set to -1
	return (mouseStates[(int)button + 1] == InputState::Pressed);
}

bool InputHandler::getMouseButtonRelease(MouseButton button) const
{
	//If the mouse button requested is set to released, it was released this exact frame. +1 since the first mouse button is set to -1
	return (mouseStates[(int)button + 1] == InputState::Released);
}

bool InputHandler::getMouseButton(MouseButton button) const
{
	//If the mouse button requested is set to pressed OR set to held, it is currently down and so should return true
	return (mouseStates[(int)button + 1] == InputState::Pressed || mouseStates[(int)button + 1] == InputState::Held);
}

float InputHandler::getMouseScroll() const
{
	//Return the amount the scroll wheel is being moved on the standard Y-axis (NOTE: this is standard up and down scrolling!)
	//NOTE: Return the negative because positive is DOWN by default instead of UP
	return -scrollValue;
}

float InputHandler::getHorizontalMouseScroll() const
{
	//Return the amount the scroll wheel is being moved on the non-standard X-axis (NOTE: this is NOT standard up and down scrolling!)
	return -horizontalScrollValue;
}


//Keyboard
bool InputHandler::getKeyPress(KeyCode key) const
{
	//If the key requested is set to pressed, it was pressed this exact frame
	return (keyboardStates[(int)key] == InputState::Pressed);
}

bool InputHandler::getKeyRelease(KeyCode key) const
{
	//If the key requested is set to released, it was released this exact frame
	return (keyboardStates[(int)key] == InputState::Released);
}

bool InputHandler::getKey(KeyCode key) const
{
	//If the key requested is set to pressed OR set to held, it is currently down and so should return true
	return (keyboardStates[(int)key] == InputState::Pressed || keyboardStates[(int)key] == InputState::Held);
}


//Any 
bool InputHandler::getAnyButtonPress() const
{
	//Loop through all of the mouse buttons. If any of them are pressed, immediately return true
	for (unsigned int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		//Immediately return true upon finding a single button that was pressed
		if (mouseStates[i] == InputState::Pressed)
			return true;
	}

	//If no mouse buttons were pressed, move to checking the keyboard. If any key was pressed, immediately return true
	for (unsigned int i = 0; i < NUM_KEY_CODES; i++)
	{
		//Immediately return true upon finding a single button that was pressed
		if (keyboardStates[i] == InputState::Pressed)
			return true;
	}

	//Return false if absolutely no buttons were pressed and so it reached this point
	return false;
}

bool InputHandler::getAnyButtonRelease() const
{
	//Loop through all of the mouse buttons. If any of them are released, immediately return true
	for (unsigned int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		//Immediately return true upon finding a single button that was released
		if (mouseStates[i] == InputState::Released)
			return true;
	}

	//If no mouse buttons were pressed, move to checking the keyboard. If any key was released, immediately return true
	for (unsigned int i = 0; i < NUM_KEY_CODES; i++)
	{
		//Immediately return true upon finding a single button that was released
		if (keyboardStates[i] == InputState::Released)
			return true;
	}

	//Return false if absolutely no buttons were released and so it reached this point
	return false;
}

bool InputHandler::getAnyButton() const
{
	//Loop through all of the mouse buttons. If any of them are down, immediately return true
	for (unsigned int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		//Immediately return true upon finding a single button that was released
		if (mouseStates[i] == InputState::Pressed || mouseStates[i] == InputState::Held)
			return true;
	}

	//If no mouse buttons were pressed, move to checking the keyboard. If any key is down, immediately return true
	for (unsigned int i = 0; i < NUM_KEY_CODES; i++)
	{
		//Immediately return true upon finding a single button that was released
		if (keyboardStates[i] == InputState::Released || keyboardStates[i] == InputState::Held)
			return true;
	}

	//Return false if absolutely no buttons are down and so it reached this point
	return false;
}



//--- Methods ---//
bool InputHandler::init()
{
	//Ensure the parent class was init first
	if (!Node::init())
		return false;

	//Set up the mouse callbacks
	initMouseListener();

	//Set up the keyboard callbacks
	initKeyboardListener();

	//Indicate the init worked properly
	return true;
}

void InputHandler::clearForNextFrame()
{
	//Loop through the mouse buttons and update their states. If they were pressed last frame, they are now held. If they were released last frame, they are now idle.
	for (unsigned int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		//Ignore the buttons that are currently not being pressed. This will be true the large majority of the time, saving the two if checks afterwards
		if (mouseStates[i] == InputState::Idle)
			continue;

		//If they button was pressed last frame, it is now considered held. If it was released last frame, is now considered idle
		if (mouseStates[i] == InputState::Pressed)
			mouseStates[i] = InputState::Held;
		else if (mouseStates[i] == InputState::Released)
			mouseStates[i] = InputState::Idle;
	}

	//Loop through the keyboard buttons and update their states. If they were pressed last frame, they are now held. If they were released last frame, they are now idle.
	for (unsigned int i = 0; i < NUM_KEY_CODES; i++)
	{
		//Ignore the keys that are currently not being pressed. This will be true the large majority of the time, saving the two if checks afterwards
		if (keyboardStates[i] == InputState::Idle)
			continue;

		//If they button was pressed last frame, it is now considered held. If it was released last frame, is now considered idle
		if (keyboardStates[i] == InputState::Pressed)
			keyboardStates[i] = InputState::Held;
		else if (keyboardStates[i] == InputState::Released)
			keyboardStates[i] = InputState::Idle;
	}

	//Reset the scroll wheel amounts to 0
	scrollValue = 0.0f;
	horizontalScrollValue = 0.0f;
}



//--- Utility Functions ---//
void InputHandler::initMouseListener()
{
	//Init the mouse listener
	mouseListener = EventListenerMouse::create();

	//On Mouse Down
	mouseListener->onMouseDown = [&](Event* event)
	{
		//Cast the event as a mouse event
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

		//Get the position of the mouse button press
		Vec2 mouseClickPosition = mouseEvent->getLocation();

		//Init the message to be output in the message box
		std::stringstream message;

		//Get the mouse button from the event handler
		MouseButton mouseButton = mouseEvent->getMouseButton();

		//Set the appropriate mouse button to be pressed (+1 to compensate for the enum in Cocos starting at -1)
		mouseStates[(int)mouseButton + 1] = InputState::Pressed;
	};


	//On Mouse Up
	mouseListener->onMouseUp = [&](Event* event)
	{
		//Cast the event as a mouse event
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

		//Get the position of the mouse button press
		Vec2 mouseUpPosition = mouseEvent->getLocation();

		//Init the message to be output in the message box
		std::stringstream message;

		//Get the mouse button from the event handler
		MouseButton mouseButton = mouseEvent->getMouseButton();

		//Set the appropriate mouse button to be released (+1 to compensate for the enum in Cocos starting at -1)
		mouseStates[(int)mouseButton + 1] = InputState::Released;
	};


	//On Mouse Move
	mouseListener->onMouseMove = [&](cocos2d::Event* event)
	{
		//Cast the event as a mouse event
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

		//Get the position of the mouse from the event handler in UI space (ie: the Y axis is flipped since it is from the TOP LEFT instead of the BOTTOM RIGHT)
		Vec2 mouseEventPos = mouseEvent->getLocationInView();

		//Store the cursor position with a FLIPPED Y. To do this, add the height of the window to the position
		mousePosition = Vec2(mouseEventPos.x, mouseEventPos.y + windowDimensions.height);
	};


	//On Mouse Scroll
	mouseListener->onMouseScroll = [&](cocos2d::Event* event)
	{
		//Cast the event as a mouse event
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

		//Get the scroll amounts from the mouse event
		scrollValue = mouseEvent->getScrollY();
		horizontalScrollValue = mouseEvent->getScrollX();
	};


	//Add the mouse listener to the dispatcher
	_eventDispatcher->addEventListenerWithFixedPriority(mouseListener, 1);
}

void InputHandler::initKeyboardListener()
{
	//Create the keyboard listener
	keyboardListener = EventListenerKeyboard::create();


	//On Key Pressed
	keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		//Set the appropriate key to be considered pressed
		keyboardStates[(int)keyCode] = InputState::Pressed;

		//Exit if the escape key was pressed and the flag is set to true
		if (exitOnEscape && keyCode == KeyCode::KEY_ESCAPE)
			Director::getInstance()->end();
	};


	//On Key Released
	keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		//Set the appropriate key to be considered released
		keyboardStates[(int)keyCode] = InputState::Released;
	};


	//Add the keyboard listener to the dispatcher
	_eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 1);
}



//--- Singleton Instance ---//
InputHandler* InputHandler::getInstance()
{
	//Generate the singleton if it hasn't been created yet
	if (!inst)
		inst = new InputHandler();
	
	//Return the singleton
	return inst;
}
