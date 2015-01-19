#include "ObjectHandle.h"
#include "Component.h"

ObjectHandle::ObjectHandle(std::string ID, glm::vec3 pos, glm::vec3 sca, float rest, float frict, float m, bool aYaw, bool aPitch, bool aRoll, bool grav, bool worldInstance, bool dynamic)
{
	QWidget* tempLay=new QWidget();
	AIPriority=false;
	isInstance=false;
	isWorldInstance=worldInstance;
	numVariables=0;

	uObj=true;

	numInstances=0;
	nameID=ID;
	tempLay->setFixedWidth(300);
	tempLay->setMinimumHeight(600);
	position=pos;
	mass=m;
	friction=frict;
	restitution=rest;
	gravityOn=grav;
	addedToWorld=false;
	addTex=false;
	addPlay=false;
	addInp=false;
	addScr=false;
	isPlay=false;
	addInt=false;
	deleteObject=false;
	cloneObject=false;
	addPa=false;
	scale=sca;
	xForce=0;
	yForce=0;
	zForce=0;
	xVelocity=0;
	yVelocity=0;
	zVelocity=0;
	addNODE=false;
	realRotation=glm::vec3(0.0f);
	camDirection=glm::vec3(0.0f);
	lerpTurn=0.0f;
	oneTurnCheck=0.0f;
	twoTurnCheck=0.0f;
	oneTurn=glm::vec3(0.0f);
	twoTurn=glm::vec3(0.0f);
	allowYaw=aYaw;
	allowPitch=aPitch;
	allowRoll=aRoll;
	switchScene=-1;
	dynamicBool=dynamic;

	int replace=1;
	std::string tempName=nameID;
#if EDITOR_ON
	while(debugMenu.alreadyInWorld(nameID))
	{
		nameID=tempName+std::to_string(replace);
		replace++;
	}

	if(!worldInstance)
	{
		debugMenu.listItem(&nameID, "World Control");
		menuOn=true;
		debugMenu.addLayout("Object");
		menuNum=debugMenu.lastLayout();

		debugMenu.basicTextBox(&nameID, "Object Name: ", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.toggleBool("Gravity: ", "Object Information", &gravityOn, menuNum);
		debugMenu.toggleBool("Dynamic: ", "Object Information", &dynamicBool, menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.numberSpinBox(&mass, "Mass: ", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.numberSpinBox(&position.x, "X: ", "Object Information", menuNum);
		debugMenu.numberSpinBox(&position.y, "Y: ", "Object Information", menuNum);
		debugMenu.numberSpinBox(&position.z, "Z: ", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.numberSpinBox(&scale.x, "Scale X: ", "Object Information", menuNum);
		debugMenu.numberSpinBox(&scale.y, "Scale Y: ", "Object Information", menuNum);
		debugMenu.numberSpinBox(&scale.z, "Scale Z: ", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.numberSpinBox(&rotation.x, "Yaw:", "Object Information", menuNum);
		debugMenu.numberSpinBox(&rotation.y, "Pitch:", "Object Information", menuNum);
		debugMenu.numberSpinBox(&rotation.z, "Roll:", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.toggleBool("Allow Yaw:", "Object Information", &allowYaw, menuNum);
		debugMenu.toggleBool("Allow Pitch:", "Object Information", &allowPitch, menuNum);
		debugMenu.toggleBool("Allow Roll:", "Object Information", &allowRoll, menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.slideFloat(&restitution, "Restitution: ", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		debugMenu.slideFloat(&friction, "Friction: ", "Object Information", menuNum);
		debugMenu.newRow("Object Information", menuNum);
		texture=new QPushButton("Add Texture");
		debugMenu.pushButton(texture, "Add Texture", "Object Information", menuNum);
		connect(texture, SIGNAL(clicked()),  this, SLOT(addTexture()));
		player=new QPushButton("Add Player");
		debugMenu.pushButton(player, "Add Player", "Object Information", menuNum);
		connect(player, SIGNAL(clicked()), this, SLOT(addPlayer()));
		debugMenu.newRow("Object Information", menuNum);
		script=new QPushButton("Add Script");
		debugMenu.pushButton(script, "Add Script", "Object Information", menuNum);
		connect(script, SIGNAL(clicked()), this, SLOT(addScript()));
		input=new QPushButton("Add Input");
		debugMenu.pushButton(input, "Add Input", "Object Information", menuNum);
		connect(input, SIGNAL(clicked()), this, SLOT(addInput()));
		debugMenu.newRow("Object Information", menuNum);
		path=new QPushButton("Add Path");
		debugMenu.pushButton(path, "Add Path", "Object Information", menuNum);
		connect(path, SIGNAL(clicked()), this, SLOT(addPath()));
		intelligence=new QPushButton("Add AI");
		debugMenu.pushButton(intelligence, "Add AI", "Object Information", menuNum);
		connect(intelligence, SIGNAL(clicked()), this, SLOT(addIntelligence()));
		debugMenu.newRow("Object Information", menuNum);
		objDelete=new QPushButton("Delete Object");
		debugMenu.pushButton(objDelete, "Delete Object", "Object Information", menuNum);
		connect(objDelete, SIGNAL(clicked()), this, SLOT(objectDelete()));
		objClone=new QPushButton("Clone Object");
		debugMenu.pushButton(objClone, "Clone Object", "Object Information", menuNum);
		connect(objClone, SIGNAL(clicked()), this, SLOT(objectClone()));
		debugMenu.newRow("Object Information", menuNum);

		debugMenu.manualChangeObjectSelected(menuNum);
	}
#endif;
}

ObjectHandle::~ObjectHandle()
{
	for(int i=0; i<components.size(); i++)
	{
		delete components[i];
	}
	components.clear();

	debugMenu.removeLayout(menuNum);
}

void ObjectHandle::objectClone()
{
	cloneObject=true;
}

void ObjectHandle::moveForward(glm::vec3 forwardVector)
{
	setXForce(forwardVector.x);
	setYForce(forwardVector.y);
	setZForce(forwardVector.z);
	tempForward=forwardVector;
}

void ObjectHandle::setTransformOfObject(glm::mat4 newTransform, btRigidBody* body)
{
	btTransform trans;
	trans.setFromOpenGLMatrix(&newTransform[0][0]);
	body->setWorldTransform(trans);
}

void ObjectHandle::objectUpdate(btDynamicsWorld* world, btRigidBody* body, bool worldRunning, float dt)
{
	trackBody=body;
	trackWorld=world;

	if(uObj || std::abs(oldRestitution-restitution)>0.0001 || std::abs(oldFriction-friction)>0.0001 || std::abs(oldMass-mass)>0.0001 || oldGravity!=gravityOn || dynamicBool!=oldDynamic)
	{
		if(std::abs(oldRestitution-restitution)>0.0001)
			oldRestitution=restitution;
		if(std::abs(oldFriction-friction)>0.0001)
			oldFriction=friction;
		if(std::abs(oldMass-mass)>0.0001)
			oldMass=mass;
		if(uObj)
			uObj=false;
		if(oldGravity!=gravityOn)
			oldGravity=gravityOn;
		if(dynamicBool!=oldDynamic)
			oldDynamic=dynamicBool;

		if(!isInstance)
			trackWorld->removeRigidBody(trackBody);

		if(gravityOn && mass>0)
		{
			trackBody->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
			trackBody->setRestitution(restitution*1.75);
			trackBody->setFriction(friction);
			if(dynamicBool)
				trackBody->setGravity(btVector3(0.0f,0.0f,0.0f));
			btVector3 inertia(0,0,0);
			trackBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
			trackBody->setActivationState(DISABLE_DEACTIVATION);
			trackBody->setMassProps(mass, inertia);
			trackBody->setLinearFactor(btVector3(1,1,1));
			trackBody->setAngularFactor(btVector3(1,1,1));
			trackBody->updateInertiaTensor();
			trackBody->clearForces();
			trackBody->setDamping(0.0f,0.0f);
			trackBody->setSleepingThresholds(0.3f,0.3f);
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
			trackBody->setWorldTransform(transform);
		}
		else
		{
			trackBody->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
			trackBody->setRestitution(restitution*1.75);
			trackBody->setFriction(friction);
			btVector3 inertia(0,0,0);
			trackBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
			trackBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
			trackBody->setMassProps(mass, inertia);
			trackBody->setLinearFactor(btVector3(0,0,0));
			trackBody->setAngularFactor(btVector3(0,0,0));
			trackBody->setGravity(btVector3(0,0,0));
			trackBody->updateInertiaTensor();
			trackBody->setAngularVelocity(btVector3(0,0,0));
			trackBody->setLinearVelocity(btVector3(0,0,0));
			trackBody->clearForces();
			trackBody->setDamping(0.0f,0.0f);
			trackBody->setSleepingThresholds(0.3f,0.3f);
			trackBody->setActivationState(WANTS_DEACTIVATION);
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
			trackBody->setWorldTransform(transform);
		}
		realPosition=position;
		realScale=scale;
		realRotation=rotation;

		if(!isInstance)
			trackWorld->addRigidBody(trackBody);
	}

	for(int i=0; i<components.size() && worldRunning; i++)
	{
		if(components[i]->type!="AI")
			components[i]->update(dt);
	}

	for(int j=0; j<components.size() && worldRunning; j++)
	{
		if(components[j]->type=="AI")
			components[j]->update(dt);
	}

	if(forceSet && worldRunning)
	{
		body->activate(true);
		body->applyImpulse(btVector3(xForce, yForce, zForce), btVector3(0,0,0));
		xForce=0;
		yForce=0;
		zForce=0;
		forceSet=false;
	}

	//Update Transform
	if(!worldRunning)
	{
		btTransform transform;

		transform.setIdentity();
		transform.setOrigin(btVector3(realPosition.x, realPosition.y,realPosition.z));
		btQuaternion quat;
		quat.setEuler(realRotation.x,realRotation.y,realRotation.z);
		transform.setRotation(quat);

		body->getMotionState()->setWorldTransform(transform);
	

		//Update Transform
		world->removeRigidBody(body);
		body->setCenterOfMassTransform(transform);
		body->setAngularVelocity(btVector3(0,0,0));
		body->setLinearVelocity(btVector3(0,0,0));
		body->getCollisionShape()->setLocalScaling(btVector3(scale.x,scale.y,scale.z));
		body->clearForces();
		body->setDamping(0.0f,0.0f);
		body->setSleepingThresholds(0.3f,0.3f);
		body->setWorldTransform(transform);
		world->addRigidBody(body);
		realPosition=position;
		realScale=scale;
		realRotation=rotation;
	}

	if(xVelocity!=0 || yVelocity!=0 || zVelocity!=0)
	{
		body->setLinearVelocity(btVector3(xVelocity,yVelocity,zVelocity));
		xVelocity=0;
		yVelocity=0;
		zVelocity=0;
	}
	if(worldRunning)
	{
		realPosition.x=trackBody->getCenterOfMassPosition().getX();
		realPosition.y=trackBody->getCenterOfMassPosition().getY();
		realPosition.z=trackBody->getCenterOfMassPosition().getZ();
	}
	else
	{
		realPosition=position;
	}
	trackBody->clearForces();
}

void ObjectHandle::stopObject()
{
	trackBody->setLinearVelocity(btVector3(0.0f,0.0f,0.0f));
}

void ObjectHandle::keepUpXVelocity(float passedIn)
{
	xVelocity=trackBody->getLinearVelocity().getX();
	yVelocity=trackBody->getLinearVelocity().getY();
	zVelocity=trackBody->getLinearVelocity().getZ();

	if(xVelocity<passedIn)
	{
		float leftOver=passedIn-xVelocity;
		if(xVelocity<0)
			xVelocity+=leftOver;
		else
			xVelocity-=leftOver;
	}
	else if(xVelocity>passedIn)
	{
		float leftOver=xVelocity-passedIn;
		if(xVelocity>0)
			xVelocity-=leftOver;
		else
			xVelocity+=leftOver;
	}
	trackBody->setLinearVelocity(btVector3(xVelocity, yVelocity, zVelocity));
}
void ObjectHandle::keepUpZVelocity(float passedIn)
{
	xVelocity=trackBody->getLinearVelocity().getX();
	yVelocity=trackBody->getLinearVelocity().getY();
	zVelocity=trackBody->getLinearVelocity().getZ();

	if(zVelocity<passedIn)
	{
		float leftOver=passedIn-zVelocity;
		if(zVelocity<0)
			zVelocity+=leftOver;
		else
			zVelocity-=leftOver;
	}
	else if(zVelocity>passedIn)
	{
		float leftOver=zVelocity-passedIn;
		if(zVelocity>0)
			zVelocity-=leftOver;
		else
			zVelocity+=leftOver;
	}
	trackBody->setLinearVelocity(btVector3(xVelocity, yVelocity, zVelocity));
}

void ObjectHandle::keepUpVelocity(float passedIn)
{
	xVelocity=trackBody->getLinearVelocity().getX();
	yVelocity=trackBody->getLinearVelocity().getY();
	zVelocity=trackBody->getLinearVelocity().getZ();
	if(glm::length(glm::vec3(xVelocity, yVelocity, zVelocity))<passedIn)
	{
		float leftOver=passedIn-glm::length(glm::vec3(xVelocity, yVelocity, zVelocity));
		leftOver=leftOver/3;
		if(xVelocity>0)
			xVelocity+=leftOver; 
		else
			xVelocity-=leftOver;
		if(yVelocity>0)
			yVelocity+=leftOver;
		else
			yVelocity-=leftOver;
		if(zVelocity>0)
			zVelocity+=leftOver;
		else
			zVelocity-=leftOver;
	}
	if(glm::length(glm::vec3(xVelocity, yVelocity, zVelocity))>passedIn)
	{
		float leftOver=glm::length(glm::vec3(xVelocity, yVelocity, zVelocity))-passedIn;
		leftOver=leftOver/3;
		if(xVelocity<0)
			xVelocity+=leftOver; 
		else
			xVelocity-=leftOver;
		if(yVelocity<0)
			yVelocity+=leftOver;
		else
			yVelocity-=leftOver;
		if(zVelocity<0)
			zVelocity+=leftOver;
		else
			zVelocity-=leftOver;
	}
	trackBody->setLinearVelocity(btVector3(xVelocity, yVelocity, zVelocity));
}

void ObjectHandle::keepUpForce(float passedIn)
{
	if(glm::length(glm::vec3(trackBody->getLinearVelocity().getX(),trackBody->getLinearVelocity().getY(),trackBody->getLinearVelocity().getZ()))<passedIn)
		forceSet=true;
	else
	{
 		xForce=0;
		yForce=0;
		zForce=0;
		forceSet=false;
	}
}

void ObjectHandle::objectVariableHandle(std::string passedIn)
{
	std::string newVariable="";
	int track=0;

	if(passedIn.find("=")==std::string::npos)
	{
		return;
	}

	while(passedIn.at(track)!=';')
	{
		newVariable+=passedIn.at(track++);
	}

	bool alreadyAVariable=false;
	int which=-1;
	for(int i=0; i<numVariables; i++)
	{
		if(objectVariables[i].variableName.compare(newVariable.substr(0, newVariable.find("=")))==0)
		{
			alreadyAVariable=true;
			which=i;
		}
	}

	if(!alreadyAVariable)
	{
		objectVariables[numVariables].variableName=newVariable.substr(0, newVariable.find("="));
		objectVariables[numVariables++].value=newVariable.substr(newVariable.find("=")+1, newVariable.length());
	}
	else
	{
		//objectVariables[which].value=newVariable.substr(newVariable.find("=")+1, newVariable.length());
	}
}

bool ObjectHandle::isPlayer()
{
	return isPlay;
}

void ObjectHandle::toggleMenu()
{
#if EDITOR_ON
	debugMenu.toggleVisibility(menuNum);
#endif
}

void ObjectHandle::addComponent(Component* comp)
{
	components.push_back(comp);
	components[components.size()-1]->Initialize(this, menuNum);
}

void ObjectHandle::addTexture()
{
	addTex=true;
}

void ObjectHandle::addInput()
{
	addInp=true;
}

void ObjectHandle::addScript()
{
	addScr=true;
}

void ObjectHandle::addPath()
{
	addPa=true;
}

void ObjectHandle::addPlayer()
{
	bool alreadyPlayer=false;

	for(int i=0; i<components.size(); i++)
	{
		if(components[i]->type=="Player")
		{
			alreadyPlayer=true;
		}
	}

	if(!alreadyPlayer)
	{
		addPlay=true;
		isPlay=true;
	}
}

void ObjectHandle::stopRotating()
{
	trackBody->setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
	trackBody->setAngularFactor(0.0f);
}

void ObjectHandle::removePlayer()
{
	isPlay=false;

	for(int i=0; i<components.size(); i++)
	{
		if(components[i]->type=="Player")
		{
			components.erase(components.begin()+i);
		}
	}
}

void ObjectHandle::objectDelete()
{
	deleteObject=true;
}

void ObjectHandle::setX(float x)
{
	btTransform transform;
	transform.setIdentity();
	realPosition.x=x;
	transform.setOrigin(btVector3(realPosition.x, trackBody->getCenterOfMassPosition().getY(), trackBody->getCenterOfMassPosition().getZ()));
	btQuaternion quat;
	quat=trackBody->getOrientation();
	transform.setRotation(quat);
	trackBody->setCenterOfMassTransform(transform);
}
void ObjectHandle::setY(float y)
{
	btTransform transform;
	transform.setIdentity();
	realPosition.y=y;
	transform.setOrigin(btVector3(trackBody->getCenterOfMassPosition().getX(), realPosition.y, trackBody->getCenterOfMassPosition().getZ()));
	btQuaternion quat;
	quat=trackBody->getOrientation();
	transform.setRotation(quat);
	trackBody->setCenterOfMassTransform(transform);
}
void ObjectHandle::setZ(float z)
{
	btTransform transform;
	transform.setIdentity();
	realPosition.z=z;
	transform.setOrigin(btVector3(trackBody->getCenterOfMassPosition().getX(), trackBody->getCenterOfMassPosition().getY(), realPosition.z));
	btQuaternion quat;
	quat=trackBody->getOrientation();
	transform.setRotation(quat);
	trackBody->setCenterOfMassTransform(transform);
}

void ObjectHandle::setXScale(float x)
{
	realScale.x=x;
	trackBody->getCollisionShape()->setLocalScaling(btVector3(realScale.x, trackBody->getCollisionShape()->getLocalScaling().getY(),trackBody->getCollisionShape()->getLocalScaling().getZ()));
}
void ObjectHandle::setYScale(float y)
{
	realScale.y=y;
	trackBody->getCollisionShape()->setLocalScaling(btVector3(trackBody->getCollisionShape()->getLocalScaling().getX(), realScale.y , trackBody->getCollisionShape()->getLocalScaling().getZ()));
}
void ObjectHandle::setZScale(float z)
{
	realScale.z=z;
	trackBody->getCollisionShape()->setLocalScaling(btVector3(trackBody->getCollisionShape()->getLocalScaling().getX(), trackBody->getCollisionShape()->getLocalScaling().getY(),realScale.z));
}
void ObjectHandle::setGravity(bool grav)
{
	gravityOn=grav;
}

void ObjectHandle::setYaw(float yaw)
{
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(realPosition.x, realPosition.y, realPosition.z));
	btQuaternion quat;
	realRotation.x=rotation.x+yaw;
	quat.setEuler(realRotation.x, realRotation.y, realRotation.z);
	tr.setRotation(quat);
	trackBody->setCenterOfMassTransform(tr);
}

void ObjectHandle::setPitch(float pitch)
{
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(realPosition.x, realPosition.y, realPosition.z));
	btQuaternion quat;
	realRotation.y=rotation.y+pitch;
	quat.setEuler(realRotation.x, realRotation.y, realRotation.z);
	tr.setRotation(quat);
	trackBody->setCenterOfMassTransform(tr);
}  

void ObjectHandle::setRoll(float roll)
{
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(realPosition.x, realPosition.y, realPosition.z));
	btQuaternion quat;
	realRotation.z=rotation.z+roll;
	quat.setEuler(realRotation.x, realRotation.y, realRotation.z);
	tr.setRotation(quat);
	trackBody->setCenterOfMassTransform(tr);
}

float ObjectHandle::getMass()
{
	return mass;
}

void ObjectHandle::setXForce(float x)
{
	xForce=x;
	forceSet=true;
}
void ObjectHandle::setYForce(float y)
{
	yForce=y;
	forceSet=true;
}
void ObjectHandle::setZForce(float z)
{
	zForce=z;
	forceSet=true;
}

void ObjectHandle::setXVelocity(float x)
{
	xVelocity=x;
}
void ObjectHandle::setYVelocity(float y)
{
	yVelocity=y;
}
void ObjectHandle::setZVelocity(float z)
{
	zVelocity=z;
}
void ObjectHandle::turnOnManualVelocity()
{
	if(manualVelocity)
	{
		manualVelocity=false;
	}
	else if(!manualVelocity)
	{
		manualVelocity=true;
	}
}

void ObjectHandle::setUpPathCOMP()
{
#if EDITOR_ON
	NODE=new QPushButton("Add Node");
	debugMenu.pushButton(NODE, "Add Node", "Path", menuNum);
	connect(NODE, SIGNAL(clicked()), this, SLOT(addNode()));
	debugMenu.newRow("Path", menuNum);
#endif
}

void ObjectHandle::addNode()
{
	addNODE=true;
}

void ObjectHandle::addIntelligence()
{
	addInt=true;
}

bool ObjectHandle::raycast(glm::vec3 toVector, glm::vec3 fromVector)
{
	btCollisionWorld::AllHitsRayResultCallback RayCallback(btVector3(fromVector.x, fromVector.y, fromVector.z), btVector3(toVector.x, toVector.y, toVector.z));
	trackWorld->rayTest(btVector3(fromVector.x, fromVector.y, fromVector.z), btVector3(toVector.x, toVector.y, toVector.z), RayCallback);
	if(RayCallback.hasHit())
	{
		return true;
	}
	return false;
}