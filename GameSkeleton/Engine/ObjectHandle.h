#pragma once

#include "GameWindowManager.h"
#include "DebugMenu.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <QtOpenGL\qglwidget>
#include <QtGui\qpushbutton.h>
#include <QtGui\qmainwindow.h>
#include "glm.hpp"
#include <vector>

class GameWindowManager;
class Component;

class ENGINE_SHARED ObjectHandle : public QGLWidget
{
	Q_OBJECT

	QPushButton* texture;
	QPushButton* player;
	QPushButton* script;
	QPushButton* input;
	QPushButton* path;
	QPushButton* intelligence;
	QPushButton* objDelete;
	QPushButton* objClone;
	QPushButton* NODE;
	btRigidBody* trackBody;
	btDynamicsWorld* trackWorld;
	bool addedToWorld;
	glm::vec3 oldPosition;
	bool manualVelocity;
public:
	//Parent class
	GameWindowManager* parent;

	//Object Variables
	struct variables{
		std::string variableName;
		std::string value;
	} objectVariables[100];

	int numVariables;

	void objectVariableHandle(std::string passedIn);

	//Update object automatically
	bool uObj;

	void stopObject();

	//Everything for Instanced Objects
	struct instanceObj
	{
		std::string instanceName;
		glm::vec3 instancePosition;
		glm::vec3 instanceForce;
	} IO[20];

	int numInstances;
	bool isInstance;
	bool isWorldInstance;

	//Handles turning
	bool isTurning;
	float lerpTurn;
	glm::vec3 oneTurn;
	glm::vec3 twoTurn;
	float oneTurnCheck;
	float twoTurnCheck;

	//Tell Gamescenes to switch
	int switchScene;

	//Tell if object can rotate on yaw, pitch, or roll
	bool allowYaw;
	bool allowPitch;
	bool allowRoll;

	//Gives Artificial Intelligence priority over pathing or anything else
	bool AIPriority;

	//Checks if object has player
	bool isPlay;

	//Pathing handlers
	void setUpPathCOMP();
	bool addNODE;

	//Objects Name
	std::string nameID;

	//Check if gravity is on or off
	bool gravityOn;
	bool oldGravity;
	bool dynamicBool;
	bool oldDynamic;

	//Position of objects before world runs
	glm::vec3 position;

	//Position of object while world is running
	glm::vec3 realPosition;

	//Scale of object
	glm::vec3 scale;

	//Scale of object during run
	glm::vec3 realScale;

	//Rotation of object
	glm::vec3 rotation;

	//Rotation of object during run
	glm::vec3 realRotation;

	//Restitution of object
	float restitution;
	float oldRestitution;

	//Friction of object
	float friction;
	float oldFriction;

	//Force to be aplied to object (X, Y, Z)
	float xForce;
	float yForce;
	float zForce;

	//Check if force needs to be set for each frame (force is cleared after each frame, no constant force is ever applied)
	bool forceSet;

	//Velocity to be applied to object (x, y, z)
	float xVelocity;
	float yVelocity;
	float zVelocity;

	//Mass of object
	float mass;
	float oldMass;

	//Components of objects
	std::vector<Component*> components;

	//Menu index to reach object quickly
	int menuNum;

	//Add Texture
	bool addTex;

	//Add Player Component
	bool addPlay;

	//Add Input
	bool addInp;

	//Add Script
	bool addScr;

	//Add Pathing
	bool addPa;

	//Add Artifical Intelligence
	bool addInt;

	//Delete the Object
	bool deleteObject;

	//Clone the Object
	bool cloneObject;

	//Vector for Camera Direction
	glm::vec3 camDirection;

	ObjectHandle(std::string ID, glm::vec3 pos=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 sca=glm::vec3(1.0f,1.0f,1.0f), float rest=0.0f, float frict=0.0f, float m=0.0f, bool aYaw=true, bool aPitch=true, bool aRoll=true, bool grav=false, bool worldInstance=false, bool dynamic=false);
	~ObjectHandle();
	void toggleMenu();
	void objectUpdate(btDynamicsWorld*, btRigidBody*, bool, float);
	void addComponent(Component* comp);
	void moveForward(glm::vec3);

	void stopRotating();
	void keepUpForce(float);
	void keepUpVelocity(float);
	void keepUpXVelocity(float);
	void keepUpZVelocity(float);

	//Forward Vector
	glm::vec3 tempForward;

	void setTransformOfObject(glm::mat4, btRigidBody*);

	//Checks if current object has a player component
	bool isPlayer();

	//Sets the X, Y, or Z
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	//Sets the Scale (X, Y, Z)
	void setXScale(float x);
	void setYScale(float y);
	void setZScale(float z);

	//Turns on or off Gravity
	void setGravity(bool grav);

	//Sets Restitution
	void setRestitution(float rest);

	//Sets Friction
	void setFriction(float frict);

	//Sets Force (X, Y, Z)
	void setXForce(float x);
	void setYForce(float y);
	void setZForce(float z);

	//Sets Velocity (X, Y, Z)
	void setXVelocity(float x);
	void setYVelocity(float y);
	void setZVelocity(float z);

	//Sets Rotation (Yaw, Pitch, Role)
	void setYaw(float yaw);
	void setPitch(float pitch);
	void setRoll(float roll);

	//Manual Velocity Input
	void turnOnManualVelocity();

	//Gets Mass
	float getMass();

	//Turns Menu On or Off
	bool menuOn;

	//Removes the player component
	void removePlayer();

	//Returns if a raycast hits an object or not
	bool raycast(glm::vec3 toVector, glm::vec3 fromVector);
private slots:
	void addTexture();
	void addPlayer();
	void addInput();
	void addScript();
	void addPath();
	void addNode();
	void addIntelligence();
	void objectDelete();
	void objectClone();
};