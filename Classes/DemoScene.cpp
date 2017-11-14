#include "DemoScene.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include "AudioEngine.h"
using experimental::AudioEngine;

//Init the static physics world pointer. Set it to be a nullptr which means it points to nothing
PhysicsWorld* DemoScene::physicsWorld = nullptr;

//--- Engine Functions ---//
Scene* DemoScene::createScene()
{
	//Create the actual scene object that gets used with the director. This function is called within AppDelegate.cpp 
	//'scene' is an autorelease object so we never have to call delete on it. If we did, your application would likely crash
	//Important note: Anytime you call ___::create() with Cocos2D, you will be getting an autoreleased object. You do not need to call delete on anything in the Cocos2D engine
	Scene* scene = Scene::createWithPhysics();



	//Create a layer that is going to be attached to the scene
	//This layer is what contains all of our objects since we are working within a DemoScene
	//Also, when we use 'this->' later on, this layer is what is being referred to
	DemoScene* layer = DemoScene::create();



	//Add the layer to the scene
	scene->addChild(layer);



	//Get the physics world from the scene so that we can work with it later
	//If we didn't do this, we would have to call director->getRunningScene()->getPhysicsWorld() every time we wanted to do something to the physics world
	physicsWorld = scene->getPhysicsWorld();



	//Return the newly built scene
	//This is then passed to the director with director->runWithScene() or director->replaceScene() etc. In this case, director->runWithScene() is called in AppDelagate.cpp
	return scene;
}

bool DemoScene::init()
{
	//Ensure the super class's init function was called first. If it wasn't, exit this one since we can't build this object if the super class wasn't built first
	if (!Scene::init())
		return false;



	//Get the director from cocos so we can use it when needed. Prevents having to call Director::getInstance() every time we needed to use the director
	director = Director::getInstance();



	//Create and set up the sprites. This is a function we added. This is not a function supplied by Cocos2D
	initScene();



	//VERY IMPORTANT LINE!
	//Allow for the update() function to be called by Cocos2D
	//Without this line, update() would NEVER be called. 
	//You can optionally add a schedule timing in here to explicity set an order for update calls. We do not need to do that, however
	//*** Forum Post: http://discuss.cocos2d-x.org/t/void-update-float-delta-is-not-executing/16614/4 ***//
	this->scheduleUpdate();



	//Let cocos know that the init function was successful
	return true;
}

void DemoScene::update(float deltaTime)
{
	//Update the mouse particles so they actually follow the mouse
	//If we didn't call this every frame in update(), they would stay where the mouse was on the very first frame of the game
	//We are using the input handler class to get the mouse position as a Vec2 and simply using that directly
	//INPUTS is a macro for InputHandler::getInstance() as InputHandler uses the same design pattern as the Director. This pattern is called the singleton pattern
	mouseParticles->setPosition(INPUTS->getMousePosition());



	//Spawn objects with the left and right mouse buttons
	if (INPUTS->getMouseButtonPress(MouseButton::BUTTON_LEFT))
	{
		//With the left mouse button, we are going to spawn a single bird.
		//This bird has a physics body attached to it so it will fall and collide according to physics
		//*** What happens if you change this to getMouseButton() instead of getMouseButtonPress()? Try it to find out! ***//
		spawnSoloObject();
	}
	else if (INPUTS->getMouseButtonPress(MouseButton::BUTTON_RIGHT))
	{
		//With the right mouse button, we are going to spawn multiple objects. 
		//The 'parent' is a red bird with a physics body, just like with the left mouse button.
		//The 'children' are a blue circle and another smaller blue bird. These children are 'attached' to the parent and so they will move along with it
		//The children can still move independently of the parent but will be dragged along behind it whenever the parent is moving
		spawnParentAndChildren();
	}



	//Mess with Gravity!
	if (INPUTS->getKeyPress(KeyCode::KEY_G))
	{
		//When the user presses the G key, we are making gravity a positive value and so it pulls the objects up instead
		//This is where it is useful that we got the reference to the physics world in the create scene function
		//IMPORTANT NOTE: You might remember gravity is defined as -9.81 m/s/s from physics. Cocos2D's physics engines work on a 10x scale. You can use -9.81 but your forces will have to be dialed down to compensate
		//*** What happens if you use 9.81 instead of 98.1? Try moving the decimal over to find out! ***//
		physicsWorld->setGravity(Vec2(0.0f, 98.1f));
	}
	else if (INPUTS->getKeyRelease(KeyCode::KEY_G))
	{
		//When the user releases the G key, we are resetting gravity to the proper value.
		//This is where it is useful that we got the reference to the physics world in the create scene function
		//IMPORTANT NOTE: You might remember gravity is defined as -9.81 m/s/s from physics. Cocos2D's physics engines work on a 10x scale. You can use -9.81 but your forces will have to be dialed down to compensate
		physicsWorld->setGravity(Vec2(0.0f, -98.1f));
	}



	//Swtich to the next debug drawing mode if the space bar is pressed. getKeyReleased() is used instead of _Pressed() so it happens when the user lets the button go
	if (INPUTS->getKeyRelease(KeyCode::KEY_SPACE))
	{
		//Physics debug drawing is very useful when it comes to trying to fix issues with your level's collision or setup. For example, you might have an invisible collider you didn't know about
		//Coocs2D has 5 different drawing modes:
		//		> None -> Absolutely no debug drawing is displayed. This should be the setting when you are actually showing off your game
		//		> Contact -> Only the collisions and collision response is showed. You might notice small blue lines wherever two physics objects are colliding. This is the force being applied to keep them from overlapping
		//		> Shapes -> This overlays semi-transparent polygons to show where the collision boxes are. You will see circles, rectangles, and perhaps some custom shapes as well
		//		> Joints -> Not visible in this demo scene since no joints are used but shows the physics joints in the scene
		//		> All -> Probably useful to just use this if you want any form of debug drawing. Simply shows everything listed above all at once
		//*** What happens if you use getKey() instead of getKeyRelease()? Try it to find out! Also, what happens when you use getAnyKey() instead? Try that too! ***//
		nextDebugDraw();
	}



	//Reload the scene if the R key is hit by the user
	if (INPUTS->getKeyRelease(KeyCode::KEY_R))
	{
		//Call the function to restart the scene
		//See this function to learn about switching scenes!
		onRestartButtonPress();
	}



	//Update the inputs so they are grabbed from the correct frame
	//This is a VERY IMPORTANT line of code. It ensures the inputs are updated and synced to the right frame
	//*** What happens if you remove this line of code? Try to run this scene without it! Hint: Try spawning birds! ***//
	INPUTS->clearForNextFrame();
}



//--- Init Functions ---//
void DemoScene::initScene()
{
	//Init the debug draw type
	//0 means none. We don't set it to anything by default and so this matches what Cocos2D is rendering as well
	debugDrawType = 0;



	//Create the background sprite
	//Since we are using a Cocos2D create function, we know that this is an autorelease object and so we don't have to call delete on it
	//We are creating a new sprite with an image at the given path
	//The path starts at the 'Resources' folder. All resources (sounds, images, etc) should go into this folder
	//For demonstation purposes, we added subfolders to the Resources folder. To navigate folders, use '/' or '\\'. Windows considers '\' an escape character so if you want to use that, you have to use '\\' instead
	//Cocos2D automatically tracks which images you are using so it won't load in the same image twice. This is great since it prevents a lot of unnecessary memory from being used
	//*** What happens if you change the anchor point? Try setting the anchor point to different positions. Try (0,0), (1, 1) and any others you want. Watch what happens to the background ***//
	//*** What happens if you use a different image? Try using a bird image then try another background image! ***//
	Sprite* spr_Background = Sprite::create("Demo/Background\\spr_Background.jpg"); //Load the handle from the image file
	spr_Background->setPosition(DISPLAY->getWindowSizeAsVec2() / 2.0f); //Center the image in the window. We can get the window size of the display handler singleton and simply divide it by 2 to center the sprite
	spr_Background->setAnchorPoint(Vec2(0.5f, 0.5f)); //Ensure the middle of the background is the anchor point. The anchor point is the point of the sprite you are positioning. (0.5, 0.5) is the center


	//Add the physics body for the ground
	//We are using another Cocos2D create function so this physics body will be automatically removed
	//We are creating a box that spans the width of the screen as the ground collider for our scene
	//Since dynamic is false, the box will not move. It will simply allow other bodies to collide with it
	//We are going to attach it to the background sprite so we are adding a positional offset to it. Otherwise, it would spawn in the center of the scene.
	//*** What happens if we leave the box as dynamic? Try it to find out! ***//
	//*** What happens if we remove the line "spr_Background->setPhysicsBody(body_Ground);"? Try it to find out! ***//
	//*** What happens if we change the -100 to +100 in the this->addChild() line. Try it to find out! Hint: Spawn some birds! ***//
	//*** What happens if you remove the 'this->addChild()' entirely? Try it to find out! Hint: This line will prove to be very important! ***//
	PhysicsBody* body_Ground = PhysicsBody::createBox(Size(director->getWinSizeInPixels().width, 15.0f)); //Create a box collider for the ground
	body_Ground->setDynamic(false); //We don't want the box collider to move around, we just want other stuff to hit it
	body_Ground->setPositionOffset(Vec2(0.0f, -215.0f)); //Move the collider to where the grass portion of the background sprite is
	spr_Background->setPhysicsBody(body_Ground); //Attach the physics body to the background sprite
	this->addChild(spr_Background, -100); //Add the sprite, pushed way to the back. The second parameter is the draw order. -ve is back, +ve is front



	//Create the particle system that follows the mouse
	//Cocos2D has a bunch of different types of particle systems. You can use a data member of type ParticleSystem and set it to any of them
	//In order to use our own custom image for the particles, we have to add it manually to the texture cache. We are adding a snowflake image here
	//*** What happens if you change the parameter for the createWithTotalParticles() function. It is set to 100. Try 1000 and 10. What is an appropriate number? ***//
	//*** What other particle systems does Cocos2D have? How do they look? Try changing the 'ParticleMeteor' to the other particle types. Hint: Look at the docs below and look for the 'Examples' heading ***//
	//*** Docs: http://www.cocos2d-x.org/wiki/Particles ***//
	mouseParticles = ParticleMeteor::createWithTotalParticles(100);
	mouseParticles->setEndColorVar(Color4F(0.75f, 0.75f, 0.75f, 0.75f));
	mouseParticles->setPosition(INPUTS->getMousePosition());
	mouseParticles->setTexture(director->getTextureCache()->addImage("Demo/Particles/spr_SnowParticle.png"));
	mouseParticles->setLife(0.5f);
	this->addChild(mouseParticles);



	//Create the Text Label In The Top Left
	//A label is just a text object. It doesn't have any special properties. It is not interactable either.
	//You can install ANY font you want as long as it is of type '.ttf'. Just put it in the fonts folder
	//We want to use a specific anchor point. (0, 1) is the top left of the label. This means we position the label using that point. This allows us to very easily put it into a corner
	//There is a shadow added to the text as a highlight
	//*** Try changing the text for the label! ***//
	//*** Try adding your own font or get one from the website below! Or, just switch the label to the other font that comes with Cocos2D! Hint: check the fonts sub-folder in the Resources folder! ***//
	//*** Try removing the enableShadow() line! What happens? Now, try to see what other effects you can use! ***//
	//*** TTF Download Site: http://all-free-download.com/font/ ***//
	Label* textLabel = Label::createWithTTF("Cocos2D!", "Fonts/arial.ttf", 100.0f);
	textLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	textLabel->setPosition(0.0f, DISPLAY->getWindowSize().height);
	textLabel->enableShadow();
	this->addChild(textLabel);



	//Create the restart button
	//The restart button is positioned in the top right of the view and can be pressed to clear everything by reloading the scene
	initRestartButton();
}

void DemoScene::initRestartButton()
{
	//Create the label for the restart menu button. This is the text for the button. It is created like the other label we made but we don't have to do any special positioning
	//We also don't need need to add it to the scene because we will be adding the parent menu object to the scene instead
	Label* restartButtonLabel = Label::createWithTTF("Clear Everything!", "Fonts/arial.ttf", 20.0f);
	restartButtonLabel->enableShadow();



	//Add the restart button and tell it which function to call when the button is pressed
	//The create function takes two parameters
	//		> The first is simply the label we want to create it with. This is the label we made above
	//		> The second parameter looks very scary and doesn't make much sense at first glance
	//			> It is a something called a 'function pointer', further defined by Cocos2D. Essentially, it tells Cocos2D that we want a certain function called when the button is pressed. This allows us to do whatever we want when the button is rressed
	//			> We created a member function for this class that handles the restart button being pressed called onRestartButtonPress()
	//			> *** Try changing the function that is called to spawnParentAndChildren(). See what happens when you press the button in the top right this way. Also, try creating your own function callback ***
	//			> There are several different types of CC_CALLBACK_#. They correspond to a different function call within the C++ standard library. It is difficult to know which you need but here, we need #0
	//			> *** Forum post outlining different # versions: https://stackoverflow.com/questions/24233439/what-is-the-difference-between-all-the-cc-callback-macros ***
	//We are using an anchor point of (1,1) so we can position the label by its top right corner, allowing us to easily put it into the scene where we want it
	MenuItemLabel* restartButton = MenuItemLabel::create(restartButtonLabel, CC_CALLBACK_0(DemoScene::onRestartButtonPress, this));
	restartButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	restartButton->setPosition(DISPLAY->getWindowSizeAsVec2() / 2.0f); //Why do we have to position it by half? Is it doubled for menus????




	//Create the menu parent with all of the elements that are part of the menu
	//If we wanted to add more menu items to the menu, we would simply put them as a parameter
	//IMPORTANT NOTE: All lists in Cocos2D need a NULL at the end, otherwise it will crash!
	//*** Try creating a second menu item and adding it to the menu. Try creating another label and then try creating a different type of menu object entirely! ***//
	Menu* menu = Menu::create(restartButton, NULL);




	//Add the menu to the scene
	//Since the menu items are now grouped underneath the parent menu, they are all added when the menu is added
	//We add it with the z-order of 100 since we always want our menu to draw on top of everything else. Rememember, +ve is in front
	this->addChild(menu, 100);
}

void DemoScene::initSounds()
{
	//Preload the sound effect so we can use it later without having to load it
	//*** What happens if you remove this line? Try it and then spawn an object. Hint: there will only be an issue the first time you spawn a bird. It also might be hard to tell!!! ***//
	//*** Try adding another sound effect yourself. Get a '.mp3' file off a safe website and add it here. Try adding a background theme too! ***//
	AudioEngine::preload("Demo/Sounds/sound_SpawnObject.mp3");
}


//--- Methods ---//
void DemoScene::spawnSoloObject()
{
	//Extract the mouse position from the input handler singleton
	//This returns a Vec2 which is a cocos2D data type
	//This value is updated every frame so you can always get this to get the accurate mouse cursor position
	//IMPORTANT NOTE: The mouse position is updated by Cocos2D through the input handler's event management system. This means there may be a slight delay as Cocos2D has to call the functions separately
	Vec2 mousePos = INPUTS->getMousePosition();



	//Create a new sprite
	//This follows a very similar format to the background sprite creation
	//We load the handle and set the anchor point just like before
	//We are setting the scale to 0.25 which is 1/4 the normal size. By default, Cocos2D translates the image size directly into the game. The bird is 256x256 which is most of our screen! So we are just scaling it down to fit better
	//We are setting the position to be the mouse position we just got from the input handler.
	Sprite* newSprite = Sprite::create("Demo/Birds/spr_BirdYellow.png"); //Load the handle
	newSprite->setPosition(mousePos); //Place the new bird at the mouse position
	newSprite->setScale(0.25f); //Scale the bird since it loads in quite large 
	newSprite->setAnchorPoint(Vec2(0.5f, 0.5f)); //Ensure the middle of the bird is the anchor point



	//Create the physics body and add the bird to the scene
	//We are using getContentSize() which returns the size of the object in pixels. Since it is scaled by 0.25x, it will be 1/4 the size of the image
	//Unlike the background, we are setting this physics body to be dynamic. This means it will interact with gravity as we expect.
	PhysicsBody* body_Bird = PhysicsBody::createCircle(newSprite->getContentSize().width / 2.0f);
	body_Bird->setDynamic(true);
	newSprite->setPhysicsBody(body_Bird); //Connect the physics body and the sprite
	this->addChild(newSprite, 0); //Add the bird to the scene in the middle rendering layer



	//Run a sequence of actions on the new bird we just added
	//A sequence is group of actions
	//An action is a pre-defined Cocos2D data type that has some form of effect, usually over a certain time
	//You can use a bunch of sub-class actions. You have to use the create function for them. You could define them as variables ahead of time or simply create them in the parameter list for the sequence like below
	//DelayTime() is an action that simply waits a given number of seconds
	//RemoveSelf() simply deletes the node that is running the action 
	//This sequence of actions means that the node will do nothing special for 5s and then delete itself. This prevents us from continually spawning new sprites and running out of memory
	//IMPORTANT NOTE: As with menu creation, you NEED a NULL at the end of the action list. Without it, Cocos2D will crash
	//*** Try changing the order of the actions. What happens? ***//
	//*** Try changing one of the actions to be something different. Take a look at the docs linked below to find some of the other types ***//
	//*** Try adding another action to the list. Play around with different options and see what you can do! ***//
	//*** Docs: http://www.cocos2d-x.org/wiki/Actions ***//
	newSprite->runAction(Sequence::create(DelayTime::create(5.0f), RemoveSelf::create(), NULL));
	


	//Play the sound now that the object has been spawned
	//Since we use the same file path as we did when we pre-loaded the sound in initSounds(), it should immediately play the one already loaded
	//*** Try playing your own unique sound here instead of the one we put in ***//
	//*** Try making it so a sound plays when the user presses a button. Hint: Place the check in a function that is called every frame ***//
	AudioEngine::play2d("Demo/Sounds/sound_SpawnObject.mp3");
}

void DemoScene::spawnParentAndChildren()
{
	//Extract the mouse position from the input handler singleton
	//This returns a Vec2 which is a cocos2D data type
	//This value is updated every frame so you can always get this to get the accurate mouse cursor position
	//IMPORTANT NOTE: The mouse position is updated by Cocos2D through the input handler's event management system. This means there may be a slight delay as Cocos2D has to call the functions separately
	Vec2 mousePos = INPUTS->getMousePosition();



	//Create the 'parent' sprite
	//This follows a very similar format to the background sprite creation
	//We load the handle and set the anchor point just like before
	//We are setting the scale to 0.25 which is 1/4 the normal size. By default, Cocos2D translates the image size directly into the game. The bird is 256x256 which is most of our screen! So we are just scaling it down to fit better
	//We are setting the position to be the mouse position we just got from the input handler.
	Sprite* parentSprite = Sprite::create("Demo/Birds/spr_BirdRed.png"); //Load the handle
	parentSprite->setPosition(mousePos); //Place the new bird at the mouse position
	parentSprite->setScale(0.25f); //Scale the bird since it loads in quite large 
	parentSprite->setAnchorPoint(Vec2(0.5f, 0.5f)); //Ensure the middle of the bird is the anchor point


	//Create a physics body and add it to the parent bird
	//We are using getContentSize() which returns the size of the object in pixels. Since it is scaled by 0.25x, it will be 1/4 the size of the image
	//Unlike the background, we are setting this physics body to be dynamic. This means it will interact with gravity as we expect.
	PhysicsBody* body_Parent = PhysicsBody::createCircle(parentSprite->getContentSize().width / 2.0f);
	body_Parent->setDynamic(true);
	parentSprite->setPhysicsBody(body_Parent); //Connect the physics body and the sprite



	//Create a child draw node
	//Child:
	//		> This will become a child of the parent sprite we just created. This means that it will be 'attached' to the parent sprite
	//		> If the parent sprite moves, this will move with it. Think of a character wearing a hat. Whenever the character moves, the hat stays on their head
	//		> We can still move this child separately but in this case, we don't
	//Draw node:
	//		> Similar to a sprite but draws its own image instead of using a sprite
	//		> Can be used to draw circles, rectangles, triangles, etc
	//		> Looks a bit like the debug draw shapes
	//		> Useful for drawing basic shapes like lines. Ex: Can be used to draw a line behind a bullet to create a trail
	//*** What happens if you change 'parentSprite->addChild()' to 'this->addChild()'? Why is this? ***//
	//*** Try drawing a different shape, instead of a dot. Try drawing a rectangle for instance ***//
	DrawNode* child_A = DrawNode::create();
	child_A->drawDot(Vec2(0.0f, 0.0f), 64.0f, Color4F(0.0f, 0.0f, 1.0f, 0.5f));
	parentSprite->addChild(child_A);



	//Create the second child as a sprite
	//Creating it like normal, just using an image from the resources folder
	//Note the position change
	//		> This puts the child on the top right of the parent bird
	//				> When positioning a child, the position becomes relative to the parent instead
	//						> (0,0) is now defined as the bottom left corner of the parent, as opposed to the bottom left of the window
	//				> The parent bird's image size is 256x256. We want it to be positioned in the top right corner so we offset it by the size of the image
	//						> You might remember that the bird is scaled by 0.25. So, shouldn't this mean that we actually only want to offset by 64? 
	//						> Not quite. The parent's scale actually affects the children as well. Scaling the parent will scale everything underneath it.
	//*** Try adding a positional offset to the other child object. What happens and why? ***//
	//*** What happens if you scale this child object now? Does the parent get affected? ***//
	//*** What happens if you make this a child of the child draw node instead? Does it look different? Why or why not? ***//
	Sprite* child_B = Sprite::create("Demo/Birds/spr_BirdBlue.png");
	child_B->setPosition(Vec2(256.0f, 256.0f));
	parentSprite->addChild(child_B);
	


	//Add the parent to the scene and thus the children as well
	//The children are added automatically since their parent has been added
	this->addChild(parentSprite, 0); 



	//Run an action on the parent sprite
	//This is the exact same sequence we run on the object we created in spawnSoloObject() above
	//It simply waits 5s and then deletes itself
	//Helps prevent overloading the memory
	//IMPORTANT NOTE: Again, all lists in Cocos2D like this require a NULL at the end, otherwise it will crash
	parentSprite->runAction(Sequence::create(DelayTime::create(5.0f), RemoveSelf::create(), NULL));



	//Run an action SEQUENCE on the child draw node
	//This sequence will cause the draw node to rotate 5 full times in 3 seconds. After it is finished rotating, it will fade out completely over the course of 2s.
	//IMPORTANT NOTE: Sequence VS Spawn
	//		> Sequence will run the action list one after another in a sequence (makes sense)
	//				> Action B will only start after Action A has finished. In this example, the FadeOut will only start after it is finished rotating
	//		> Spawn is poorly named but basically, it runs all of the actions at once
	//				> In the spawn example below, the object will rotate, scale, and tint at the same time. It does not wait until one is finished before starting the next
	//		> *** Docs: http://www.cocos2d-x.org/wiki/Actions (Look for "Sequences and How To Run Them") ***//
	//IMPORTANT NOTE: Again, all lists in Cocos2D like this require a NULL at the end, otherwise it will crash
	child_A->runAction(Sequence::create(RotateBy::create(3.0f, Vec3(1800.0f, 0.0f, 0.0f)), FadeOut::create(2.0f), NULL));



	//Run an action SPAWN on the child blue bird sprite
	//This causes the bird to rotate 10 full times, scale up, and tint a greenish colour. Since it is Spawn instead of Sequence, all will start right away
	//IMPORTANT NOTE: Sequence VS Spawn
	//		> Sequence will run the action list one after another in a sequence (makes sense)
	//				> Action B will only start after Action A has finished. In this example, the FadeOut will only start after it is finished rotating
	//		> Spawn is poorly named but basically, it runs all of the actions at once
	//				> In the spawn example below, the object will rotate, scale, and tint at the same time. It does not wait until one is finished before starting the next
	//		> *** Docs: http://www.cocos2d-x.org/wiki/Actions (Look for "Sequences and How To Run Them") ***//
	//IMPORTANT NOTE: Again, all lists in Cocos2D like this require a NULL at the end, otherwise it will crash
	//*** Try changing RotateBy() to RotateTo() and ScaleTo() to ScaleBy(). What is the difference between _To() and _By()? Hint: It is mentioned in the docs above! ***//
	child_B->runAction(Spawn::create(RotateBy::create(3.0f, Vec3(0.0f, 0.0f, 3600.0f)), ScaleTo::create(3.0f, 1.5f, 1.5f), TintTo::create(3.0f, Color3B(255.0f, 255.0f, 0.0f)), NULL));
	


	//Play the sound now that the object has been spawned
	//Since we use the same file path as we did when we pre-loaded the sound in initSounds(), it should immediately play the one already loaded
	AudioEngine::play2d("Demo/Sounds/sound_SpawnObject.mp3");
}

void DemoScene::nextDebugDraw()
{
	//Increment the current debug draw type
	debugDrawType++;

	//Ensure the draw type gets wrapped properly. Prevents trying to set a debug draw type that doesn't exist
	if (debugDrawType > 3)
		debugDrawType = 0;

	//Set the correct debug draw type depending on the new value
	//Use the physicsWorld reference we set in the createScene() function
	//The 'Draw Mask' is just what we want to see be drawn
	switch (debugDrawType)
	{
	case 0: //None
		physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
		break;

	case 1: //Contact
		physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_CONTACT);
		break;

	case 2: //Shape
		physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);
		break;

	case 3: //All
		physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		break;
	}
}



//--- Menu Callbacks ---//
void DemoScene::onRestartButtonPress()
{
	//Reloading the scene can be accomplished by simply replacing the scene with the same scene we are running
	//This is the exact same logic as changing to a different scene
	//replaceScene() simply ends the current scene and switches to the new scene. popScene stores the active scene so we can come back to it with no data loss. Use the appropriate choice for your game
	//Note that we don't have to do anything else. Cocos2D handles clearing the physics world, cleaning up the sprites, etc.
	//*** Try to create a menu scene for this demo project. Add a button in the menu to switch to this scene and one in here to go back! ***//
	//*** Try adding a transition to the scene swap! For instance, replace the line below with "director->replaceScene(TransitionPageTurn::create(2.0f, DemoScene::createScene(), false));" to add a page turn effect! ***//
	//*** Docs: http://www.cocos2d-x.org/wiki/Building_and_Transitioning_Scenes ***//
	director->replaceScene(DemoScene::createScene());
}