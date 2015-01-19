#pragma once

#include <GL\glew.h>
#include <glm.hpp>
#include <fstream>
#include <iostream>
#include "ObjectHandle.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision\Gimpact\btGImpactShape.h>
#include <BulletCollision\CollisionShapes\btShapeHull.h>
#include "Vertex.h"
#include "ExportHeader.h"

static int currentGeometry;
static int currentShader;
static int currentRenderer;
static int currentTexture;
static int currentMesh;

class ENGINE_SHARED GeneralGlWindow : public QGLWidget
{
public:
	static const int MY_NUM_BUFFERS=1000;
	static const int MY_NUM_GEOMETRIES=1000;
	static const int NUM_SHADER_INFO=100;
	static const int MY_NUM_RENDERABLES=10000;
	static const int MY_NUM_TEXTURES=1000;
	static const int MY_NUM_MESHES=10000;
	static const int MEGABYTE_SIZE=1004857699;
	bool change;
	GLuint programID;

	GLuint deferredID;
	//Deferred Textures
	GLuint deferredDiffuse;
	GLuint deferredPosition;
	GLuint deferredNormals;
	GLuint deferredDepth;


	enum ParameterType
	{
		// These values matter:
		PT_FLOAT = sizeof(float) * 1,
		PT_VEC2 = sizeof(float) * 2,
		PT_VEC3 = sizeof(float) * 3,
		PT_VEC4 = sizeof(float) * 4,
		PT_MAT3 = sizeof(float) * 9,
		PT_MAT4 = sizeof(float) *16,
		PT_TEXTURE=sizeof(float)*32
	};

	struct BufferInfo
	{
		GLuint glBufferID;
		GLuint remainingSize;

		BufferInfo()
		{
			remainingSize=MEGABYTE_SIZE;
		}
	} myBufferInfo[MY_NUM_BUFFERS];

	class GeometryInfo
	{
	public:
		GLuint vertexArrayID;
		GLuint bufferID;
		ushort* indices;
		DLTVertex* vertices;
		GLuint numVerts;
		GLuint numIndices;
		GLuint indexingMode;
		GLuint vertexOffset;
		GLuint indexOffset;
		friend class GeneralGlWindow;
	} geometryInfos[MY_NUM_GEOMETRIES];

	class ShaderInfo
	{
	public:
		GLuint programID;
		friend class GeneralGlWindow;
	} myShaderInfo[NUM_SHADER_INFO];

	struct TextureInfo
	{
	public:
		GLuint textureID;
		std::string textureFileName;
		friend class GeneralGlWindow;
	} textureInfos[MY_NUM_TEXTURES];

	struct MeshInfo
	{
		float mass;
		btCollisionShape* shape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidBody;
		friend class GeneralGlWindow;
	} meshInfos[MY_NUM_MESHES];

	struct RenderableInfo
	{
	public:
		GeometryInfo* whatGeometryIndex;
		ShaderInfo* howShaderIndex;
		glm::mat4 where;
		bool visible;
		TextureInfo* texture;
		ObjectHandle* OH;
		MeshInfo* collisionMesh;
		bool used;
		friend class GeneralGlWindow;
	} renderableInfos[MY_NUM_RENDERABLES];

	GeneralGlWindow();

	GeometryInfo* addGeometry(void* verts, GLuint numVertices, GLuint vertexDataSize, ushort* indices, uint numIndices,GLuint indexingMode);
	
	ShaderInfo* createShaderInfo(const char* vertexShaderFileName,const char* fragmentShaderFileName);
	
	RenderableInfo* addRenderable(GeometryInfo* whatGeometry,const glm::mat4& whereMatrix,ShaderInfo* howShaders, TextureInfo* texture, MeshInfo* mesh, std::string nameID, glm::vec3 position=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 sca=glm::vec3(1.0f,1.0f,1.0f), float rest=0.0f, float frict=0.0f, float m=0.0f, bool grav=false, bool worldInstance=false, bool aYaw=true, bool aPitch=true, bool aRoll=true, glm::vec3 startForce=glm::vec3(0.0f));

	RenderableInfo* instancedRenderables(GeometryInfo* whatGeometry, ShaderInfo* howShaders, TextureInfo* texture, MeshInfo* mesh, std::string nameID, glm::vec3 sca=glm::vec3(1.0f,1.0f,1.0f), float rest=0.0f, float frict=0.0f, float m=0.0f, bool grav=false, bool aYaw=true, bool aPitch=true, bool aRoll=true);

	RenderableInfo* addLightSource(GeometryInfo* whatGeometry,const glm::mat4& whereMatrix,ShaderInfo* howShaders, TextureInfo* texture, MeshInfo* mesh);

	MeshInfo* addMesh(GeometryInfo* shape, glm::vec3 position, float mass);

	std::string readShaderCode(const char* fileName);
	
	void addShaderStreamedParameter(GeometryInfo* gInfo,ShaderInfo* shaderID, uint layoutLocation, GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride, uint vertexStride);
	
	void addRenderableUniformParameter(ShaderInfo* shader,const char* name,GeneralGlWindow::ParameterType parameterType, const float* value);
	
	void addTextureUniformparameter(ShaderInfo* shader, const char* name, GLuint ti);

	void setUpDeferred();

	void setUpFinal();

	TextureInfo* addTexture(const char* fileName);
};