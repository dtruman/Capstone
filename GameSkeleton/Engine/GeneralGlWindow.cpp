#include "GeneralGlWindow.h"

GeneralGlWindow::GeneralGlWindow()
{
	currentGeometry=0;
	currentShader=0;
	currentRenderer=0;
	currentTexture=0;
	change=false;
	currentMesh=0;
}

GeneralGlWindow::GeometryInfo* GeneralGlWindow::addGeometry(void* verts, GLuint numVertices, GLuint vertexDataSize, ushort* indices, uint numIndices,GLuint indexingMode)
{
	bool setBuffer=false;
	GLuint indexSize=numIndices*sizeof(ushort);

	for(int i=0; i<GeneralGlWindow::MY_NUM_BUFFERS && !setBuffer; i++)
	{
		if(myBufferInfo[i].remainingSize>(vertexDataSize+(indexSize)))
		{
			setBuffer=true;

			if(myBufferInfo[i].remainingSize==MEGABYTE_SIZE)
			{
				glGenBuffers(1, &myBufferInfo[i].glBufferID);
				glBindBuffer(GL_ARRAY_BUFFER, myBufferInfo[i].glBufferID);
				glBufferData(GL_ARRAY_BUFFER, MEGABYTE_SIZE, 0, GL_STATIC_DRAW);
			}

			glBufferSubData(GL_ARRAY_BUFFER, (GeneralGlWindow::MEGABYTE_SIZE-myBufferInfo[i].remainingSize), vertexDataSize, verts);
			geometryInfos[currentGeometry].vertexOffset=(GeneralGlWindow::MEGABYTE_SIZE-myBufferInfo[i].remainingSize);
			myBufferInfo[i].remainingSize=myBufferInfo[i].remainingSize-vertexDataSize;

			glBufferSubData(GL_ARRAY_BUFFER, (GeneralGlWindow::MEGABYTE_SIZE-myBufferInfo[i].remainingSize), indexSize, indices);
			geometryInfos[currentGeometry].indexOffset=(GeneralGlWindow::MEGABYTE_SIZE-myBufferInfo[i].remainingSize);
			myBufferInfo[i].remainingSize=myBufferInfo[i].remainingSize-indexSize;

			geometryInfos[currentGeometry].numIndices=numIndices;

			glGenVertexArrays(1, &geometryInfos[currentGeometry].vertexArrayID);

			myBufferInfo[i].remainingSize=myBufferInfo[i].remainingSize-indexSize;

			geometryInfos[currentGeometry].indexingMode=indexingMode;

			geometryInfos[currentGeometry].bufferID=myBufferInfo[i].glBufferID;
			geometryInfos[currentGeometry].indices=indices;
			geometryInfos[currentGeometry].vertices=(DLTVertex*)verts;
			geometryInfos[currentGeometry].numVerts=numVertices;
			
			GeometryInfo* ret=&geometryInfos[currentGeometry++];
			return ret;
		}
	}

	return NULL;
}

GeneralGlWindow::ShaderInfo* GeneralGlWindow::createShaderInfo(const char* vertexShaderFileName,const char* fragmentShaderFileName)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar* adapter[1];
	std::string temp=readShaderCode(vertexShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	temp=readShaderCode(fragmentShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(fragShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragShaderID);

	GLint compileStatus;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer=new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(vertexShaderID, logLength, &bitBucket, buffer);
		qDebug() << buffer;
		delete [] buffer;
	}
	compileStatus;
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer=new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(fragShaderID, logLength, &bitBucket, buffer);
		qDebug() << buffer;
		delete [] buffer;
	}

	myShaderInfo[currentShader].programID= glCreateProgram();

	glAttachShader(myShaderInfo[currentShader].programID, vertexShaderID);
	glAttachShader(myShaderInfo[currentShader].programID, fragShaderID);

	glLinkProgram(myShaderInfo[currentShader].programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragShaderID);

	glUseProgram(myShaderInfo[currentShader].programID);

	ShaderInfo* ret=&myShaderInfo[currentShader++];

	return ret;
}

std::string GeneralGlWindow::readShaderCode(const char* fileName)
{
	std::ifstream meInput(fileName);
	if(!meInput.good())
	{
		std::cout << "File failed to load... " << fileName;
		exit(1);
	}

	return std::string(std::istreambuf_iterator<char>(meInput),std::istreambuf_iterator<char>());
}

GeneralGlWindow::RenderableInfo* GeneralGlWindow::instancedRenderables(GeometryInfo* whatGeometry, ShaderInfo* howShaders, TextureInfo* texture, MeshInfo* mesh, std::string nameID, glm::vec3 sca, float rest, float frict, float m, bool grav, bool aYaw, bool aPitch, bool aRoll)
{
	renderableInfos[currentRenderer].whatGeometryIndex=whatGeometry;
	renderableInfos[currentRenderer].where=glm::mat4(0.0f);
	renderableInfos[currentRenderer].howShaderIndex=howShaders;
	renderableInfos[currentRenderer].used=true;

	if(texture!=NULL)
	{
		renderableInfos[currentRenderer].texture=texture;
	}
	if(mesh!=NULL)
	{
		renderableInfos[currentRenderer].collisionMesh=mesh;
	}

	renderableInfos[currentRenderer].OH=new ObjectHandle(nameID, glm::vec3(0.0f,0.0f,0.0f), sca, rest, frict, m, grav, aYaw, aPitch, aRoll, false);

	for(int i=2; i<currentRenderer; i++)
	{
		if(renderableInfos[i].OH && renderableInfos[i].OH->menuOn && renderableInfos[i].used)
			renderableInfos[i].OH->toggleMenu();
	}

	RenderableInfo* ret=&renderableInfos[currentRenderer++];

	return ret;
}
	
GeneralGlWindow::RenderableInfo* GeneralGlWindow::addRenderable(GeometryInfo* whatGeometry,const glm::mat4& whereMatrix,ShaderInfo* howShaders, TextureInfo* texture, MeshInfo* mesh, std::string nameID, glm::vec3 position, glm::vec3 sca, float rest, float frict, float m, bool grav, bool worldInstance, bool aYaw, bool aPitch, bool aRoll, glm::vec3 startForce)
{
	if(!worldInstance)
	{
		for(int i=2; i<currentRenderer; i++)
		{
			if(renderableInfos[i].whatGeometryIndex->numVerts==whatGeometry->numVerts && renderableInfos[i].whatGeometryIndex->numIndices==whatGeometry->numIndices && renderableInfos[i].OH->nameID==nameID && renderableInfos[i].OH->deleteObject)
			{
				RenderableInfo* ret=&renderableInfos[i];

				for(int k=2; k<currentRenderer; k++)
				{
					if(renderableInfos[k].OH && renderableInfos[k].OH->menuOn && renderableInfos[k].used)
						renderableInfos[k].OH->toggleMenu();
				}
				renderableInfos[i].OH->deleteObject=false;
				renderableInfos[i].OH->nameID=nameID;
				renderableInfos[i].OH->position=position;
				renderableInfos[i].OH->scale=sca;
				renderableInfos[i].OH->restitution=rest;
				renderableInfos[i].OH->friction=frict;
				renderableInfos[i].OH->mass=m;
				renderableInfos[i].OH->gravityOn=grav;
				renderableInfos[i].OH->isInstance=worldInstance;
				renderableInfos[i].OH->allowYaw=aYaw;
				renderableInfos[i].OH->allowPitch=aPitch;
				renderableInfos[i].OH->allowRoll=aRoll;
#if EDITOR_ON
				debugMenu.listItem(&renderableInfos[i].OH->nameID, "World Control");
#endif

				return ret;
			}
		}
	}

	renderableInfos[currentRenderer].whatGeometryIndex=whatGeometry;
	renderableInfos[currentRenderer].where=whereMatrix;
	renderableInfos[currentRenderer].howShaderIndex=howShaders;
	renderableInfos[currentRenderer].used=true;

	if(texture!=NULL)
	{
		renderableInfos[currentRenderer].texture=texture;
	}
	if(mesh!=NULL)
	{
		renderableInfos[currentRenderer].collisionMesh=mesh;
	}

	renderableInfos[currentRenderer].OH=new ObjectHandle(nameID, position, sca, rest, frict, m, aYaw, aPitch, aRoll, grav, worldInstance);

	renderableInfos[currentRenderer].OH->setXForce(startForce.x);
	renderableInfos[currentRenderer].OH->setYForce(startForce.y);
	renderableInfos[currentRenderer].OH->setZForce(startForce.z);

	RenderableInfo* ret=&renderableInfos[currentRenderer++];

	return ret;
}

GeneralGlWindow::RenderableInfo* GeneralGlWindow::addLightSource(GeometryInfo* whatGeometry,const glm::mat4& whereMatrix,ShaderInfo* howShaders, TextureInfo* texture, MeshInfo* mesh)
{
	renderableInfos[currentRenderer].whatGeometryIndex=whatGeometry;
	renderableInfos[currentRenderer].where=whereMatrix;
	renderableInfos[currentRenderer].howShaderIndex=howShaders;

	if(texture!=NULL)
	{
		renderableInfos[currentRenderer].texture=texture;
	}
	if(mesh!=NULL)
	{
		renderableInfos[currentRenderer].collisionMesh=mesh;
	}

	RenderableInfo* ret=&renderableInfos[currentRenderer++];

	return ret;
}

GeneralGlWindow::MeshInfo* GeneralGlWindow::addMesh(GeometryInfo* shape, glm::vec3 position, float mass)
{
	btTriangleMesh* mTriMesh=new btTriangleMesh();
	for(int i=0; i<shape->numIndices; i+=3)
	{
		btVector3 v0(shape->vertices[shape->indices[i]].position.x, shape->vertices[shape->indices[i]].position.y, shape->vertices[shape->indices[i]].position.z);
		btVector3 v1(shape->vertices[shape->indices[i+1]].position.x, shape->vertices[shape->indices[i+1]].position.y, shape->vertices[shape->indices[i+1]].position.z);
		btVector3 v2(shape->vertices[shape->indices[i+2]].position.x, shape->vertices[shape->indices[i+2]].position.y, shape->vertices[shape->indices[i+2]].position.z);
	
		mTriMesh->addTriangle(v0,v1,v2);
	}

	btConvexShape* tmpConvexShape=new btConvexTriangleMeshShape(mTriMesh);
	
	btShapeHull* hull=new btShapeHull(tmpConvexShape);
	btScalar margin=tmpConvexShape->getMargin();
	hull->buildHull(margin);
	tmpConvexShape->setUserPointer(hull);

	btConvexHullShape* convexShape=new btConvexHullShape();
	bool updateLocalAabb=false;

	for(int i=0; i<hull->numVertices(); i++)
	{
		convexShape->addPoint(hull->getVertexPointer()[i],updateLocalAabb);
	}
	convexShape->recalcLocalAabb();
	convexShape->setMargin(0.01f);

	delete tmpConvexShape;
	delete hull;

	meshInfos[currentMesh].shape=convexShape;
	meshInfos[currentMesh].motionState=new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(position.x,position.y,position.z)));
	meshInfos[currentMesh].mass=mass;

	btVector3 inertia;
	meshInfos[currentMesh].shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, meshInfos[currentMesh].motionState, meshInfos[currentMesh].shape, inertia);
	meshInfos[currentMesh].rigidBody=new btRigidBody(rigidBodyCI);

	MeshInfo* ret=&meshInfos[currentMesh++];

	return ret;
}

GeneralGlWindow::TextureInfo* GeneralGlWindow::addTexture(const char* fileName)
{
	QImage imageTexture;

	if(!imageTexture.load(fileName))
	{
		std::cout << "Texture failed to load" << std::endl;
		return NULL;
	}
	else
	{
		imageTexture=QGLWidget::convertToGLFormat(imageTexture);
		glGenTextures(1, &textureInfos[currentTexture].textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureInfos[currentTexture].textureID);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,imageTexture.width(),imageTexture.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,(void*)imageTexture.bits());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		textureInfos[currentTexture].textureFileName=(char*)fileName;

		TextureInfo* ret=&textureInfos[currentTexture++];

		return ret;
	}
}

void GeneralGlWindow::addShaderStreamedParameter(GeometryInfo* gInfo, ShaderInfo* shaderID, uint layoutLocation, GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride, uint vertexStride)
{
	glLinkProgram(shaderID->programID);
	glUseProgram(shaderID->programID);
	glBindVertexArray(gInfo->vertexArrayID);
	glEnableVertexAttribArray(layoutLocation);
	glBindBuffer(GL_ARRAY_BUFFER, gInfo->bufferID);
	glVertexAttribPointer(layoutLocation, bufferStride, GL_FLOAT, GL_FALSE, vertexStride, (void*)(bufferOffset)); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gInfo->bufferID);
}
	
void GeneralGlWindow::addRenderableUniformParameter(ShaderInfo* shader,const char* name,GeneralGlWindow::ParameterType parameterType, const float* value)
{
	GLint uniformLocation = glGetUniformLocation(shader->programID, name);
	if(parameterType==PT_FLOAT)
	{
		glUniform1fv(uniformLocation, 1, value);
	}
	else if(parameterType==PT_VEC2)
	{
		glUniform2fv(uniformLocation, 1, value);
	}
	else if(parameterType==PT_VEC3)
	{
		glUniform3fv(uniformLocation, 1, value);
	}
	else if(parameterType==PT_VEC4)
	{
		glUniform4fv(uniformLocation, 1, value);
	}
	else if(parameterType==PT_MAT3)
	{
		glUniformMatrix3fv(uniformLocation, 1, false, value);
	}
	else if(parameterType==PT_MAT4)
	{
		glUniformMatrix4fv(uniformLocation, 1, false, value);
	}
}

void GeneralGlWindow::addTextureUniformparameter(ShaderInfo* shader, const char* name, GLuint ti)
{
	GLint uniformLocation=glGetUniformLocation(shader->programID, name);
	glUniform1i(uniformLocation, ti);
}

void GeneralGlWindow::setUpDeferred()
{
	glGenFramebuffers(1, &deferredID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, deferredID);
	glViewport(0,0,width(), height());

	glGenTextures(1, &deferredDepth);

	glGenTextures(1, &deferredDiffuse);
	glBindTexture(GL_TEXTURE_2D, deferredDiffuse);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, deferredDiffuse, 0);

	glGenTextures(1, &deferredPosition);
	glBindTexture(GL_TEXTURE_2D, deferredPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, deferredPosition, 0);

	glGenTextures(1, &deferredNormals);
	glBindTexture(GL_TEXTURE_2D, deferredNormals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, deferredNormals, 0);

	glBindTexture(GL_TEXTURE_2D, deferredDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, deferredDepth, 0);

	GLenum DrawBuffers[]={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

	glDrawBuffers(3, DrawBuffers);

	GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status!=GL_FRAMEBUFFER_COMPLETE)
		qDebug() << "Deferred Frame Buffer Did not Complete";

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}