/*
============================================================
	Author:
		- Daniel MacCormick - July 2017
		- Second year UOIT student, not affiliated with Cocos2D

	Input Handler:
		- Simple input handler to wrap up Cocos2D's event based input system
		- Use this to get keyboard and mouse input events
			> Press is only true for the one frame the button is pressed (good for inputting names in textfield)
			> Release is only true for the one frame the button is released (good for general input like jumping -> feels natural to jump when you let the space bar go)
			> The other versions (getKey(), getMouseButton()) are true for every frame the button is held (good for continuous actions like moving a character or shooting a machine gun)
		- Also special 'anyButton' events
			> Same as mouse and keyboard but checks for ANY key and ANY mouse
			> Useful for splash screens and other similar systems where you just want the player to press ANYTHING before they move on

	Usage:
		- You are free to use this class for the case studies and for GDW
		- You are free to edit / overwrite any or all of this class
			> It is simply here to make your life easier
		- You must leave this comment block at the top of the header

	Note:
		- This class uses the Singleton design pattern
			> Do not ever make more than one instance of this class in its current form
			> You don't ever have to call the constructor for this class. Simply start using it and it will build itself
			> There is a macro "INPUTS->" that provides a shortcut for getting the singleton instance
============================================================
*/

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

//3rd Party Libraries
#include "cocos2d.h"

//Namespaces
using namespace cocos2d;

/*
	Input State Enum
	- Used for differentiating the types of input events
	- Used for both the mouse input and keyboard input

	> Idle
		- The key or button has not been touched for multiple frames. In general, this will be the majority of the keys (Never returns true in this state)
	> Pressed
		- The key or button was pressed this EXACT frame. Will switch to held the next frame if the button is still down. (Returns true for get_Pressed() and get_())
	> Released
		- The key or button was released this EXACT frame. Will switch to idle the next frame if the button is still up. (Returns true for get_Released())
	> Held
		- The key or button has been down for multiple frames. (Returns true for get_())
*/
enum InputState
{
	Idle,
	Pressed,
	Released,
	Held
};

//Useful shorthands
#define NUM_MOUSE_BUTTONS (int)cocos2d::EventMouse::MouseButton::BUTTON_8 + 2 //The number of mouse buttons supported by Cocos2D.
#define NUM_KEY_CODES (int)cocos2d::EventKeyboard::KeyCode::KEY_PLAY + 1  //The number of keys supported by Cocos2D.
typedef cocos2d::EventKeyboard::KeyCode KeyCode; //A shortcut for accessing KeyCodes
typedef cocos2d::EventMouse::MouseButton MouseButton; //A shortcut for accessing MouseButtons



/*
	Input Handler Class:
	> Setters
		- Set exit on escape
	> Getters
		- Get the mouse position
		- Get mouse button press / release / hold events
		- Get key press / release / hold events
		- Get any key or button press / release / hold events
	> Methods
		- Init
		- Clear inputs for the next frame
*/
class InputHandler : public Node
{
protected:
	//--- Constructor ---//
	InputHandler(); //The constructor is protected so only one instance of this class can ever exist. This is called the singleton pattern.
	
public:
	//--- Destructor ---/
	~InputHandler();



	//--- Setters ---//
	/*
		Set whether or not pressing the Escape key automatically exits the program

		@param ExitOnEscape -> If true, pressing escape will exit the program. If false, this will not happen. It is TRUE by default
	*/
	void setExitOnEscape(bool exitOnEscape); //Enable / disable exiting the program when escape is pressed. This is defaulted to true. Only set to false if you really want to use escape as a button in game



	//--- Getters ---//
	//Mouse
	/*
		Get the position of the mouse cursor. (0, 0) is the BOTTOM LEFT of the screen. (windowWidth, windowHeight) is the top TOP RIGHT of the screen

		@return Returns -> The position of the mouse cursor as a Vec2. Use position.x and position.y to access the values individually
	*/
	Vec2 getMousePosition() const;

	/*
		Get if a certain mouse button was pressed. This means that the button was pressed this EXACT frame. It will be false on the next frame. Prevents detecting user input for more than a single frame

		@param Button -> The mouse button you want to check. Use MouseButton:: to see the full list then select one. Ex: getMouseButtonPress(MouseButton::BUTTON_LEFT) for left mouse button
		@return Returns -> True if the mouse button was pressed this EXACT frame. False if not
	*/
	bool getMouseButtonPress(MouseButton button) const;

	/*
		Get if a certain mouse button was released. This means that the button was released this EXACT frame. It will be false on the next frame. Prevents detecting user input for more than a single frame

		@param Button -> The mouse button you want to check. Use MouseButton:: to see the full list then select one. Ex: getMouseButtonRelease(MouseButton::BUTTON_LEFT) for left mouse button
		@return Returns -> True if the mouse button was released this EXACT frame. False if not
	*/
	bool getMouseButtonRelease(MouseButton button) const;

	/*
		Get if a certain mouse button is currently down. This is true for EVERY frame the button is down. It will not switch to false the next frame if the button is still down.

		@param Button -> The mouse button you want to check. Use MouseButton:: to see the full list then select one. Ex: getMouseButton(MouseButton::BUTTON_LEFT) for left mouse button
		@return Returns -> True if the mouse button is currently down. False if not.
	*/
	bool getMouseButton(MouseButton button) const;

	/*
		Get if the user is scrolling with the mouse wheel. The magnitude of the value represents how fast they are scrolling and the sign is the direction. Postive values means scrolling up, negative means down

		@return Returns -> A float representing the scroll amount. Magnitude is the speed, sign is the direction. 1 is slowest scroll, 5 is fastest. +ve values are up, -ve values are down
	*/
	float getMouseScroll() const; 

	/*
		Rarely used but get if the user is scrolling HORIZONTALLY with the mouse wheel. Very rarely supported with computer mice.

		@return Returns -> A float representing the horizontal scroll amount
	*/
	float getHorizontalMouseScroll() const;


	//Keyboard
	/*
		Get if a certain key was pressed. This means that the key was pressed this EXACT frame. It will be false on the next frame. Prevents detecting user input for more than a single frame

		@param Key -> The key you want to check. Use KeyCode:: to see the full list then select one. Ex: getKeyPress(KeyCode::KEY_SPACE) for space bar
		@return Returns -> True if the key was pressed this EXACT frame. False if not
	*/
	bool getKeyPress(KeyCode key) const;

	/*
		Get if a certain key was released. This means that the key was released this EXACT frame. It will be false on the next frame. Prevents detecting user input for more than a single frame

		@param Key -> The key you want to check. Use KeyCode:: to see the full list then select one. Ex: getKeyRelease(KeyCode::KEY_SPACE) for space bar
		@return Returns -> True if the key was released this EXACT frame. False if not
	*/
	bool getKeyRelease(KeyCode key) const;

	/*
		Get if a certain key is currently down. This is true for EVERY frame the button is down. It will not switch to false on the next frame if the button is still held down

		@param Key -> The key you want to check. Use KeyCode:: to see the full list then select one. Ex: getKey(KeyCode::KEY_SPACE) for space bar
		@return Returns -> True if the key is currently down. False if not
	*/
	bool getKey(KeyCode key) const;


	//Any.
	/*
		Get if a ANY KEY OR ANY MOUSE BUTTON was pressed. This means that the key or button was pressed this EXACT frame

		@return Returns -> True if ANY KEY OR ANY MOUSE BUTTON was pressed this frame. Useful for splash screens or loading screens
	*/
	bool getAnyButtonPress() const; 

	/*
		Get if a ANY KEY OR ANY MOUSE BUTTON was released. This means that the key or button was pressed this EXACT frame

		@return Returns -> True if ANY KEY OR ANY MOUSE BUTTON was released this frame. Useful for splash screens or loading screens
	*/
	bool getAnyButtonRelease() const;

	/*
		Get if a ANY KEY OR ANY MOUSE BUTTON is down. This means that ANY key is currently being interacted with by the user

		@return Returns -> True if ANY KEY OR ANY MOUSE BUTTON is down. False if there is absolutely no input from the user
	*/
	bool getAnyButton() const;



	//--- Methods ---//
	/*
		This HAS to be called ONCE! If not, no inputs will EVER be read. Sets up the input handling events so it is ready to accept inputs.

		@return Returns -> True if the init succeeded. False if not. Use to ensure everything was set up properly and throw and error if not.
	*/
	bool init();

	/*
		This HAS to be called EVERY FRAME at the END OF THE FRAME! If not, the inputs won't be synced to the current frame! Gets ready for the next frame of input handling.
	*/
	void clearForNextFrame();



	//--- Singleton Instance ---//
	/*
		Get the instance of the singleton. You shouldn't ever need to call this directly since the macro (INPUTS->) automatically calls it

		@return Returns -> The singleton instance of this class. This is the ONLY instance of this class so intrefacing HAS to be done through this instance.
	*/
	static InputHandler* getInstance(); //Singleton 

private:
	//--- Private Data ---//
	//Cocos Engine
	Size windowDimensions; //The size of the window created at the start of the game. Only used to ensure the mouse position's y-coordinate is flipped properly
	bool exitOnEscape; //If true, the program will exit when escape is pressed. This is the default

	//Mouse
	Vec2 mousePosition; //The current position of the mouse, stored as a Vec2. Updated every time the mouse is moved.
	float scrollValue; //The value for the mouse wheel scrolling on the standard Y-axis (Note: this is the standard up and down scrolling)
	float horizontalScrollValue; //The value for the mouse wheel scrolling on the non-standard X-axis (NOTE: this is NOT up and down scrolling!)
	InputState mouseStates[NUM_MOUSE_BUTTONS]; //States for all of the mouse buttons in cocos2D. +2 since unset is a button as well and is defaulted to -1
	EventListenerMouse* mouseListener; //The listener for the mouse events

	//Keyboard
	InputState keyboardStates[NUM_KEY_CODES]; //States for all of the keycodes in cocos2D
	EventListenerKeyboard* keyboardListener; //The listener for the keyboard events

	//--- Utility Functions ---//
	void initMouseListener(); //Set up the mouse event handling through the listener
	void initKeyboardListener(); //Set up the keyboard event handling through the listener

	//--- Singleton Instance ---//
	static InputHandler* inst; //The singleton instance. Ie: The only instance of this class that can ever exist
};

#define INPUTS InputHandler::getInstance() //Macro to make using the input handler easier. Automatically gets the singleton instance

#endif
