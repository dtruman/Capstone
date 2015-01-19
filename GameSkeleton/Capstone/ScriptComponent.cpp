#include "ScriptComponent.h"
#include "DebugMenu.h"

ScriptComponent::ScriptComponent(std::string passScript)
{
	script=passScript;
	numVariables=0;
}

void ScriptComponent::Initialize(ObjectHandle* oh, int menuNum)
{
	parent=oh;
#if EDITOR_ON
	debugMenu.basicTextField(&script, "Script: ", "Script", menuNum);
#endif
	type="Script";
}

void ScriptComponent::InitializeLight(AmbientLight* lh, int menuNum)
{
	//need a link to parent
#if EDITOR_ON
	debugMenu.basicTextField(&script, "Script: ", "Script", menuNum);
#endif
	type="Script";
}

void ScriptComponent::InitializeDLight(DiffuseLight* lh, int menuNum)
{
	//need a link to parent
#if EDITOR_ON
	debugMenu.basicTextField(&script, "Script: ", "Script", menuNum);
#endif
	type="Script";
}

void ScriptComponent::InitializeInput(ObjectHandle* oh)
{
	parent=oh;
}

void ScriptComponent::update(float dt)
{
	if(!firstRun)
	{
		for(int i=0; i<parent->parent->numVariables; i++)
		{
			parent->parent->globalVariables[i].value="";
			parent->parent->globalVariables[i].variableName="";
			parent->parent->numVariables=0;
		}
		for(int i=0; i<parent->numVariables; i++)
		{
			parent->objectVariables[i].value="";
			parent->objectVariables[i].variableName="";
			parent->numVariables=0;
		}
		for(int i=0; i<numVariables; i++)
		{
			myVariables[i].value="";
			myVariables[i].variableName="";
			numVariables=0;
		}
	}
	scriptUpdate(script);
}

void ScriptComponent::scriptUpdate(std::string script)
{
	int track=0;
	std::string scriptTrack=script;
	replaceAll(scriptTrack, " ", "");
	replaceAll(scriptTrack, "\n", "");
	replaceAll(scriptTrack,"\t", "");
	std::string scriptChecker="";
	while(track<scriptTrack.size())
	{
		scriptChecker+=scriptTrack.at(track++);

		if(scriptChecker=="this.position.setX")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleSetX(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.position.setY")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetY(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.position.setZ")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetZ(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.scale.setX")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetScaleX(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.scale.setY")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetScaleY(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.scale.setZ")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetScaleZ(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.velocity.setX")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetVelocityX(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.velocity.setY")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetVelocityY(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.velocity.setZ")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetVelocityZ(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.rotation.setYaw")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleSetRotationX(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.rotation.setPitch")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleSetRotationY(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.rotation.setRoll")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleSetRotationZ(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.position.setAll")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetAll(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.velocity.setAll")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetVelocityAll(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.scale.setAll")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetScaleAll(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.force.setX")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetForceX(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.force.setY")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetForceY(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.force.setZ")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetForceZ(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.force.setAll")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleSetForceAll(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.rotation.setAll")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleSetRotationAll(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="var")
		{
			std::string pass=scriptTrack.substr(track, scriptTrack.find(';')-(track-1));
			track+=pass.length();
			handleVariableDeclaration(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="if")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			if(handleTrueCheck(pass))
			{
				std::string pass=getMiddleString('{','}', scriptTrack.substr(track, scriptTrack.length()));
				scriptUpdate(pass);
				track+=pass.length()+1;
			}
			scriptChecker="";
		}
		else if(scriptChecker=="while")
		{
			std::string pass=getMiddleString('(',')', scriptTrack.substr(track, scriptTrack.length()));
			int whileTrack=0;
			whileTrack=track+pass.length()+1;
			while(handleTrueCheck(pass))
			{
				pass=getMiddleString('{','}', scriptTrack.substr(whileTrack, scriptTrack.length()));
				scriptUpdate(pass);
				pass=getMiddleString('(',')', scriptTrack.substr(track, scriptTrack.length()));
			}
			pass=getMiddleString('{','}',scriptTrack.substr(whileTrack, scriptTrack.length()));
			track+=whileTrack+pass.length()+1;
			scriptChecker="";
		}
		else if(scriptChecker=="other.instanceObjectForce")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleGrabObject(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="game.scene.set")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;

			handleSetLvl(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.delete")
		{
			track+=2;
			parent->deleteObject=true;
			scriptChecker="";
		}
		else if(scriptChecker=="this.rotation.stop")
		{
			track+=2;
			parent->stopRotating();
			scriptChecker="";
		}
		else if(scriptChecker=="this.object.stop")
		{
			track+=2;
			parent->stopObject();
			scriptChecker="";
		}
		else if(scriptChecker=="this.maintain.force")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleMaintainForce(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.maintain.velocity")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleMaintainVelocity(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.maintain.xVelocity")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleMaintainXVelocity(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="this.maintain.zVelocity")
		{
			std::string pass=getMiddleString('(', ')', scriptTrack.substr(track, scriptTrack.length()));
			track+=pass.length()+1;
			handleMaintainZVelocity(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="globalVar")
		{
			std::string pass=scriptTrack.substr(track, scriptTrack.find(';')-(track-1));
			track+=pass.length();
			parent->parent->globalVariableHandle(pass);
			scriptChecker="";
		}
		else if(scriptChecker=="objectVar")
		{
			std::string pass=scriptTrack.substr(track, scriptTrack.find(';')-(track-1));
			track+=pass.length();
			parent->objectVariableHandle(pass);
			scriptChecker="";
		}
		for(int i=0; i<parent->parent->numVariables; i++)
		{
			if(scriptChecker==parent->parent->globalVariables[i].variableName)
			{
				std::string pass=scriptTrack.substr(track+(parent->parent->globalVariables[i].variableName.size()+1), scriptTrack.find(';')-1);
				if(isNumber(parent->parent->globalVariables[i].value))
				{
					parent->parent->globalVariables[i].value=std::to_string(handleNumbers(pass));
				}
				else
				{
					parent->parent->globalVariables[i].value+=pass;
				}
			}
		}
		for(int i=0; i<parent->numVariables; i++)
		{
			if(scriptChecker==parent->objectVariables[i].variableName)
			{
				std::string pass=scriptTrack.substr(track+(parent->parent->globalVariables[i].variableName.size()+1), scriptTrack.find(';')-1);
				if(isNumber(parent->objectVariables[i].value))
				{
					parent->objectVariables[i].value=std::to_string(handleNumbers(pass));
				}
				else
				{
					parent->objectVariables[i].value+=pass;
				}
			}
		}
		for(int i=0; i<numVariables; i++)
		{
			if(scriptChecker==parent->parent->globalVariables[i].variableName)
			{
				std::string pass=scriptTrack.substr(track+(parent->parent->globalVariables[i].variableName.size()+1), scriptTrack.find(';')-1);
				if(isNumber(parent->objectVariables[i].value))
				{
					parent->objectVariables[i].value=std::to_string(handleNumbers(pass));
				}
				else
				{
					parent->objectVariables[i].value+=pass;
				}
			}
		}
	}

	firstRun=false;
}

float ScriptComponent::handleNumbers(std::string parse)
{
	float ret=0;
	int mathProperty=0;
	bool tryNum=false;
	std::string track;

	while(parse.find('(')!=std::string::npos)
	{
		float re=handleNumbers(getMiddleString('(', ')', parse));
		std::string replacement=getMiddleString('(', ')', parse);
		replacement="("+replacement;
		replaceAll(parse, replacement, std::to_string(re));
	}

	while(parse.find('*')!=std::string::npos || parse.find('/')!=std::string::npos)
	{
		int position;
		if(parse.find('*')!=std::string::npos && parse.find('/')!=std::string::npos)
		{
			if(parse.find('*')<parse.find('/'))
			{
				position=parse.find('*');
			}
			else
			{
				position=parse.find('/');
			}
		}
		else if(parse.find('*')!=std::string::npos)
		{
			position=parse.find('*');
		}
		else
		{
			position=parse.find('/');
		}

		std::string firstParam="";
		std::string secondParam="";

		int i=-1;

		while((position+i)>(-1)&& parse.at(position+i)!='+' && parse.at(position+i)!='-')
		{
			firstParam=parse.at(position+i)+firstParam;
			i--;
		}

		i=1;

		while(parse.at(position+i)!='+' && parse.at(position+i)!='-' && parse.at(position+i)!='*' && parse.at(position+i)!='/' && parse.at(position+i)!=',' && parse.at(position+i)!=')')
		{
			secondParam=secondParam+parse.at(position+i);
			i++;
		}

		float get=handleEquations(parse.at(position), firstParam, secondParam);
		std::string from=firstParam+parse.at(position)+secondParam;
		replaceAll(parse, from, std::to_string(get));
	}

	while(parse.find('+')!=std::string::npos || (parse.find('-')!=std::string::npos && parse.find('-')!=0))
	{
		int position;
		if(parse.find('+')!=std::string::npos && parse.find('-')!=std::string::npos)
		{
			if(parse.find('+')<parse.find('-'))
			{
				position=parse.find('+');
			}
			else
			{
				position=parse.find('-');
			}
		}
		else if(parse.find('+')!=std::string::npos)
		{
			position=parse.find('+');
		}
		else
		{
			position=parse.find('-');
		}

		std::string firstParam="";
		std::string secondParam="";

		int i=-1;

		while((position+i)>(-1))
		{
			firstParam=parse.at(position+i)+firstParam;
			i--;
		}

		i=1;

		while(parse.at(position+i)!='+' && parse.at(position+i)!='-' && parse.at(position+i)!=',' && parse.at(position+i)!=')' && parse.at(position+i)!=';')
		{
			secondParam=secondParam+parse.at(position+i);
			i++;
		}

		float get=handleEquations(parse.at(position), firstParam, secondParam);
		std::string from=firstParam+parse.at(position)+secondParam;
		replaceAll(parse, from, std::to_string(get));
	}

	if(parse.find(',')!=std::string::npos)
		track=parse.substr(0, parse.length()-1);
	else if(parse.find(')')==parse.length()-1)
		track=parse.substr(0, parse.length()-1);
	else
		track=parse.substr(0, parse.length());

	if(isNumber(track))
	{
		ret+=std::stof(track);
	}
	else if(track=="this.getPosition.X")
	{
		ret+=parent->realPosition.x;
	}
	else if(track=="this.getPosition.Y")
	{
		ret+=parent->realPosition.y;
	}
	else if(track=="this.getPosition.Z")
	{
		ret+=parent->realPosition.z;
	}
	else if(track=="this.getScale.X")
	{
		ret+=parent->realScale.x;
	}
	else if(track=="this.getScale.Y")
	{
		ret+=parent->realScale.y;
	}
	else if(track=="this.getScale.Z")
	{
		ret+=parent->realScale.z;
	}
	else if(track=="this.getVelocity.X")
	{
		ret+=parent->xVelocity;
	}
	else if(track=="this.getVelocity.Y")
	{
		ret+=parent->yVelocity;
	}
	else if(track=="this.getVelocity.Z")
	{
		ret+=parent->zVelocity;
	}
	else if(track=="this.getForce.X")
	{
		ret+=parent->xForce;
	}
	else if(track=="this.getForce.Y")
	{
		ret+=parent->yForce;
	}
	else if(track=="this.getForce.Z")
	{
		ret+=parent->zForce;
	}
	else if(track=="this.getForward.X")
	{
		ret+=parent->tempForward.x;
	}
	else if(track=="this.getForward.Y")
	{
		ret+=parent->tempForward.y;
	}
	else if(track=="this.getForward.Z")
	{
		ret+=parent->tempForward.z;
	}

	for(int k=0; k<numVariables; k++)
	{
		std::string check="";
		for(int i=0; i<parse.length()-1; i++)
		{
			check+=parse[i];
		}

		if(myVariables[k].variableName==check)
		{
			ret+=std::stof(myVariables[k].value);
		}
	}

	for(int k=0; k<parent->numVariables; k++)
	{
		std::string check="";
		for(int i=0; i<parse.length()-1; i++)
		{
			check+=parse[i];
		}

		if(parent->objectVariables[k].variableName==check)
		{
			ret+=std::stof(parent->objectVariables[k].value);
		}
	}

	for(int k=0; k<parent->parent->numVariables; k++)
	{
		std::string check="";
		for(int i=0; i<parse.length()-1; i++)
		{
			check+=parse[i];
		}

		if(parent->parent->globalVariables[k].variableName==check)
		{
			ret+=std::stof(parent->parent->globalVariables[k].value);
		}
	}
	return ret;
}

float ScriptComponent::handleEquations(char sign, std::string firstParam, std::string secondParam)
{
	float numOne=0;
	float numTwo=0;

	if(isNumber(firstParam))
	{
		numOne+=std::stof(firstParam);
	}
	else if(firstParam=="this.getPosition.X")
	{
		numOne+=parent->realPosition.x;
	}
	else if(firstParam=="this.getPosition.Y")
	{
		numOne+=parent->realPosition.y;
	}
	else if(firstParam=="this.getPosition.Z")
	{
		numOne+=parent->realPosition.z;
	}
	else if(firstParam=="this.getScale.X")
	{
		numOne+=parent->realScale.x;
	}
	else if(firstParam=="this.getScale.Y")
	{
		numOne+=parent->realScale.y;
	}
	else if(firstParam=="this.getScale.Z")
	{
		numOne+=parent->realScale.z;
	}
	else if(firstParam=="this.getVelocity.X")
	{
		numOne+=parent->xVelocity;
	}
	else if(firstParam=="this.getVelocity.Y")
	{
		numOne+=parent->yVelocity;
	}
	else if(firstParam=="this.getVelocity.Z")
	{
		numOne+=parent->zVelocity;
	}
	else if(firstParam=="this.getForce.X")
	{
		numOne+=parent->xForce;
	}
	else if(firstParam=="this.getForce.Y")
	{
		numOne+=parent->yForce;
	}
	else if(firstParam=="this.getForce.Z")
	{
		numOne+=parent->zForce;
	}
	else if(firstParam=="this.getForward.X")
	{
		numOne+=parent->tempForward.x;
	}
	else if(firstParam=="this.getForward.Y")
	{
		numOne+=parent->tempForward.y;
	}
	else if(firstParam=="this.getForward.Z")
	{
		numOne+=parent->tempForward.z;
	}

	for(int k=0; k<numVariables; k++)
	{
		if(myVariables[k].variableName==firstParam)
		{
			numOne+=std::stof(myVariables[k].value);
		}
	}
	for(int k=0; k<parent->numVariables; k++)
	{
		if(parent->objectVariables[k].variableName==firstParam)
		{
			numOne+=std::stof(parent->objectVariables[k].value);
		}
	}
	for(int k=0; k<parent->parent->numVariables; k++)
	{
		if(parent->parent->globalVariables[k].variableName==firstParam)
		{
			numOne+=std::stof(parent->parent->globalVariables[k].value);
		}
	}

	if(isNumber(secondParam))
	{
		numTwo+=std::stof(secondParam);
	}
	else if(secondParam=="this.getPosition.X")
	{
		numTwo+=parent->realPosition.x;
	}
	else if(secondParam=="this.getPosition.Y")
	{
		numTwo+=parent->realPosition.y;
	}
	else if(secondParam=="this.getPosition.Z")
	{
		numTwo+=parent->realPosition.z;
	}
	else if(secondParam=="this.getScale.X")
	{
		numTwo+=parent->realScale.x;
	}
	else if(secondParam=="this.getScale.Y")
	{
		numTwo+=parent->realScale.y;
	}
	else if(secondParam=="this.getScale.Z")
	{
		numTwo+=parent->realScale.z;
	}
	else if(secondParam=="this.getVelocity.X")
	{
		numTwo+=parent->xVelocity;
	}
	else if(secondParam=="this.getVelocity.Y")
	{
		numTwo+=parent->yVelocity;
	}
	else if(secondParam=="this.getVelocity.Z")
	{
		numTwo+=parent->zVelocity;
	}
	else if(secondParam=="this.getForce.X")
	{
		numTwo+=parent->xForce;
	}
	else if(secondParam=="this.getForce.Y")
	{
		numTwo+=parent->yForce;
	}
	else if(secondParam=="this.getForce.Z")
	{
		numTwo+=parent->zForce;
	}
	else if(secondParam=="this.getForward.X")
	{
		numTwo+=parent->tempForward.x;
	}
	else if(secondParam=="this.getForward.Y")
	{
		numTwo+=parent->tempForward.y;
	}
	else if(secondParam=="this.getForward.Z")
	{
		numTwo+=parent->tempForward.z;
	}

	for(int k=0; k<numVariables; k++)
	{
		if(myVariables[k].variableName==secondParam)
		{
			numTwo+=std::stof(myVariables[k].value);
		}
	}
	for(int k=0; k<parent->numVariables; k++)
	{
		if(parent->objectVariables[k].variableName==secondParam)
		{
			numTwo+=std::stof(parent->objectVariables[k].value);
		}
	}
	for(int k=0; k<parent->parent->numVariables; k++)
	{
		if(parent->parent->globalVariables[k].variableName==secondParam)
		{
			numTwo+=std::stof(parent->parent->globalVariables[k].value);
		}
	}

	float ret=0;
	switch(sign)
	{
	case '+':
		ret=numOne+numTwo;
		break;
	case '-':
		ret=numOne-numTwo;
		break;
	case '/':
		ret=numOne/numTwo;
		break;
	case '*':
		ret=numOne*numTwo;
		break;
	}

	return ret;
}

void ScriptComponent::handleGrabObject(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->IO[parent->numInstances].instanceName=(passedIn.substr(0, passedIn.find(',')));
	replaceAll(parent->IO[parent->numInstances].instanceName,"\"", "");

	passedIn=passedIn.substr(passedIn.find(',')+1, passedIn.length());


	parent->IO[parent->numInstances].instancePosition.x=(handleNumbers(passedIn.substr(0, passedIn.find(',')+1)));

	passedIn=passedIn.substr(passedIn.find(',')+1, passedIn.length());


	parent->IO[parent->numInstances].instancePosition.y=(handleNumbers(passedIn.substr(0, passedIn.find(',')+1)));


	passedIn=passedIn.substr(passedIn.find(',')+1, passedIn.length());


	parent->IO[parent->numInstances].instancePosition.z=(handleNumbers(passedIn.substr(0, passedIn.find(',')+1)));

	passedIn=passedIn.substr(passedIn.find(',')+1, passedIn.length());

	parent->IO[parent->numInstances].instanceForce.x=(handleNumbers(passedIn.substr(0, passedIn.find(',')+1)));

	passedIn=passedIn.substr(passedIn.find(',')+1, passedIn.length());

	parent->IO[parent->numInstances].instanceForce.y=(handleNumbers(passedIn.substr(0, passedIn.find(',')+1)));

	passedIn=passedIn.substr(passedIn.find(',')+1, passedIn.length());

	parent->IO[parent->numInstances++].instanceForce.z=(handleNumbers(passedIn.substr(0, passedIn.find(')')+1)));
}

bool ScriptComponent::handleTrueCheck(std::string passedIn)
{
	std::string checkTrue="";
	int track=0;
	bool check;

	while(track<passedIn.length())
	{
		checkTrue+=passedIn.at(track++);

		if(checkTrue=="Equals")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=handleEquals(pass);
			checkTrue="";

			if((track+1)<passedIn.length())
			{
				checkTrue+=passedIn.at(track++);
				checkTrue+=passedIn.at(track++);

				if(checkTrue!="&&")
				{
					return check;
				}
				checkTrue="";
			}
			if(!check)
			{
				return check;
			}
		}
		else if(checkTrue=="NotEquals")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=handleNotEquals(pass);
			checkTrue="";

			if((track+1)<passedIn.length())
			{
				checkTrue+=passedIn.at(track++);
				checkTrue+=passedIn.at(track++);

				if(checkTrue!="&&")
				{
					return check;
				}
				checkTrue="";
			}
			if(!check)
			{
				return check;
			}
		}
		else if(checkTrue=="this.afterAnyContact")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=parent->parent->afterAnyContact(parent->nameID);
			checkTrue="";
		
			if((track+1)<passedIn.length())
			{
				checkTrue+=passedIn.at(track++);
				checkTrue+=passedIn.at(track++);
		
				if(checkTrue!="&&")
				{
					return check;
				}
				checkTrue="";
			}
			if(!check)
			{
				return check;
			}
		}
		else if(checkTrue=="this.anyContact")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=parent->parent->detectAnyContact(parent->nameID);
			checkTrue="";
		
			if((track+1)<passedIn.length())
			{
				checkTrue+=passedIn.at(track++);
				checkTrue+=passedIn.at(track++);
		
				if(checkTrue!="&&")
				{
					return check;
				}
				checkTrue="";
			}
			if(!check)
			{
				return check;
			}
		}
		else if(checkTrue=="this.oneContact")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=parent->parent->detectContacts(parent->nameID, pass);
			checkTrue="";
		
			if((track+1)<passedIn.length())
			{
				checkTrue+=passedIn.at(track++);
				checkTrue+=passedIn.at(track++);
		
				if(checkTrue!="&&")
				{
					return check;
				}
				checkTrue="";
			}
			if(!check)
			{
				return check;
			}
		}
		else if(checkTrue=="this.oneContactContains")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=parent->parent->detectContactsContaining(parent->nameID, pass);
			checkTrue="";
		
			if((track+1)<passedIn.length())
			{
				checkTrue+=passedIn.at(track++);
				checkTrue+=passedIn.at(track++);
		
				if(checkTrue!="&&")
				{
					return check;
				}
				checkTrue="";
			}
			if(!check)
			{
				return check;
			}
		}
		else if(checkTrue=="this.raycast")
		{
			std::string pass=getMiddleString('(', ')', passedIn.substr(track, passedIn.length()));
			track+=pass.length()+1;
			check=parent->raycast(glm::vec3(handleNumbers(pass.substr(0, pass.find(","))), handleNumbers(pass.substr(pass.find(","), pass.find_last_of(","))), handleNumbers(pass.substr(pass.find_last_of(","), pass.length()))),parent->realPosition);
			checkTrue="";
		}
	}

	return check;
}

bool ScriptComponent::handleNotEquals(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return false;
	}

	std::string variable="";
	std::string checkOne=passedIn.substr(0, passedIn.find(','));
	std::string checkTwo=passedIn.substr(passedIn.find(',')+1, (passedIn.find(')')-2));

	if(isNumber(checkOne))
	{
		int switchNum=handleNumbers(checkOne);
		checkOne=std::to_string(switchNum);
	}
	else
	{
		replaceAll(checkOne, "\"", "");
	}
	if(isNumber(checkTwo))
	{
		int switchNum=handleNumbers(checkTwo);
		checkTwo=std::to_string(switchNum);
	}
	else
	{
		checkTwo=checkTwo.substr(0, checkTwo.length()-1);
		replaceAll(checkTwo,"\"","");
	}
	
	if(checkOne==checkTwo)
	{
		return false;
	}
	else
	{
		for(int i=0; i<numVariables; i++)
		{
			if(myVariables[i].variableName==checkOne)
			{
				variable=myVariables[i].value;

				if(variable!=checkTwo)
				{
					return true;
				}
			}
		}

		for(int i=0; i<numVariables; i++)
		{
			if(myVariables[i].variableName==checkTwo)
			{
				variable=myVariables[i].value;

				if(variable!=checkOne)
				{
					return true;
				}
			}
		}

		for(int i=0; i<numVariables; i++)
		{
			if(myVariables[i].variableName==checkOne)
			{
				variable=myVariables[i].value;

				std::string variable2="";

				for(int j=0; j<numVariables; j++)
				{
					if(myVariables[i].variableName==checkTwo)
					{
						variable2=myVariables[i].value;

						if(variable!=variable2)
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool ScriptComponent::handleEquals(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return false;
	}

	std::string variable="";
	std::string checkOne=passedIn.substr(0, passedIn.find(','));
	std::string checkTwo=passedIn.substr(passedIn.find(',')+1, (passedIn.find(')')-2));

	if(isNumber(checkOne))
	{
		int switchNum=handleNumbers(checkOne);
		checkOne=std::to_string(switchNum);
	}
	else
	{
		replaceAll(checkOne, "\"", "");
	}
	if(isNumber(checkTwo))
	{
		int switchNum=handleNumbers(checkTwo);
		checkTwo=std::to_string(switchNum);
	}
	else
	{
		checkTwo=checkTwo.substr(0, checkTwo.length()-1);
		replaceAll(checkTwo,"\"","");
	}
	
	if(checkOne==checkTwo)
	{
		return true;
	}
	else
	{
		for(int i=0; i<numVariables; i++)
		{
			if(myVariables[i].variableName==checkOne)
			{
				variable=myVariables[i].value;

				if(variable==checkTwo)
				{
					return true;
				}
			}
		}

		for(int i=0; i<numVariables; i++)
		{
			if(myVariables[i].variableName==checkTwo)
			{
				variable=myVariables[i].value;

				if(variable==checkOne)
				{
					return true;
				}
			}
		}

		for(int i=0; i<numVariables; i++)
		{
			if(myVariables[i].variableName==checkOne)
			{
				variable=myVariables[i].value;

				std::string variable2="";

				for(int j=0; j<numVariables; j++)
				{
					if(myVariables[i].variableName==checkTwo)
					{
						variable2=myVariables[i].value;

						if(variable==variable2)
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

void ScriptComponent::handleVariableDeclaration(std::string passedIn)
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
		if(myVariables[i].variableName==newVariable.substr(0, newVariable.find("=")))
		{
			alreadyAVariable=true;
			which=i;
		}
	}

	if(!alreadyAVariable)
	{
		myVariables[numVariables].variableName=newVariable.substr(0, newVariable.find("="));
		myVariables[numVariables++].value=newVariable.substr(newVariable.find("=")+1, newVariable.length());
	}
	else
	{
		//myVariables[which].value=newVariable.substr(newVariable.find("=")+1, newVariable.length());
	}
}

void ScriptComponent::handleSetLvl(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->switchScene=handleNumbers(passedIn);
}

void ScriptComponent::handleSetX(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setX(handleNumbers(passedIn));
}

void ScriptComponent::handleSetY(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setY(handleNumbers(passedIn));
}

void ScriptComponent::handleSetZ(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setZ(handleNumbers(passedIn));
}

void ScriptComponent::handleSetScaleX(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->scale.x=(handleNumbers(passedIn));
}

void ScriptComponent::handleSetScaleY(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->scale.y=(handleNumbers(passedIn));
}

void ScriptComponent::handleSetScaleZ(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->scale.z=(handleNumbers(passedIn));
}

void ScriptComponent::handleSetVelocityX(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setXVelocity(handleNumbers(passedIn));
}

void ScriptComponent::handleSetVelocityY(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setYVelocity(handleNumbers(passedIn));
}

void ScriptComponent::handleSetVelocityZ(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setZVelocity(handleNumbers(passedIn));
}

void ScriptComponent::handleSetRotationX(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setYaw(handleNumbers(passedIn));
}
void ScriptComponent::handleSetRotationY(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setPitch(handleNumbers(passedIn));
}
void ScriptComponent::handleSetRotationZ(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setRoll(handleNumbers(passedIn));
}
void ScriptComponent::handleSetRotationAll(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setYaw(handleNumbers(passedIn.substr(0, passedIn.find(','))));
	parent->setPitch(handleNumbers(passedIn.substr(passedIn.find(',')+1, passedIn.find_last_of(','))));
	parent->setRoll(handleNumbers(passedIn.substr(passedIn.find_last_of(',')+1, passedIn.find_last_of(')'))));
}

void ScriptComponent::handleSetAll(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setX(handleNumbers(passedIn.substr(0, passedIn.find(','))));
	parent->setY(handleNumbers(passedIn.substr(passedIn.find(',')+1, passedIn.find_last_of(','))));
	parent->setZ(handleNumbers(passedIn.substr(passedIn.find_last_of(',')+1, passedIn.find_last_of(')'))));
}

void ScriptComponent::handleSetScaleAll(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setXScale(handleNumbers(passedIn.substr(0, passedIn.find(','))));
	parent->setYScale(handleNumbers(passedIn.substr(passedIn.find(',')+1, passedIn.find_last_of(','))));
	parent->setZScale(handleNumbers(passedIn.substr(passedIn.find_last_of(',')+1, passedIn.find_last_of(')'))));
}

void ScriptComponent::handleSetVelocityAll(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setXVelocity(handleNumbers(passedIn.substr(0, passedIn.find(','))));
	parent->setYVelocity(handleNumbers(passedIn.substr(passedIn.find(',')+1, passedIn.find_last_of(','))));
	parent->setZVelocity(handleNumbers(passedIn.substr(passedIn.find_last_of(',')+1, passedIn.find_last_of(')'))));
}

void ScriptComponent::handleSetForceX(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setXForce(handleNumbers(passedIn));
}

void ScriptComponent::handleSetForceY(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setYForce(handleNumbers(passedIn));
}

void ScriptComponent::handleSetForceZ(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setZForce(handleNumbers(passedIn));
}

void ScriptComponent::handleSetForceAll(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->setXForce(handleNumbers(passedIn.substr(0, passedIn.find(','))));
	parent->setYForce(handleNumbers(passedIn.substr(passedIn.find(',')+1, passedIn.find_last_of(','))));
	parent->setZForce(handleNumbers(passedIn.substr(passedIn.find_last_of(',')+1, passedIn.find_last_of(')'))));
}

void ScriptComponent::handleMaintainForce(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->keepUpForce(handleNumbers(passedIn.substr(0, passedIn.find(')'))));
}

void ScriptComponent::handleMaintainVelocity(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->keepUpVelocity(handleNumbers(passedIn.substr(0, passedIn.find(')'))));
}

void ScriptComponent::handleMaintainXVelocity(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->keepUpXVelocity(handleNumbers(passedIn.substr(0, passedIn.find(')'))));
}
void ScriptComponent::handleMaintainZVelocity(std::string passedIn)
{
	if(passedIn.length()==0 || passedIn.find(')')==std::string::npos)
	{
		return;
	}

	parent->keepUpZVelocity(handleNumbers(passedIn.substr(0, passedIn.find(')'))));
}

std::string ScriptComponent::getMiddleString(char first, char last, std::string checkString)
{
	std::string ret="";
	int countAppearance=1;
	int track=1;

	while(countAppearance>0 && track<checkString.size())
	{
		if(checkString.at(track)==first)
		{
			countAppearance++;
		}
		if(checkString.at(track)==last)
		{
			countAppearance--;
		}

		ret+=checkString.at(track++);
	}

	return ret;
}

bool ScriptComponent::isNumber(const std::string& s)
{
	std::string check;
	for(int i = 0; i < s.length(); i++) {
        if(s[i] == ' ') {
            continue;
        } else {
            check += s[i];
        }
    }

	std::string negCheck;
	for(int i = 0; i < check.length(); i++) {
        if(check[i] == '-') {
            continue;
        } else {
            negCheck += check[i];
        }
    }

	std::string::const_iterator it=check.begin();
	while(it!=check.end() && check.find_first_not_of("0123456789.-"))
	{
			++it;
	}

	std::string::const_iterator itNeg=negCheck.begin();
	while(itNeg!=negCheck.end() && negCheck.find_first_not_of("0123456789.-"))
	{
			++itNeg;
	}

	return !check.empty() && it==check.end() && check.find("--")==check.npos && itNeg==negCheck.end() && !negCheck.empty();
}

bool ScriptComponent::insensitiveStringComparision(std::string s, std::string d)
{
	int sz=s.size();
	if(d.size()!=sz)
		return false;
	for(int i=0; i<sz; i++)
	{
		if(tolower(s[i])!=tolower(d[i]))
			return false;
	}
	return true;
}

void ScriptComponent::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	if(from.empty())
		return;
	size_t start_pos=0;
	while((start_pos=str.find(from, start_pos)) !=std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos+=to.length();
	}
}