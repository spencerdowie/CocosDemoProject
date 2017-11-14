#pragma once

//3rd Party Libraries
#include "cocos2d.h"

//Namespaces
using namespace cocos2d;


class DemoScene : public cocos2d::Scene
{
public:
	//Engine Functions (Only functions supplied by Cocos2D)
	static cocos2d::Scene* createScene(); //The function that actually builds the scene and returns it. Called in AppDelegate.cpp right before director->runWithScene()
	virtual bool init(); //An init function that sets up the values for this class and returns a flag indicating if it succeeded or not. Sort of like the constructor for scenes
	void update(float deltaTime); //A function that is called every frame. This is essentially your game loop
	CREATE_FUNC(DemoScene); //This is a special macro'd function created by Cocos2D. It automatically releases the memory for this scene when it is no longer being used by anything

	//Init Functions (These functions and others are ours, not Cocos2D's)
	void initScene(); //Create the background, the particle system, and the physics here
	void initRestartButton(); //Create the interactable restart button. This button has a label and also a callback function so it is a bit special
	void initSounds(); //Load the sounds we want to use so they don't get loaded the first time they are used

	//Methods
	void spawnSoloObject(); //Spawn a single yellow bird
	void spawnParentAndChildren(); //Spawn a red bird with two child objects. One is a draw node and the other is another sprite
	void nextDebugDraw(); //Switch the setting on the physics debug draw to view the different types available with Cocos2D

	//Menu Callbacks
	void onRestartButtonPress(); //Simple callback function that is called whenever the button in the top right is presseds

private:
	//Engine
	Director* director; //A reference to the director so we don't have to call getInstance() every time we want to use it

	//Following particle system
	ParticleSystem* mouseParticles; //A particle system that is going to follow the mouse cursor every frame. Only thing we need to hold on to so we can explicity control it

	//This scene's physics world
	//Reference to the physics world used within the scene. Prevents having to call: director->getRunningScene()->getPhysicsWorld() every time we want to do something
	//HAS to be static because the create function we set its value in is a static function. The compiler will complain if we try to use a non-static member in a static function
	static PhysicsWorld* physicsWorld; 

	//The current debug draw type
	int debugDrawType; //The current type of debug drawing being used. Default is 0. 0 = none, 1 = contact, 2 = shapes, 3 = all
};

