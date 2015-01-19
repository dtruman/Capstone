#include "MyGLWindow.h"

using std::cout;
using std::vector;
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;

#define R_CS(a) reinterpret_cast<char*>(&a), sizeof(a)
#define R_C(t,a) reinterpret_cast<t>(a)

vector<GeneralGlWindow::RenderableInfo*> allShapes;

void MyGLWindow::initializeGL()
{
	glewInit();
	setMouseTracking(true);
	glEnable(GL_DEPTH_TEST);
	switchCams=true;
	runWorld=false;
	switchWorldON=false;
	switchScene=-1;

	btDefaultCollisionConfiguration* collisionConfiguration=new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher=new btCollisionDispatcher(collisionConfiguration);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
	
	btBroadphaseInterface* overlappingPairCache=new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-20,0));

#if EDITOR_ON
	debugMenu.toggleBool("Start the World: ", "World Control", &runWorld);
	debugMenu.newRow("World Control");
	debugMenu.listView("World Control");
#endif

	gwm.dynamicsWorld=dynamicsWorld;
	gwm.runWorld=&runWorld;

	turnAround=false;

	current=clock();
	t=current-clock();
	last=current;

	sendDataToHardware();

	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(0);
}

void MyGLWindow::sendDataToHardware()
{
	shaderCompileOne=GGLWindow.createShaderInfo("Shaders\\VertexShader.glsl", "Shaders\\FragmentShader.glsl");
	deferredPass=GGLWindow.createShaderInfo("Shaders\\DeferredPassVert.glsl", "Shaders\\DeferredPassFrag.glsl");
	GeneralGlWindow::ShaderInfo* ComponentShader=GGLWindow.createShaderInfo("Shaders\\ComponentVertex.glsl", "Shaders\\ComponentFragment.glsl");

	defaultTexture=GGLWindow.addTexture("Files\\default.bmp");

	setUpSphere();
	setUpCamera();

	GGLWindow.setUpDeferred();
	previousWidth=width();
	previousHeight=height();

	addAmbientLight();
}

GeneralGlWindow::GeometryInfo* MyGLWindow::inWindow()
{
	float closest=-0.999f;

	DLTVertex stackVerts[]=
	{
		vec3(+0.0f, +0.5f, closest),// 0
		vec4(0.0f,0.0f,0.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec2(+0.0f, +0.0f), // UV

		vec3(+0.5f, +0.5f, closest), // 1
		vec4(0.0f,0.0f,0.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec2(+1.0f, +0.0f), // UV

		vec3(+0.5f, +1.0f, closest), // 2
		vec4(0.0f,0.0f,0.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec2(+1.0f, +1.0f), // UV

		vec3(-0.0f, +1.0f, closest), // 3
		vec4(0.0f,0.0f,0.0f,0.0f),
		vec3(0.0f,0.0f,0.0f),
		vec2(+0.0f, +1.0f) // UV
	};

	unsigned short stackIndices[]=
	{
		0, 1, 2, 0, 2, 3 // Front
	};

	GeneralGlWindow::GeometryInfo* windowComponent=GGLWindow.addGeometry(stackVerts, 4, sizeof(stackVerts), stackIndices, 6, GL_TRIANGLES);

	return windowComponent;
}

void MyGLWindow::paintGL()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,width(),height());

	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix=glm::perspective(60.0f, ((float)width())/height(), 0.1f, 10000.0f);
	mat4 worldToViewMatrix=camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix=viewToProjectionMatrix*worldToViewMatrix;

	if(runWorld)
	{
		float pass=(float)t/CLOCKS_PER_SEC;
		dynamicsWorld->stepSimulation(pass, 1);
	}

	for(int i=0; i<diffuseLights.size(); i++)
	{
		glUseProgram(shaderCompileOne->programID);
		glBindVertexArray(sphere->whatGeometryIndex->vertexArrayID);

		fullTransformMatrix=worldToProjectionMatrix*glm::translate(diffuseLights[i]->position)*glm::scale(glm::vec3(diffuseLights[i]->size,diffuseLights[i]->size,diffuseLights[i]->size));
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gWVP", GeneralGlWindow::PT_MAT4, &fullTransformMatrix[0][0]);

		glActiveTexture(GL_TEXTURE0+GGLWindow.deferredDiffuse);
		glBindTexture(GL_TEXTURE_2D, GGLWindow.deferredDiffuse);

		GGLWindow.addTextureUniformparameter(shaderCompileOne, "gColorMap", GGLWindow.deferredDiffuse);

		glActiveTexture(GL_TEXTURE0+GGLWindow.deferredPosition);
		glBindTexture(GL_TEXTURE_2D, GGLWindow.deferredPosition);
		
		GGLWindow.addTextureUniformparameter(shaderCompileOne, "gPositionMap", GGLWindow.deferredPosition);

		glActiveTexture(GL_TEXTURE0+GGLWindow.deferredNormals);
		glBindTexture(GL_TEXTURE_2D, GGLWindow.deferredNormals);

		GGLWindow.addTextureUniformparameter(shaderCompileOne, "gNormalMap", GGLWindow.deferredNormals);

		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Base.Color", GeneralGlWindow::PT_VEC3, &diffuseLights[i]->color[0]);

		float ambInt=0.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Base.AmbientIntensity", GeneralGlWindow::PT_FLOAT, &ambInt);

		float diffInt=0.2f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Base.DiffuseIntensity", GeneralGlWindow::PT_FLOAT, &diffInt);

		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Position", GeneralGlWindow::PT_VEC3, &diffuseLights[i]->position[0]);

		float constant=0.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Atten.Constant", GeneralGlWindow::PT_FLOAT, &constant);

		float linear=1.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Atten.Linear", GeneralGlWindow::PT_FLOAT, &linear);

		float exp=0.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Atten.Exp", GeneralGlWindow::PT_FLOAT, &exp);

		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gEyeWorldPos", GeneralGlWindow::PT_VEC3, &camera.position[0]);

		glm::vec2 screenSize=glm::vec2(width(), height());
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gScreenSize", GeneralGlWindow::PT_VEC2, &screenSize[0]);

		glDrawElements(sphere->whatGeometryIndex->indexingMode, sphere->whatGeometryIndex->numIndices, GL_UNSIGNED_SHORT, (void*)sphere->whatGeometryIndex->indexOffset);

		glUseProgram(0);

		if(diffuseLights[i]->cloneLight)
		{
			diffuseLights.push_back(new DiffuseLight(diffuseLights[i]->nameID, diffuseLights[i]->position, diffuseLights[i]->color));

			diffuseLights[i]->cloneLight=false;
		}

		if(diffuseLights[i]->deleteLight)
		{
			debugMenu.removeListItem(diffuseLights[i]->nameID, "World Control");
			
			delete diffuseLights[i];
			
			diffuseLights.erase(diffuseLights.begin()+i);
		}
	}

	for(int i=0; i<ambientLights.size(); i++)
	{
		glUseProgram(shaderCompileOne->programID);
		glBindVertexArray(sphere->whatGeometryIndex->vertexArrayID);

		fullTransformMatrix=worldToProjectionMatrix*glm::translate(glm::vec3(0.0f,0.0f,0.0f))*glm::scale(glm::vec3(2000,2000,2000));
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gWVP", GeneralGlWindow::PT_MAT4, &fullTransformMatrix[0][0]);

		glActiveTexture(GL_TEXTURE0+GGLWindow.deferredDiffuse);
		glBindTexture(GL_TEXTURE_2D, GGLWindow.deferredDiffuse);

		GGLWindow.addTextureUniformparameter(shaderCompileOne, "gColorMap", GGLWindow.deferredDiffuse);

		glActiveTexture(GL_TEXTURE0+GGLWindow.deferredPosition);
		glBindTexture(GL_TEXTURE_2D, GGLWindow.deferredPosition);
		
		GGLWindow.addTextureUniformparameter(shaderCompileOne, "gPositionMap", GGLWindow.deferredPosition);

		glActiveTexture(GL_TEXTURE0+GGLWindow.deferredNormals);
		glBindTexture(GL_TEXTURE_2D, GGLWindow.deferredNormals);

		GGLWindow.addTextureUniformparameter(shaderCompileOne, "gNormalMap", GGLWindow.deferredNormals);

		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Base.Color", GeneralGlWindow::PT_VEC3, &ambientLights[i]->color[0]);

		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Base.AmbientIntensity", GeneralGlWindow::PT_FLOAT, &ambientLights[i]->intensity);

		float diffInt=0.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Base.DiffuseIntensity", GeneralGlWindow::PT_FLOAT, &diffInt);

		glm::vec3 tempPos=glm::vec3(0.0f,0.0f,0.0f);
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Position", GeneralGlWindow::PT_VEC3, &tempPos[0]);

		float constant=0.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Atten.Constant", GeneralGlWindow::PT_FLOAT, &constant);

		float linear=0.0f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Atten.Linear", GeneralGlWindow::PT_FLOAT, &linear);

		float exp=0.3f;
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gPointLight.Atten.Exp", GeneralGlWindow::PT_FLOAT, &exp);

		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gEyeWorldPos", GeneralGlWindow::PT_VEC3, &camera.position[0]);

		glm::vec2 screenSize=glm::vec2(width(), height());
		GGLWindow.addRenderableUniformParameter(shaderCompileOne, "gScreenSize", GeneralGlWindow::PT_VEC2, &screenSize[0]);

		glDrawElements(sphere->whatGeometryIndex->indexingMode, sphere->whatGeometryIndex->numIndices, GL_UNSIGNED_SHORT, (void*)sphere->whatGeometryIndex->indexOffset);

		glUseProgram(0);

		if(ambientLights[i]->cloneLight)
		{
			ambientLights.push_back(new AmbientLight(ambientLights[i]->nameID, ambientLights[i]->color, ambientLights[i]->intensity));

			ambientLights[i]->cloneLight=false;
		}

		if(ambientLights[i]->deleteLight)
		{
#if EDITOR_ON
			debugMenu.removeListItem(ambientLights[i]->nameID, "World Control");
#endif

			delete ambientLights[i];

			ambientLights.erase(ambientLights.begin()+i);
		}
	}
}

void MyGLWindow::addAmbientLight()
{
	ambientLights.push_back(new AmbientLight("Ambient Light"));
}

void MyGLWindow::addDiffuseLight()
{
	diffuseLights.push_back(new DiffuseLight("Diffuse Light"));
}

void MyGLWindow::keyPressEvent(QKeyEvent* e)
{
	repaint();
}

void MyGLWindow::mouseMoveEvent(QMouseEvent* e)
{
	if(GetAsyncKeyState(VK_RBUTTON) && !runWorld)
	{
		camera.mouseUpdate(glm::vec2(e->x(),e->y()));
	}
	else if(runWorld)
	{
		camera.mouseUpdate(glm::vec2(e->x(),e->y()));
	}
}

void MyGLWindow::myUpdate()
{
	current=clock();
	t=current-last;
	last=current;

	if(runWorld)
	{
		GetAsyncKeyState(VK_UP);
		GetAsyncKeyState(VK_DOWN);
		GetAsyncKeyState(VK_LEFT);
		GetAsyncKeyState(VK_RIGHT);
		GetAsyncKeyState(VK_SPACE);
		GetAsyncKeyState(0x51);
		GetAsyncKeyState(0x57);
		GetAsyncKeyState(0x45);
		GetAsyncKeyState(0x52);
		GetAsyncKeyState(0x54);
		GetAsyncKeyState(0x59);
		GetAsyncKeyState(0x55);
		GetAsyncKeyState(0x49);
		GetAsyncKeyState(0x4F);
		GetAsyncKeyState(0x50);
		GetAsyncKeyState(0x41);
		GetAsyncKeyState(0x53);
		GetAsyncKeyState(0x44);
		GetAsyncKeyState(0x46);
		GetAsyncKeyState(0x47);
		GetAsyncKeyState(0x48);
		GetAsyncKeyState(0x4A);
		GetAsyncKeyState(0x4B);
		GetAsyncKeyState(0x4C);
		GetAsyncKeyState(0x5A);
		GetAsyncKeyState(0x58);
		GetAsyncKeyState(0x43);
		GetAsyncKeyState(0x56);
		GetAsyncKeyState(0x42);
		GetAsyncKeyState(0x4E);
		GetAsyncKeyState(0x4D);
		GetAsyncKeyState(0x30);
		GetAsyncKeyState(0x31);
		GetAsyncKeyState(0x32);
		GetAsyncKeyState(0x33);
		GetAsyncKeyState(0x34);
		GetAsyncKeyState(0x35);
		GetAsyncKeyState(0x36);
		GetAsyncKeyState(0x37);
		GetAsyncKeyState(0x38);
		GetAsyncKeyState(0x39);
	}
	if(!runWorld)
	{
		if(GetAsyncKeyState(VK_RBUTTON))
		{
			if(GetAsyncKeyState(0x57))	//W
			{
				camera.moveForward();
			}
			if(GetAsyncKeyState(0x53))	//S
			{
				camera.moveBackward();
			}
			if(GetAsyncKeyState(0x44))	//D
			{
				camera.strafeRight();
			}
			if(GetAsyncKeyState(0x41))	//A
			{
				camera.strafeLeft();
			}
			if(GetAsyncKeyState(0x52))	//R
			{
				camera.moveUp();
			}
			if(GetAsyncKeyState(0x46))	//F
			{
				camera.moveDown();
			}
		}
	}

	if(width()!=previousWidth || height()!=previousHeight)
	{
		GGLWindow.setUpDeferred();
		previousWidth=width();
		previousHeight=height();
	}

	updateContacts();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GGLWindow.deferredID);

	GLenum buffers[]={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, buffers);
	
	lightPass();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GGLWindow.programID);
	repaint();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(switchWorldON)
	{
		runWorld=true;
		switchWorldON=false;
	}
}

void MyGLWindow::updateContacts()
{
	gwm.CS.clear();
	if(runWorld)
	{
		int numManifolds=dynamicsWorld->getDispatcher()->getNumManifolds();
	
		for(int k=0; k<numManifolds; k++)
		{
			GameWindowManager::contactSources temp;
			btPersistentManifold* contactManifold=dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(k);
			for(int i=0; i<allShapes.size(); i++)
			{
				if(contactManifold->getBody0()==allShapes[i]->collisionMesh->rigidBody)
					temp.nameOne=allShapes[i]->OH->nameID;
	
				if(contactManifold->getBody1()==allShapes[i]->collisionMesh->rigidBody)
					temp.nameTwo=allShapes[i]->OH->nameID;
			}

			for(int i=0; i<instanceShapes.size(); i++)
			{
				if(contactManifold->getBody0()==instanceShapes[i]->collisionMesh->rigidBody)
					temp.nameOne=instanceShapes[i]->OH->nameID;

				if(contactManifold->getBody1()==instanceShapes[i]->collisionMesh->rigidBody)
					temp.nameTwo=instanceShapes[i]->OH->nameID;
			}

			gwm.CS.push_back(temp);
		}
	}
}

void MyGLWindow::loadInstantModel()
{
	QString targetObj = QFileDialog::getOpenFileName(this, "Open Model");
	if(targetObj == "")
	return;

	std::string target=targetObj.toUtf8().constData();

	int pos=target.find_last_of("/");

	wchar_t result[MAX_PATH];

	std::wstring found(result, GetModuleFileNameW(NULL, result, MAX_PATH));
	std::string newPosition(found.begin(), found.end());

	int newPos=nthOccurence(newPosition, "\\", 1);

	std::string oldPosition=target;

	std::string newFile=newPosition.substr(0,newPos)+"\\Files\\" + target.substr(pos+1,target.length());
	std::string realNewFile="Files\\" + target.substr(pos+1,target.length());

	std::ifstream oldFile(target, std::ios::binary | std::ios::in);

	oldPosition=replaceAll(oldPosition, "\/", "\\");

	if((oldPosition.compare(newFile))!=0)
	{
		std::ofstream newTarget(realNewFile, std::ios::binary | std::ios::out);

		char c;
		while(oldFile.get(c))
		{
			newTarget.put(c);
		}

		newTarget.close();
		oldFile.close();
	}

	loadInstant(realNewFile);
}

void MyGLWindow::loadModel()
{
	QString targetObj = QFileDialog::getOpenFileName(this, "Open Model");
	if(targetObj == "")
	return;

	std::string target=targetObj.toUtf8().constData();

	int pos=target.find_last_of("/");

	wchar_t result[MAX_PATH];

	std::wstring found(result, GetModuleFileNameW(NULL, result, MAX_PATH));
	std::string newPosition(found.begin(), found.end());

	int newPos=nthOccurence(newPosition, "\\", 1);

	std::string oldPosition=target;

	std::string newFile=newPosition.substr(0,newPos)+"\\Files\\" + target.substr(pos+1,target.length());
	std::string realNewFile="Files\\" + target.substr(pos+1,target.length());

	std::ifstream oldFile(target, std::ios::binary | std::ios::in);

	oldPosition=replaceAll(oldPosition, "\/", "\\");

	if((oldPosition.compare(newFile))!=0)
	{
		std::ofstream newTarget(realNewFile, std::ios::binary | std::ios::out);

		char c;
		while(oldFile.get(c))
		{
			newTarget.put(c);
		}

		newTarget.close();
		oldFile.close();
	}

	loadObj(realNewFile);
}

std::string& MyGLWindow::replaceAll(std::string& context, std::string const& from, std::string const& to)
{
	std::size_t lookHere = 0;
    std::size_t foundHere;
    while((foundHere = context.find(from, lookHere)) != std::string::npos)
    {
          context.replace(foundHere, from.size(), to);
          lookHere = foundHere + to.size();
    }
    return context;
}

int MyGLWindow::nthOccurence(std::string str, std::string c, int n)
{
	int pos=str.find_last_of(c,str.length()-1);
	while(n!=0 && pos!=-1)
	{
		pos=str.find_last_of(c, pos-1);
		n--;
	}
	return pos;
}


void MyGLWindow::loadObj(std::string fileName)
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);

	if (pScene) {
        for(int i=0; i<pScene->mNumMeshes; i++)
		{
			int numIndices=pScene->mMeshes[i]->mNumFaces*3;
			int indTrack=0;
			int vertTrack=0;
			ushort* indices=new ushort[numIndices];

			DLTVertex* verts=new DLTVertex[pScene->mMeshes[i]->mNumVertices];

			//Do checks here to make sure it doesn't crash if you load a bad object
			for(int j=0; j<pScene->mMeshes[i]->mNumFaces; j++)
			{
				assert(pScene->mMeshes[i]->mFaces[j].mNumIndices==3);
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[0];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[1];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[2];

				for(int k=0; k<3; k++)
				{
					verts[vertTrack ].position.x=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack ].position.y=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack ].position.z=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					verts[vertTrack].normal.x=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack].normal.y=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack].normal.z=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					if(pScene->mMeshes[i]->HasTextureCoords(0))
					{
						verts[vertTrack].uv.x=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
						verts[vertTrack++].uv.y=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					}
				}
			}

			GeneralGlWindow::GeometryInfo* tempGeo=GGLWindow.addGeometry(verts, pScene->mMeshes[i]->mNumVertices, pScene->mMeshes[i]->mNumVertices*sizeof(DLTVertex), indices, numIndices, GL_TRIANGLES);

			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 0, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 1, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 2, GGLWindow.PT_VEC2, tempGeo->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

			GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(tempGeo, glm::vec3(0.0f,0.0f,0.0f), 0);

			dynamicsWorld->addRigidBody(tempMesh->rigidBody);

			GeneralGlWindow::RenderableInfo* tempRender=GGLWindow.addRenderable(tempGeo, glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f))), shaderCompileOne, defaultTexture, tempMesh, fileName.substr(6, fileName.size()));

			tempRender->OH->parent=&gwm;

			allShapes.push_back(tempRender);
		}
    }
    else {
        printf("Error parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
    }
}

void MyGLWindow::loadInstant(std::string fileName)
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);

	if (pScene) {
        for(int i=0; i<pScene->mNumMeshes; i++)
		{
			int numIndices=pScene->mMeshes[i]->mNumFaces*3;
			int indTrack=0;
			int vertTrack=0;
			ushort* indices=new ushort[numIndices];

			DLTVertex* verts=new DLTVertex[pScene->mMeshes[i]->mNumVertices];

			for(int j=0; j<pScene->mMeshes[i]->mNumFaces; j++)
			{
				assert(pScene->mMeshes[i]->mFaces[i].mNumIndices==3);
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[0];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[1];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[2];

				for(int k=0; k<3; k++)
				{
					verts[vertTrack ].position.x=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack ].position.y=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack ].position.z=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					verts[vertTrack].normal.x=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack].normal.y=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack].normal.z=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					verts[vertTrack].uv.x=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack++].uv.y=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
				}
			}

			GeneralGlWindow::GeometryInfo* tempGeo=GGLWindow.addGeometry(verts, pScene->mMeshes[i]->mNumVertices, pScene->mMeshes[i]->mNumVertices*sizeof(DLTVertex), indices, numIndices, GL_TRIANGLES);

			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 0, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 1, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 2, GGLWindow.PT_VEC2, tempGeo->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

			GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(tempGeo, glm::vec3(0.0f,0.0f,0.0f), 0);

			GeneralGlWindow::RenderableInfo* tempRender=GGLWindow.instancedRenderables(tempGeo, shaderCompileOne, defaultTexture, tempMesh, fileName.substr(6, fileName.size()));
			tempRender->OH->isInstance=true;

			tempRender->OH->parent=&gwm;

			instanceShapes.push_back(tempRender);
		}
    }
    else {
        printf("Error parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
    }
}

void MyGLWindow::setUpCamera()
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile("Files\\Camera.obj", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);

	if (pScene) {
        for(int i=0; i<pScene->mNumMeshes; i++)
		{
			int numIndices=pScene->mMeshes[i]->mNumFaces*3;
			int indTrack=0;
			int vertTrack=0;
			ushort* indices=new ushort[numIndices];

			DLTVertex* verts=new DLTVertex[pScene->mMeshes[i]->mNumVertices];

			for(int j=0; j<pScene->mMeshes[i]->mNumFaces; j++)
			{
				assert(pScene->mMeshes[i]->mFaces[i].mNumIndices==3);
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[0];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[1];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[2];

				for(int k=0; k<3; k++)
				{
					verts[vertTrack ].position.x=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack ].position.y=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack ].position.z=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					verts[vertTrack].normal.x=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack].normal.y=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack].normal.z=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					if(pScene->mMeshes[i]->HasTextureCoords(0))
					{
						verts[vertTrack].uv.x=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
						verts[vertTrack++].uv.y=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					}
				}
			}

			GeneralGlWindow::GeometryInfo* tempGeo=GGLWindow.addGeometry(verts, pScene->mMeshes[i]->mNumVertices, pScene->mMeshes[i]->mNumVertices*sizeof(DLTVertex), indices, numIndices, GL_TRIANGLES);

			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 0, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 1, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 2, GGLWindow.PT_VEC2, tempGeo->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

			GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(tempGeo, glm::vec3(0.0f,0.0f,0.0f), 0);

			cameraRen=GGLWindow.addLightSource(tempGeo, glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f))), deferredPass, defaultTexture, tempMesh);
		}
	}
}

void MyGLWindow::setUpSphere()
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile("Files\\sphere.obj", aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);

	if (pScene)
	{
		for(int i=0; i<pScene->mNumMeshes; i++)
		{
			int numIndices=pScene->mMeshes[i]->mNumFaces*3;
			int indTrack=0;
			int vertTrack=0;
			ushort* indices=new ushort[numIndices];

			DLTVertex* verts=new DLTVertex[pScene->mMeshes[i]->mNumVertices];

			for(int j=0; j<pScene->mMeshes[i]->mNumFaces; j++)
			{
				assert(pScene->mMeshes[i]->mFaces[i].mNumIndices==3);
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[0];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[1];
				indices[indTrack++]=pScene->mMeshes[i]->mFaces[j].mIndices[2];

				for(int k=0; k<3; k++)
				{
					verts[vertTrack ].position.x=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack ].position.y=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack ].position.z=pScene->mMeshes[i]->mVertices[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					verts[vertTrack].normal.x=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
					verts[vertTrack].normal.y=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					verts[vertTrack].normal.z=pScene->mMeshes[i]->mNormals[pScene->mMeshes[i]->mFaces[j].mIndices[k]].z;

					if(pScene->mMeshes[i]->HasTextureCoords(0))
					{
						verts[vertTrack].uv.x=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].x;
						verts[vertTrack++].uv.y=pScene->mMeshes[i]->mTextureCoords[0][pScene->mMeshes[i]->mFaces[j].mIndices[k]].y;
					}
				}
			}

			GeneralGlWindow::GeometryInfo* tempGeo=GGLWindow.addGeometry(verts, pScene->mMeshes[i]->mNumVertices, pScene->mMeshes[i]->mNumVertices*sizeof(DLTVertex), indices, numIndices, GL_TRIANGLES);

			//Light Setup
			GGLWindow.addShaderStreamedParameter(tempGeo, shaderCompileOne, 0, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);

			//Path Setup
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 0, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 1, GGLWindow.PT_VEC3, tempGeo->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
			GGLWindow.addShaderStreamedParameter(tempGeo, deferredPass, 2, GGLWindow.PT_VEC2, tempGeo->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

			GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(tempGeo, glm::vec3(0.0f,0.0f,0.0f), 0);

			sphere=GGLWindow.addLightSource(tempGeo, glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f))), shaderCompileOne, defaultTexture, tempMesh);
		}
	}
}

void MyGLWindow::loadTexture(GeneralGlWindow::RenderableInfo* ri)
{
	QString targetObj = QFileDialog::getOpenFileName(this, "Open Model");
	if(targetObj == "")
	return;

	std::string target=targetObj.toUtf8().constData();

	int pos=target.find_last_of("/");

	wchar_t result[MAX_PATH];

	std::wstring found(result, GetModuleFileNameW(NULL, result, MAX_PATH));
	std::string newPosition(found.begin(), found.end());

	int newPos=nthOccurence(newPosition, "\\", 1);

	std::string oldPosition=target;

	std::string newFile=newPosition.substr(0,newPos)+"\\Files\\" + target.substr(pos+1,target.length());
	std::string realNewFile="Files\\" + target.substr(pos+1,target.length());

	std::ifstream oldFile(target, std::ios::binary | std::ios::in);

	oldPosition=replaceAll(oldPosition, "\/", "\\");

	if((oldPosition.compare(newFile))!=0)
	{
		std::cout << "Adding model to project" << std::endl;
		std::ofstream newTarget(realNewFile, std::ios::binary | std::ios::out);

		char c;
		while(oldFile.get(c))
		{
			newTarget.put(c);
		}

		newTarget.close();
		oldFile.close();
	}

	loadTex(ri, realNewFile.c_str());
	ri->texture->textureFileName=("Files\\" + target.substr(pos+1,target.length()));
}

void MyGLWindow::saveScene(std::string sceneName)
{
	std::ofstream out(sceneName, std::ios::binary | std::ios::out);

	int writeData;
	uint connectionFileOffset=0;

	writeData=allShapes.size();
	out.write(R_CS(writeData));

	for(unsigned int i=0; i<allShapes.size(); i++)
	{
		out.write(R_CS(allShapes[i]->whatGeometryIndex->numVerts));
		out.write(R_CS(allShapes[i]->whatGeometryIndex->numIndices));
		out.write((char*)allShapes[i]->whatGeometryIndex->vertices, allShapes[i]->whatGeometryIndex->numVerts*sizeof(DLTVertex));
		out.write((char*)allShapes[i]->whatGeometryIndex->indices, allShapes[i]->whatGeometryIndex->numIndices*sizeof(ushort));
		std::string tex=allShapes[i]->texture->textureFileName;
		int nameSize=tex.length();
		out.write(R_CS(nameSize));
		out.write(tex.c_str(), nameSize);
		out.write(R_CS(allShapes[i]->where));

		nameSize=allShapes[i]->OH->nameID.length();
		out.write(R_CS(nameSize));
		out.write(allShapes[i]->OH->nameID.c_str(), nameSize);
		out.write(R_CS(allShapes[i]->OH->position));
		out.write(R_CS(allShapes[i]->OH->scale));
		out.write(R_CS(allShapes[i]->OH->restitution));
		out.write(R_CS(allShapes[i]->OH->friction));
		out.write(R_CS(allShapes[i]->OH->allowYaw));
		out.write(R_CS(allShapes[i]->OH->allowPitch));
		out.write(R_CS(allShapes[i]->OH->allowRoll));
		out.write(R_CS(allShapes[i]->OH->dynamicBool));
		float m=allShapes[i]->OH->getMass();
		out.write(R_CS(m));
		out.write(R_CS(allShapes[i]->OH->gravityOn));

		writeData=allShapes[i]->OH->components.size();
		out.write(R_CS(writeData));

		for(unsigned int j=0; j<allShapes[i]->OH->components.size(); j++)
		{
			if(allShapes[i]->OH->components[j]->type=="Input")
			{
				InputComponent tempIC=*dynamic_cast<InputComponent*>(allShapes[i]->OH->components[j]);
				
				nameSize=tempIC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempIC.type.c_str(), nameSize);
				out.write(R_CS(tempIC.updateNum));
				out.write(R_CS(tempIC.actionNum));
				nameSize=tempIC.sc->script.length();
				out.write(R_CS(nameSize));
				out.write(tempIC.sc->script.c_str(), nameSize);
			}
			else if(allShapes[i]->OH->components[j]->type=="Script")
			{
				ScriptComponent tempSC=*dynamic_cast<ScriptComponent*>(allShapes[i]->OH->components[j]);
				nameSize=tempSC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempSC.type.c_str(), nameSize);
				nameSize=tempSC.script.length();
				out.write(R_CS(nameSize));
				out.write(tempSC.script.c_str(), nameSize);
			}
			else if(allShapes[i]->OH->components[j]->type=="Player")
			{
				PlayerComponent tempPC=*dynamic_cast<PlayerComponent*>(allShapes[i]->OH->components[j]);
				nameSize=tempPC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempPC.type.c_str(), nameSize);
				out.write(R_CS(tempPC.cameraPosition));
				out.write(R_CS(tempPC.cameraDirection));
			}
			else if(allShapes[i]->OH->components[j]->type=="Path")
			{
				PathComponent tempPC=*dynamic_cast<PathComponent*>(allShapes[i]->OH->components[j]);
				nameSize=tempPC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempPC.type.c_str(), nameSize);
				out.write(R_CS(tempPC.pType));
				out.write(R_CS(tempPC.path.numNodes));
				
				for(int k=0; k<tempPC.path.numNodes; k++)
				{
					out.write(R_CS(tempPC.path.pathNodes[k].position));
				}
			}
		}
	}

	writeData=instanceShapes.size();
	out.write(R_CS(writeData));

	for(unsigned int i=0; i<instanceShapes.size(); i++)
	{
		out.write(R_CS(instanceShapes[i]->whatGeometryIndex->numVerts));
		out.write(R_CS(instanceShapes[i]->whatGeometryIndex->numIndices));
		out.write((char*)instanceShapes[i]->whatGeometryIndex->vertices, instanceShapes[i]->whatGeometryIndex->numVerts*sizeof(DLTVertex));
		out.write((char*)instanceShapes[i]->whatGeometryIndex->indices, instanceShapes[i]->whatGeometryIndex->numIndices*sizeof(ushort));
		std::string tex=instanceShapes[i]->texture->textureFileName;
		int nameSize=tex.length();
		out.write(R_CS(nameSize));
		out.write(tex.c_str(), nameSize);
		out.write(R_CS(instanceShapes[i]->where));

		nameSize=instanceShapes[i]->OH->nameID.length();
		out.write(R_CS(nameSize));
		out.write(instanceShapes[i]->OH->nameID.c_str(), nameSize);
		out.write(R_CS(instanceShapes[i]->OH->position));
		out.write(R_CS(instanceShapes[i]->OH->scale));
		out.write(R_CS(instanceShapes[i]->OH->restitution));
		out.write(R_CS(instanceShapes[i]->OH->friction));
		out.write(R_CS(instanceShapes[i]->OH->allowYaw));
		out.write(R_CS(instanceShapes[i]->OH->allowPitch));
		out.write(R_CS(instanceShapes[i]->OH->allowRoll));
		out.write(R_CS(instanceShapes[i]->OH->dynamicBool));
		float m=instanceShapes[i]->OH->getMass();
		out.write(R_CS(m));
		out.write(R_CS(instanceShapes[i]->OH->gravityOn));

		writeData=instanceShapes[i]->OH->components.size();
		out.write(R_CS(writeData));

		for(unsigned int j=0; j<instanceShapes[i]->OH->components.size(); j++)
		{
			if(instanceShapes[i]->OH->components[j]->type=="Input")
			{
				InputComponent tempIC=*dynamic_cast<InputComponent*>(instanceShapes[i]->OH->components[j]);
				
				nameSize=tempIC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempIC.type.c_str(), nameSize);
				out.write(R_CS(tempIC.updateNum));
				out.write(R_CS(tempIC.actionNum));
				nameSize=tempIC.sc->script.length();
				out.write(R_CS(nameSize));
				out.write(tempIC.sc->script.c_str(), nameSize);
			}
			else if(instanceShapes[i]->OH->components[j]->type=="Script")
			{
				ScriptComponent tempSC=*dynamic_cast<ScriptComponent*>(instanceShapes[i]->OH->components[j]);
				nameSize=tempSC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempSC.type.c_str(), nameSize);
				nameSize=tempSC.script.length();
				out.write(R_CS(nameSize));
				out.write(tempSC.script.c_str(), nameSize);
			}
			else if(instanceShapes[i]->OH->components[j]->type=="Player")
			{
				PlayerComponent tempPC=*dynamic_cast<PlayerComponent*>(instanceShapes[i]->OH->components[j]);
				nameSize=tempPC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempPC.type.c_str(), nameSize);
				out.write(R_CS(tempPC.cameraPosition));
				out.write(R_CS(tempPC.cameraDirection));
			}
			else if(instanceShapes[i]->OH->components[j]->type=="Path")
			{
				PathComponent tempPC=*dynamic_cast<PathComponent*>(instanceShapes[i]->OH->components[j]);
				nameSize=tempPC.type.length();
				out.write(R_CS(nameSize));
				out.write(tempPC.type.c_str(), nameSize);
				out.write(R_CS(tempPC.pType));
				out.write(R_CS(tempPC.path.numNodes));
				
				for(int k=0; k<tempPC.path.numNodes; k++)
				{
					out.write(R_CS(tempPC.path.pathNodes[k].position));
				}
			}
		}
	}

	writeData=diffuseLights.size();
	out.write(R_CS(writeData));

	for(unsigned int i=0; i<diffuseLights.size(); i++)
	{
		int nameSize=diffuseLights[i]->nameID.length();
		out.write(R_CS(nameSize));
		out.write(diffuseLights[i]->nameID.c_str(), nameSize);
		out.write(R_CS(diffuseLights[i]->position));
		out.write(R_CS(diffuseLights[i]->color));
		out.write(R_CS(diffuseLights[i]->size));
	}

	writeData=ambientLights.size();
	out.write(R_CS(writeData));

	for(unsigned int i=0; i<ambientLights.size(); i++)
	{
		int nameSize=ambientLights[i]->nameID.length();
		out.write(R_CS(nameSize));
		out.write(ambientLights[i]->nameID.c_str(), nameSize);
		out.write(R_CS(ambientLights[i]->color));
		out.write(R_CS(ambientLights[i]->intensity));
	}

	out.close();
}

void MyGLWindow::loadSceneFromProject(char* tempBuffer)
{
	runWorld=false;
	switchWorldON=true;
	int deleteTheseShapes=allShapes.size();
	
	for(int i=0; i<ambientLights.size(); i)
	{
#if EDITOR_ON
		debugMenu.removeListItem(ambientLights[i]->nameID, "World Control");
#endif
	
		delete ambientLights[i];
	
		ambientLights.erase(ambientLights.begin()+i);
	}
	
	for(int i=0; i<diffuseLights.size(); i)
	{
#if EDITOR_ON
		debugMenu.removeListItem(diffuseLights[i]->nameID, "World Control");
#endif
	
		delete diffuseLights[i];
	
		diffuseLights.erase(diffuseLights.begin()+i);
	}

	for(int i=0; i<deleteTheseShapes; i)
	{
#if EDITOR_ON
			debugMenu.removeListItem(allShapes[i]->OH->nameID, "World Control");
#endif
			dynamicsWorld->removeRigidBody(allShapes[i]->collisionMesh->rigidBody);
			allShapes[i]->used=false;
	
			allShapes.erase(allShapes.begin()+i);
			deleteTheseShapes--;
	}

	for(int i=0; i<instanceShapes.size(); i)
	{
#if EDITOR_ON
		debugMenu.removeListItem(instanceShapes[i]->OH->nameID, "World Control");
#endif
		instanceShapes[i]->used=false;
		instanceShapes.erase(instanceShapes.begin()+i);
	}

#if EDITOR_ON
	debugMenu.update();
#endif

	int displacement=0;

	int numOfObjects=*reinterpret_cast<int*>(tempBuffer);
	displacement+=sizeof(int);

	for(int i=0; i<numOfObjects; i++)
	{
		int numVerts=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);
		int numIndices=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);

		GeneralGlWindow::GeometryInfo* createdGeometryOne=GGLWindow.addGeometry(tempBuffer+displacement, numVerts, numVerts*sizeof(DLTVertex), (ushort*)(tempBuffer+displacement+(sizeof(DLTVertex)*numVerts)), numIndices, GL_TRIANGLES);
		displacement+=(sizeof(DLTVertex)*numVerts+sizeof(ushort)*numIndices);

		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 0, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 1, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 2, GGLWindow.PT_VEC2, createdGeometryOne->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

		int textureNameLength=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string textureName;

		if(textureNameLength>0)
		{
			textureName.append((tempBuffer+displacement), textureNameLength);
		}
		displacement+=textureName.length();

		glm::mat4 savedWhere=*reinterpret_cast<glm::mat4*>(tempBuffer+displacement);
		displacement+=sizeof(glm::mat4);

		std::string objName;

		int nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		objName.append((tempBuffer+displacement), nameLen);
		displacement+=objName.length();
		glm::vec3 position=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		glm::vec3 scale=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float restitution=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		float friction=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool aYaw=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aPitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aRoll=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool dynamic=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		float mass=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool gravSwitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);

		GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(createdGeometryOne, position, mass);

		dynamicsWorld->addRigidBody(tempMesh->rigidBody);

		GeneralGlWindow::TextureInfo* ti=GGLWindow.addTexture(textureName.c_str());

		GeneralGlWindow::RenderableInfo* tempRender=GGLWindow.addRenderable(createdGeometryOne, savedWhere, deferredPass, ti, tempMesh, objName, position, scale, restitution, friction, mass, gravSwitch, false);//, aYaw, aPitch, aRoll);

		tempRender->OH->parent=&gwm;
		tempRender->OH->dynamicBool=dynamic;

		int compNum=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);

		for(int j=0; j<compNum; j++)
		{
			nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
			displacement+=sizeof(int);
			std::string type;
			type.append((tempBuffer+displacement), nameLen);
			displacement+=type.length();

			if(type=="Script")
			{
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string script;
				script.append((tempBuffer+displacement), nameLen);
				displacement+=script.length();
				ScriptComponent* sc=new ScriptComponent(script);
				tempRender->OH->addComponent(sc);
			}
			else if(type=="Input")
			{
				int key=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int actionKey=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string actionScript;
				actionScript.append((tempBuffer+displacement), nameLen);
				displacement+=actionScript.length();
				
				int tabName=0;
				for(int i=0; i<tempRender->OH->components.size(); i++)
				{
					if(tempRender->OH->components[i]->type=="Input")
					{
						tabName++;
					}
				}

				InputComponent* ip=new InputComponent("Input"+std::to_string(tabName), key, actionKey, actionScript);
				tempRender->OH->addComponent(ip);
			}
			else if(type=="Player")
			{
				glm::vec3 camPos=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				glm::vec3 camDir=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				PlayerComponent* pc=new PlayerComponent(camPos, camDir);
				tempRender->OH->addComponent(pc);
				tempRender->OH->isPlay=true;
				switchCams=true;
			}
			else if(type=="Path")
			{
				int type=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int numNodes=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);

				PathComponent* pc=new PathComponent(type);

				tempRender->OH->addComponent(pc);
				tempRender->OH->setUpPathCOMP();

				for(int k=0; k<numNodes; k++)
				{
					PathComponent* temp=dynamic_cast<PathComponent*>(tempRender->OH->components[tempRender->OH->components.size()-1]);
					temp->addNode(*reinterpret_cast<glm::vec3*>(tempBuffer+displacement));
					displacement+=sizeof(glm::vec3);
				}
			}
		}
		allShapes.push_back(tempRender);
	}

	numOfObjects=*reinterpret_cast<int*>(tempBuffer+displacement);
	displacement+=sizeof(int);

	bool first=true;

	for(int i=0; i<numOfObjects; i++)
	{
		int numVerts=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);
		int numIndices=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);

		GeneralGlWindow::GeometryInfo* createdGeometryOne=GGLWindow.addGeometry(tempBuffer+displacement, numVerts, numVerts*sizeof(DLTVertex), (ushort*)(tempBuffer+displacement+(sizeof(DLTVertex)*numVerts)), numIndices, GL_TRIANGLES);
		displacement+=(sizeof(DLTVertex)*numVerts+sizeof(ushort)*numIndices);

		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 0, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 1, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 2, GGLWindow.PT_VEC2, createdGeometryOne->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

		int textureNameLength=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string textureName;

		if(textureNameLength>0)
		{
			textureName.append((tempBuffer+displacement), textureNameLength);
		}
		displacement+=textureName.length();

		glm::mat4 savedWhere=*reinterpret_cast<glm::mat4*>(tempBuffer+displacement);
		displacement+=sizeof(glm::mat4);

		std::string objName;
		int nameLen;

		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		objName.append((tempBuffer+displacement), nameLen);
		displacement+=objName.length();
		glm::vec3 position=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		glm::vec3 scale=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float restitution=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		float friction=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool aYaw=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aPitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aRoll=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool dynamic=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		float mass=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool gravSwitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);

		GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(createdGeometryOne, position, mass);

		dynamicsWorld->addRigidBody(tempMesh->rigidBody);

		GeneralGlWindow::TextureInfo* ti=GGLWindow.addTexture(textureName.c_str());

		GeneralGlWindow::RenderableInfo* tempRender=GGLWindow.addRenderable(createdGeometryOne, savedWhere, shaderCompileOne, ti, tempMesh, objName, position, scale, restitution, friction, mass, gravSwitch, false, aYaw, aPitch, aRoll);

		tempRender->OH->parent=&gwm;
		tempRender->OH->dynamicBool=dynamic;

		int compNum=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);

		for(int j=0; j<compNum; j++)
		{
			nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
			displacement+=sizeof(int);
			std::string type;
			type.append((tempBuffer+displacement), nameLen);
			displacement+=type.length();

			if(type=="Script")
			{
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string script;
				script.append((tempBuffer+displacement), nameLen);
				displacement+=script.length();
				ScriptComponent* sc=new ScriptComponent(script);
				tempRender->OH->addComponent(sc);
			}
			else if(type=="Input")
			{
				int key=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int actionKey=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string actionScript;
				actionScript.append((tempBuffer+displacement), nameLen);
				displacement+=actionScript.length();
				
				int tabName=0;
				for(int i=0; i<tempRender->OH->components.size(); i++)
				{
					if(tempRender->OH->components[i]->type=="Input")
					{
						tabName++;
					}
				}

				InputComponent* ip=new InputComponent("Input"+std::to_string(tabName), key, actionKey, actionScript);
				tempRender->OH->addComponent(ip);
			}
			else if(type=="Player")
			{
				glm::vec3 camPos=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				glm::vec3 camDir=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				PlayerComponent* pc=new PlayerComponent(camPos, camDir);
				tempRender->OH->addComponent(pc);
				tempRender->OH->isPlay=true;
				switchCams=true;
			}
			else if(type=="Path")
			{
				int type=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int numNodes=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);

				PathComponent* pc=new PathComponent(type);

				tempRender->OH->addComponent(pc);
				tempRender->OH->setUpPathCOMP();

				for(int k=0; k<numNodes; k++)
				{
					PathComponent* temp=dynamic_cast<PathComponent*>(tempRender->OH->components[tempRender->OH->components.size()-1]);
					temp->addNode(*reinterpret_cast<glm::vec3*>(tempBuffer+displacement));
					displacement+=sizeof(glm::vec3);
				}
			}
		}
		instanceShapes.push_back(tempRender);
	}

	numOfObjects=*reinterpret_cast<int*>(tempBuffer+displacement);
	displacement+=sizeof(int);
	int nameLen;

	for(int i=0; i<numOfObjects; i++)
	{
		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string dlName;
		dlName.append((tempBuffer+displacement), nameLen);
		displacement+=dlName.length();
		glm::vec3 position=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		glm::vec3 color=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float size=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		DiffuseLight* dl=new DiffuseLight(dlName, position, color, size);
		diffuseLights.push_back(dl);
	}

	numOfObjects=*reinterpret_cast<int*>(tempBuffer+displacement);
	displacement+=sizeof(int);

	for(int i=0; i<numOfObjects; i++)
	{
		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string alName;
		alName.append((tempBuffer+displacement), nameLen);
		displacement+=alName.length();
		glm::vec3 color=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float intensity=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		AmbientLight* al=new AmbientLight(alName, color, intensity);
		ambientLights.push_back(al);
	}
}

void MyGLWindow::loadScene(std::string scene)
{
	runWorld=false;
	switchWorldON=true;
	int deleteTheseShapes=allShapes.size();
	
	for(int i=0; i<ambientLights.size(); i)
	{
#if EDITOR_ON
		debugMenu.removeListItem(ambientLights[i]->nameID, "World Control");
#endif
	
		delete ambientLights[i];
	
		ambientLights.erase(ambientLights.begin()+i);
	}
	
	for(int i=0; i<diffuseLights.size(); i)
	{
#if EDITOR_ON
		debugMenu.removeListItem(diffuseLights[i]->nameID, "World Control");
#endif
	
		delete diffuseLights[i];
	
		diffuseLights.erase(diffuseLights.begin()+i);
	}

	for(int i=0; i<deleteTheseShapes; i)
	{
#if EDITOR_ON
			debugMenu.removeListItem(allShapes[i]->OH->nameID, "World Control");
#endif
			dynamicsWorld->removeRigidBody(allShapes[i]->collisionMesh->rigidBody);
			allShapes[i]->used=false;
	
			allShapes.erase(allShapes.begin()+i);
			deleteTheseShapes--;
	}

	for(int i=0; i<instanceShapes.size(); i)
	{
#if EDITOR_ON
		debugMenu.removeListItem(instanceShapes[i]->OH->nameID, "World Control");
#endif
		instanceShapes[i]->used=false;
		instanceShapes.erase(instanceShapes.begin()+i);
	}

	debugMenu.update();

	std::ifstream input(scene, std::ios::binary | std::ios::in);
	std::streampos begin, end;
	input.seekg(0, input.end);
	end=input.tellg();
	input.seekg(0, input.beg);
	begin=input.tellg();
	size_t numBytes=(end-begin);
	char* tempBuffer=new char[numBytes];
	input.read(tempBuffer, numBytes);

	int displacement=0;

	int numOfObjects=*reinterpret_cast<int*>(tempBuffer);
	displacement+=sizeof(int);

	bool first=true;

	for(int i=0; i<numOfObjects; i++)
	{
		int numVerts=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);
		int numIndices=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);

		GeneralGlWindow::GeometryInfo* createdGeometryOne=GGLWindow.addGeometry(tempBuffer+displacement, numVerts, numVerts*sizeof(DLTVertex), (ushort*)(tempBuffer+displacement+(sizeof(DLTVertex)*numVerts)), numIndices, GL_TRIANGLES);
		displacement+=(sizeof(DLTVertex)*numVerts+sizeof(ushort)*numIndices);

		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 0, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 1, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 2, GGLWindow.PT_VEC2, createdGeometryOne->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

		int textureNameLength=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string textureName;

		if(textureNameLength>0)
		{
			textureName.append((tempBuffer+displacement), textureNameLength);
		}
		displacement+=textureName.length();

		glm::mat4 savedWhere=*reinterpret_cast<glm::mat4*>(tempBuffer+displacement);
		displacement+=sizeof(glm::mat4);

		std::string objName;
		int nameLen;

		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		objName.append((tempBuffer+displacement), nameLen);
		displacement+=objName.length();
		glm::vec3 position=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		glm::vec3 scale=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float restitution=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		float friction=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool aYaw=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aPitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aRoll=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool dynamic=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		float mass=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool gravSwitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);

		GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(createdGeometryOne, position, mass);

		dynamicsWorld->addRigidBody(tempMesh->rigidBody);

		GeneralGlWindow::TextureInfo* ti=GGLWindow.addTexture(textureName.c_str());

		GeneralGlWindow::RenderableInfo* tempRender=GGLWindow.addRenderable(createdGeometryOne, savedWhere, shaderCompileOne, ti, tempMesh, objName, position, scale, restitution, friction, mass, gravSwitch, false, aYaw, aPitch, aRoll);

		tempRender->OH->parent=&gwm;
		tempRender->OH->dynamicBool=dynamic;

		int compNum=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);

		for(int j=0; j<compNum; j++)
		{
			nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
			displacement+=sizeof(int);
			std::string type;
			type.append((tempBuffer+displacement), nameLen);
			displacement+=type.length();

			if(type=="Script")
			{
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string script;
				script.append((tempBuffer+displacement), nameLen);
				displacement+=script.length();
				ScriptComponent* sc=new ScriptComponent(script);
				tempRender->OH->addComponent(sc);
			}
			else if(type=="Input")
			{
				int key=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int actionKey=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string actionScript;
				actionScript.append((tempBuffer+displacement), nameLen);
				displacement+=actionScript.length();
				
				int tabName=0;
				for(int i=0; i<tempRender->OH->components.size(); i++)
				{
					if(tempRender->OH->components[i]->type=="Input")
					{
						tabName++;
					}
				}

				InputComponent* ip=new InputComponent("Input"+std::to_string(tabName), key, actionKey, actionScript);
				tempRender->OH->addComponent(ip);
			}
			else if(type=="Player")
			{
				glm::vec3 camPos=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				glm::vec3 camDir=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				PlayerComponent* pc=new PlayerComponent(camPos, camDir);
				tempRender->OH->addComponent(pc);
				tempRender->OH->isPlay=true;
				switchCams=true;
			}
			else if(type=="Path")
			{
				int type=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int numNodes=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);

				PathComponent* pc=new PathComponent(type);

				tempRender->OH->addComponent(pc);
				tempRender->OH->setUpPathCOMP();

				for(int k=0; k<numNodes; k++)
				{
					PathComponent* temp=dynamic_cast<PathComponent*>(tempRender->OH->components[tempRender->OH->components.size()-1]);
					temp->addNode(*reinterpret_cast<glm::vec3*>(tempBuffer+displacement));
					displacement+=sizeof(glm::vec3);
				}
			}
		}
		allShapes.push_back(tempRender);
	}

	numOfObjects=*reinterpret_cast<int*>(tempBuffer+displacement);
	displacement+=sizeof(int);

	first=true;

	for(int i=0; i<numOfObjects; i++)
	{
		int numVerts=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);
		int numIndices=*reinterpret_cast<int*>(tempBuffer+(displacement));
		displacement+=sizeof(int);

		GeneralGlWindow::GeometryInfo* createdGeometryOne=GGLWindow.addGeometry(tempBuffer+displacement, numVerts, numVerts*sizeof(DLTVertex), (ushort*)(tempBuffer+displacement+(sizeof(DLTVertex)*numVerts)), numIndices, GL_TRIANGLES);
		displacement+=(sizeof(DLTVertex)*numVerts+sizeof(ushort)*numIndices);

		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 0, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::POSITION_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 1, GGLWindow.PT_VEC3, createdGeometryOne->vertexOffset+DLTVertex::NORMAL_OFFSET, 3, DLTVertex::STRIDE);
		GGLWindow.addShaderStreamedParameter(createdGeometryOne, deferredPass, 2, GGLWindow.PT_VEC2, createdGeometryOne->vertexOffset+DLTVertex::UV_OFFSET, 2, DLTVertex::STRIDE);

		int textureNameLength=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string textureName;

		if(textureNameLength>0)
		{
			textureName.append((tempBuffer+displacement), textureNameLength);
		}
		displacement+=textureName.length();

		glm::mat4 savedWhere=*reinterpret_cast<glm::mat4*>(tempBuffer+displacement);
		displacement+=sizeof(glm::mat4);

		std::string objName;
		int nameLen;

		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		objName.append((tempBuffer+displacement), nameLen);
		displacement+=objName.length();
		glm::vec3 position=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		glm::vec3 scale=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float restitution=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		float friction=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool aYaw=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aPitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool aRoll=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		bool dynamic=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);
		float mass=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		bool gravSwitch=*reinterpret_cast<bool*>(tempBuffer+displacement);
		displacement+=sizeof(bool);

		GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(createdGeometryOne, position, mass);

		dynamicsWorld->addRigidBody(tempMesh->rigidBody);

		GeneralGlWindow::TextureInfo* ti=GGLWindow.addTexture(textureName.c_str());

		GeneralGlWindow::RenderableInfo* tempRender=GGLWindow.addRenderable(createdGeometryOne, savedWhere, shaderCompileOne, ti, tempMesh, objName, position, scale, restitution, friction, mass, gravSwitch, false);//, aYaw, aPitch, aRoll);

		tempRender->OH->parent=&gwm;
		tempRender->OH->dynamicBool=dynamic;

		int compNum=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);

		for(int j=0; j<compNum; j++)
		{
			nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
			displacement+=sizeof(int);
			std::string type;
			type.append((tempBuffer+displacement), nameLen);
			displacement+=type.length();

			if(type=="Script")
			{
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string script;
				script.append((tempBuffer+displacement), nameLen);
				displacement+=script.length();
				ScriptComponent* sc=new ScriptComponent(script);
				tempRender->OH->addComponent(sc);
			}
			else if(type=="Input")
			{
				int key=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int actionKey=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				std::string actionScript;
				actionScript.append((tempBuffer+displacement), nameLen);
				displacement+=actionScript.length();
				
				int tabName=0;
				for(int i=0; i<tempRender->OH->components.size(); i++)
				{
					if(tempRender->OH->components[i]->type=="Input")
					{
						tabName++;
					}
				}

				InputComponent* ip=new InputComponent("Input"+std::to_string(tabName), key, actionKey, actionScript);
				tempRender->OH->addComponent(ip);
			}
			else if(type=="Player")
			{
				glm::vec3 camPos=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				glm::vec3 camDir=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
				displacement+=sizeof(glm::vec3);
				PlayerComponent* pc=new PlayerComponent(camPos, camDir);
				tempRender->OH->addComponent(pc);
				tempRender->OH->isPlay=true;
				switchCams=true;
			}
			else if(type=="Path")
			{
				int type=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);
				int numNodes=*reinterpret_cast<int*>(tempBuffer+displacement);
				displacement+=sizeof(int);

				PathComponent* pc=new PathComponent(type);

				tempRender->OH->addComponent(pc);
				tempRender->OH->setUpPathCOMP();

				for(int k=0; k<numNodes; k++)
				{
					PathComponent* temp=dynamic_cast<PathComponent*>(tempRender->OH->components[tempRender->OH->components.size()-1]);
					temp->addNode(*reinterpret_cast<glm::vec3*>(tempBuffer+displacement));
					displacement+=sizeof(glm::vec3);
				}
			}
		}
		instanceShapes.push_back(tempRender);
	}

	numOfObjects=*reinterpret_cast<int*>(tempBuffer+displacement);
	displacement+=sizeof(int);
	int nameLen;

	for(int i=0; i<numOfObjects; i++)
	{
		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string dlName;
		dlName.append((tempBuffer+displacement), nameLen);
		displacement+=dlName.length();
		glm::vec3 position=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		glm::vec3 color=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float size=*reinterpret_cast<float*>(tempBuffer+ displacement);
		displacement+=sizeof(float);
		DiffuseLight* dl=new DiffuseLight(dlName, position, color, size);
		diffuseLights.push_back(dl);
	}

	numOfObjects=*reinterpret_cast<int*>(tempBuffer+displacement);
	displacement+=sizeof(int);

	for(int i=0; i<numOfObjects; i++)
	{
		nameLen=*reinterpret_cast<int*>(tempBuffer+displacement);
		displacement+=sizeof(int);
		std::string alName;
		alName.append((tempBuffer+displacement), nameLen);
		displacement+=alName.length();
		glm::vec3 color=*reinterpret_cast<glm::vec3*>(tempBuffer+displacement);
		displacement+=sizeof(glm::vec3);
		float intensity=*reinterpret_cast<float*>(tempBuffer+displacement);
		displacement+=sizeof(float);
		AmbientLight* al=new AmbientLight(alName, color, intensity);
		ambientLights.push_back(al);
	}
}

void MyGLWindow::loadTex(GeneralGlWindow::RenderableInfo* ri, std::string fileName)
{
	GeneralGlWindow::TextureInfo* ti=GGLWindow.addTexture(fileName.c_str());
	if(ti)
		ri->texture=ti;
}

void MyGLWindow::lightPass()
{
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glViewport(0,0,width(), height());

	glClearColor(0.0f,0.0f,0.0f,1.0f);

	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix=glm::perspective(60.0f, ((float)width())/height(), 0.1f, 10000.0f);
	mat4 worldToViewMatrix=camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix=viewToProjectionMatrix*worldToViewMatrix;
	for(int selection=0; selection<allShapes.size(); selection++)
	{
		if(allShapes[selection]->OH->switchScene>=0)
		{
			switchScene=allShapes[selection]->OH->switchScene;
			allShapes[selection]->OH->switchScene=-1;
		}

		if(allShapes[selection]->OH->addTex)
		{
			loadTexture(allShapes[selection]);
			allShapes[selection]->OH->addTex=false;
		}

		if(allShapes[selection]->OH->addPlay)
		{
			for(int i=0; i<allShapes.size(); i++)
			{
				if(i!=selection && allShapes[i]->OH->isPlay)
				{
					allShapes[i]->OH->removePlayer();
				}
			}

			PlayerComponent* pc=new PlayerComponent();
			allShapes[selection]->OH->addComponent(pc);
			allShapes[selection]->OH->addPlay=false;
		}

		if(allShapes[selection]->OH->addInp)
		{
			int tabNum=0;

			for(int i=0; i<allShapes[selection]->OH->components.size(); i++)
			{
				if(allShapes[selection]->OH->components[i]->type=="Input")
				{
					tabNum++;
				}
			}

			InputComponent* ic=new InputComponent("Input"+std::to_string(tabNum));
			allShapes[selection]->OH->addComponent(ic);
			allShapes[selection]->OH->addInp=false;
		}

		if(allShapes[selection]->OH->addPa)
		{
			bool alreadyAdded=false;

			for(int i=0; i<allShapes[selection]->OH->components.size(); i++)
			{
				if(allShapes[selection]->OH->components[i]->type=="Path")
				{
					alreadyAdded=true;
				}
			}

			if(!alreadyAdded)
			{
				PathComponent* pc=new PathComponent();
				allShapes[selection]->OH->addComponent(pc);
				allShapes[selection]->OH->setUpPathCOMP();
			}
			allShapes[selection]->OH->addPa=false;
		}

		if(allShapes[selection]->OH->addScr)
		{
			bool alreadyAdded=false;

			for(int i=0; i<allShapes[selection]->OH->components.size(); i++)
			{
				if(allShapes[selection]->OH->components[i]->type=="Script")
				{
					alreadyAdded=true;
				}
			}

			if(!alreadyAdded)
			{
				ScriptComponent* sc=new ScriptComponent();
				allShapes[selection]->OH->addComponent(sc);
			}
			allShapes[selection]->OH->addScr=false;
		}

		if(allShapes[selection]->OH->addInt)
		{
			bool alreadyAdded=false;

			for(int i=0; i<allShapes[selection]->OH->components.size(); i++)
			{
				if(allShapes[selection]->OH->components[i]->type=="AI")
				{
					alreadyAdded=true;
				}
			}

			if(!alreadyAdded)
			{
				AIComponent* AI=new AIComponent();
				allShapes[selection]->OH->addComponent(AI);
			}
			allShapes[selection]->OH->addInt=false;
		}

		float pass=(float)t/CLOCKS_PER_SEC;
		allShapes[selection]->OH->objectUpdate(dynamicsWorld, allShapes[selection]->collisionMesh->rigidBody, runWorld, pass);

		for(int k=0; k<instanceShapes.size(); k++)
		{
			instanceShapes[k]->OH->objectUpdate(dynamicsWorld, instanceShapes[k]->collisionMesh->rigidBody, runWorld, pass);
		}
		for(int k=0; k<allShapes[selection]->OH->numInstances; k++)
		{
			std::string grab=allShapes[selection]->OH->IO[k].instanceName;

			for(int j=0; j<instanceShapes.size(); j++)
			{
				if(instanceShapes[j]->OH->nameID==grab)
				{
					GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(instanceShapes[k]->whatGeometryIndex, allShapes[selection]->OH->IO[k].instancePosition, instanceShapes[j]->OH->mass);
					GeneralGlWindow::RenderableInfo* temp=GGLWindow.addRenderable(instanceShapes[k]->whatGeometryIndex, glm::translate(allShapes[selection]->OH->IO[k].instancePosition), instanceShapes[j]->howShaderIndex, instanceShapes[j]->texture, tempMesh, allShapes[selection]->OH->IO[k].instanceName, allShapes[selection]->OH->IO[k].instancePosition, instanceShapes[j]->OH->scale, instanceShapes[j]->OH->restitution, instanceShapes[j]->OH->friction, instanceShapes[j]->OH->mass, instanceShapes[j]->OH->gravityOn,true, instanceShapes[j]->OH->allowYaw, instanceShapes[j]->OH->allowPitch, instanceShapes[j]->OH->allowRoll, allShapes[selection]->OH->IO[k].instanceForce);
					dynamicsWorld->addRigidBody(temp->collisionMesh->rigidBody);
					allShapes.push_back(temp);
				}
			}
		}
		if(!runWorld && allShapes[selection]->OH->isWorldInstance)
		{
			allShapes[selection]->OH->deleteObject=true;
		}

		allShapes[selection]->OH->numInstances=0;

		if(allShapes[selection]->OH->isPlay && runWorld)
		{
			allShapes[selection]->OH->camDirection=camera.viewDirection;
			lookAt=camera.position+camera.viewDirection;
			
			if(allShapes[selection]->OH->allowPitch)
				allShapes[selection]->OH->setPitch(std::atan2((lookAt.y-camera.position.y), 1));
			
			if(allShapes[selection]->OH->allowYaw)
			{
				if(std::abs(lookAt.z-allShapes[selection]->OH->oneTurn.z)>0.0001)
				{
					allShapes[selection]->OH->isTurning=true;
					allShapes[selection]->OH->oneTurn=lookAt;
				}
			
				if(std::abs(camera.position.z-allShapes[selection]->OH->twoTurn.z)>0.0001)
				{
					allShapes[selection]->OH->isTurning=true;
					allShapes[selection]->OH->twoTurn=camera.position;
				}
			
				if(allShapes[selection]->OH->isTurning)
				{
					allShapes[selection]->OH->oneTurnCheck=(-std::atan2((allShapes[selection]->OH->oneTurn.x-allShapes[selection]->OH->twoTurn.x), -(allShapes[selection]->OH->oneTurn.z-allShapes[selection]->OH->twoTurn.z)));
			
					float tempTurn=(allShapes[selection]->OH->oneTurnCheck-allShapes[selection]->OH->twoTurnCheck);
					if(allShapes[selection]->OH->oneTurnCheck-allShapes[selection]->OH->twoTurnCheck>3 || allShapes[selection]->OH->oneTurnCheck-allShapes[selection]->OH->twoTurnCheck<(-3))
						allShapes[selection]->OH->lerpTurn=1.0f;
			
					allShapes[selection]->OH->setYaw(allShapes[selection]->OH->twoTurnCheck+(tempTurn*allShapes[selection]->OH->lerpTurn));
					allShapes[selection]->OH->lerpTurn+=0.01f;
				}
			
				if(allShapes[selection]->OH->isTurning && (std::abs(1.0f-std::abs(allShapes[selection]->OH->lerpTurn))<0.0001f || std::abs(1.01f-std::abs(allShapes[selection]->OH->lerpTurn))<0.0001f))
				{
					allShapes[selection]->OH->twoTurnCheck=allShapes[selection]->OH->oneTurnCheck;
					allShapes[selection]->OH->isTurning=false;
					allShapes[selection]->OH->lerpTurn=0.0f;
				}
			}
		}

		btTransform trans;
		allShapes[selection]->collisionMesh->rigidBody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(&allShapes[selection]->where[0][0]);
		allShapes[selection]->where=allShapes[selection]->where*glm::scale(glm::vec3(allShapes[selection]->collisionMesh->shape->getLocalScaling().getX(), allShapes[selection]->collisionMesh->shape->getLocalScaling().getY(), allShapes[selection]->collisionMesh->shape->getLocalScaling().getZ()));

#if EDITOR_ON
		if(!runWorld)
		{
			for(int j=0; j<allShapes[selection]->OH->components.size(); j++)
			{
				if(allShapes[selection]->OH->components[j]->type=="Script")
				{
					ScriptComponent* temp=dynamic_cast<ScriptComponent*>(allShapes[selection]->OH->components[j]);
					temp->firstRun=true;
				}

				if(dynamic_cast<PlayerComponent*>(allShapes[selection]->OH->components[j]))
				{
					PlayerComponent* temp=dynamic_cast<PlayerComponent*>(allShapes[selection]->OH->components[j]);

					glUseProgram(cameraRen->howShaderIndex->programID);
					glBindVertexArray(cameraRen->whatGeometryIndex->vertexArrayID);

					lookAt=temp->cameraPosition+temp->cameraDirection;
					glm::vec3 originalLookAt=temp->cameraPosition+glm::vec3(0.0f,0.0f,1.0f);

					glm::mat4 originalPlayerRotation=glm::rotate(((std::atan2(temp->cameraPosition.z-lookAt.z, temp->cameraPosition.x-lookAt.x)*180)/3.14f)-90, glm::vec3(0.0f, -1.0f, 0.0f));
					yRotation=(((std::atan2(temp->cameraPosition.z-lookAt.z, temp->cameraPosition.x-lookAt.x)*180)/3.14f)-90);

					if(temp->cameraDirection.z>0)
					{
						turnAround=true;
						xRotation=(((std::atan2(temp->cameraPosition.y-lookAt.y, temp->cameraPosition.z-lookAt.z)*180)/3.14f));
						originalPlayerRotation*=glm::rotate(((std::atan2(temp->cameraPosition.y-lookAt.y, temp->cameraPosition.z-lookAt.z)*180)/3.14f), glm::vec3(1.0f,0.0f,0.0f));
						originalPlayerRotation*=glm::rotate(180.0f, glm::vec3(1.0f,0.0f,0.0f));
						originalPlayerRotation*=glm::rotate(180.0f, glm::vec3(0.0f,0.0f,1.0f));
					}
					else
					{
						xRotation=(((std::atan2(temp->cameraPosition.y-lookAt.y, temp->cameraPosition.z-lookAt.z)*180)/3.14f));
						originalPlayerRotation*=glm::rotate(((std::atan2(temp->cameraPosition.y-lookAt.y, temp->cameraPosition.z-lookAt.z)*180)/3.14f), glm::vec3(-1.0f,0.0f,0.0f));
					}

					lookAt=temp->cameraPosition;

					glActiveTexture(GL_TEXTURE0 + defaultTexture->textureID);
					glBindTexture(GL_TEXTURE_2D, defaultTexture->textureID);

					GGLWindow.addTextureUniformparameter(deferredPass, "tDiffuse", defaultTexture->textureID);

					fullTransformMatrix=worldToProjectionMatrix*glm::translate(allShapes[selection]->OH->position+temp->cameraPosition)*originalPlayerRotation;
					GGLWindow.addRenderableUniformParameter(deferredPass,"fullTransformMatrix", GeneralGlWindow::PT_MAT4, &fullTransformMatrix[0][0]);
					GGLWindow.addRenderableUniformParameter(deferredPass, "modelToWorldMatrix", GeneralGlWindow::PT_MAT4,&allShapes[selection]->where[0][0]);

					glDrawElements(cameraRen->whatGeometryIndex->indexingMode, cameraRen->whatGeometryIndex->numIndices, GL_UNSIGNED_SHORT, (void*)cameraRen->whatGeometryIndex->indexOffset);

					glUseProgram(0);
				}

				if(dynamic_cast<PathComponent*>(allShapes[selection]->OH->components[j]))
				{
					PathComponent* temp=dynamic_cast<PathComponent*>(allShapes[selection]->OH->components[j]);
					if(allShapes[selection]->OH->addNODE)
					{
						temp->addNode();

						allShapes[selection]->OH->addNODE=false;
					}

					for(int k=0; k<temp->path.numNodes; k++)
					{
						glUseProgram(deferredPass->programID);
						glBindVertexArray(sphere->whatGeometryIndex->vertexArrayID);

						glActiveTexture(GL_TEXTURE0 + defaultTexture->textureID);
						glBindTexture(GL_TEXTURE_2D, defaultTexture->textureID);

						GGLWindow.addTextureUniformparameter(deferredPass, "tDiffuse", defaultTexture->textureID);

						fullTransformMatrix=worldToProjectionMatrix*glm::translate(temp->path.pathNodes[k].position);
						GGLWindow.addRenderableUniformParameter(deferredPass,"fullTransformMatrix", GeneralGlWindow::PT_MAT4, &fullTransformMatrix[0][0]);
						GGLWindow.addRenderableUniformParameter(deferredPass, "modelToWorldMatrix", GeneralGlWindow::PT_MAT4,&allShapes[selection]->where[0][0]);

						glDrawElements(sphere->whatGeometryIndex->indexingMode, sphere->whatGeometryIndex->numIndices, GL_UNSIGNED_SHORT, (void*)sphere->whatGeometryIndex->indexOffset);

						glUseProgram(0);
					}
				}
			}
		}
#endif

		if(allShapes[selection]->OH->isPlayer() && runWorld)
		{
			if(switchCams)
			{
				oldCamPosition=camera.position;
				oldCamDirection=camera.viewDirection;
			}
			for(unsigned int i=0; i<allShapes[selection]->OH->components.size(); i++)
			{
				if(dynamic_cast<PlayerComponent*>(allShapes[selection]->OH->components[i]))
				{
					PlayerComponent* temp=dynamic_cast<PlayerComponent*>(allShapes[selection]->OH->components[i]);
					camera.position=glm::vec3(allShapes[selection]->collisionMesh->rigidBody->getWorldTransform().getOrigin().getX(), allShapes[selection]->collisionMesh->rigidBody->getWorldTransform().getOrigin().getY(), allShapes[selection]->collisionMesh->rigidBody->getWorldTransform().getOrigin().getZ())+temp->cameraPosition;
					if(switchCams)
					{
						camera.viewDirection=temp->cameraDirection;
					}

					for(unsigned int j=0; j<allShapes.size(); j++)
					{
						for(int k=0; k<allShapes[j]->OH->components.size(); k++)
						{
							if(allShapes[j]->OH->components[k]->type=="AI")
							{
								AIComponent* aiTemp=dynamic_cast<AIComponent*>(allShapes[j]->OH->components[k]);
								aiTemp->passPlayer(allShapes[selection]->OH);
							}
						}
					}
				}
			}

			switchCams=false;
		}
		else if(!runWorld && !switchCams)
		{
			camera.position=oldCamPosition;
			camera.viewDirection=oldCamDirection;
			switchCams=true;
		}

		worldToViewMatrix=camera.getWorldToViewMatrix();
		worldToProjectionMatrix=viewToProjectionMatrix*worldToViewMatrix;

		glUseProgram(deferredPass->programID);
		glBindVertexArray(allShapes[selection]->whatGeometryIndex->vertexArrayID);

		fullTransformMatrix=worldToProjectionMatrix*allShapes[selection]->where;

		GGLWindow.addRenderableUniformParameter(deferredPass,"fullTransformMatrix", GeneralGlWindow::PT_MAT4, &fullTransformMatrix[0][0]);
		GGLWindow.addRenderableUniformParameter(deferredPass, "modelToWorldMatrix", GeneralGlWindow::PT_MAT4,&allShapes[selection]->where[0][0]);

		glActiveTexture(GL_TEXTURE0 + allShapes[selection]->texture->textureID);
		glBindTexture(GL_TEXTURE_2D, allShapes[selection]->texture->textureID);

		GGLWindow.addTextureUniformparameter(deferredPass, "tDiffuse", allShapes[selection]->texture->textureID);

		glDrawElements(allShapes[selection]->whatGeometryIndex->indexingMode, allShapes[selection]->whatGeometryIndex->numIndices, GL_UNSIGNED_SHORT, (void*)allShapes[selection]->whatGeometryIndex->indexOffset);

		glUseProgram(0);

		if(allShapes[selection]->OH->cloneObject)
		{
			GeneralGlWindow::MeshInfo* tempMesh=GGLWindow.addMesh(allShapes[selection]->whatGeometryIndex, allShapes[selection]->OH->position, allShapes[selection]->OH->mass);
			GeneralGlWindow::RenderableInfo* temp=GGLWindow.addRenderable(allShapes[selection]->whatGeometryIndex, glm::translate(allShapes[selection]->OH->position), allShapes[selection]->howShaderIndex, allShapes[selection]->texture, tempMesh, allShapes[selection]->OH->nameID, allShapes[selection]->OH->position, allShapes[selection]->OH->scale, allShapes[selection]->OH->restitution, allShapes[selection]->OH->friction, allShapes[selection]->OH->mass, allShapes[selection]->OH->gravityOn);
			dynamicsWorld->addRigidBody(temp->collisionMesh->rigidBody);
			temp->OH->parent=&gwm;
			allShapes.push_back(temp);

			allShapes[selection]->OH->cloneObject=false;
		}

		if(allShapes[selection]->OH->deleteObject)
		{
			if(!allShapes[selection]->OH->isWorldInstance)
			{
#if EDITOR_ON
				debugMenu.removeListItem(allShapes[selection]->OH->nameID, "World Control");
#endif
			}

			dynamicsWorld->removeRigidBody(allShapes[selection]->collisionMesh->rigidBody);
			allShapes[selection]->used=false;

			allShapes.erase(allShapes.begin()+selection);
		}
	}

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}

MyGLWindow::~MyGLWindow()
{

}