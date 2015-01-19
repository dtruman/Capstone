#pragma once
#include <GeneralGlWindow.h>
#include "ScriptComponent.h"
#include "InputComponent.h"
#include "PlayerComponent.h"
#include "PathComponent.h"
#include "AIComponent.h"
#include <QtOpenGL\qglwidget>
#include <Qt\qtimer.h>
#include <fstream>
#include <Qt\qdebug.h>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#if EDITOR_ON
#include "DebugMenu.h"
#endif
#include <gtx\transform.hpp>
#include "LinearMath\btIDebugDraw.h"
#include <glm.hpp>
#include <iostream>
#include "Camera.h"
#include <Qt\qimage.h>
#include <ctime>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <QtGui\qfiledialog.h>
#include <fstream>
#include "Vertex.h"
class MyGLWindow : public QGLWidget
{
	Q_OBJECT

	QVBoxLayout* mainLayout;
	btDiscreteDynamicsWorld* dynamicsWorld;

	GeneralGlWindow::ShaderInfo* shaderCompileOne;
	GeneralGlWindow::ShaderInfo* shaderCamera;
	GeneralGlWindow::TextureInfo* defaultTexture;
	GeneralGlWindow::RenderableInfo* sphere;
	GeneralGlWindow::RenderableInfo* cameraRen;
	bool runWorld;

	glm::vec3 oldDirection;

	void sendDataToHardware();
	void setupVertexArrays();
	void intiailizeTextures();
	int nthOccurence(std::string str, std::string c, int n);
	void setUpSphere();
	void setUpCamera();
	float previousWidth;
	float previousHeight;
	std::string& replaceAll(std::string& context, std::string const& from, std::string const& to);
	GeneralGlWindow::GeometryInfo* inWindow();
	QTimer myTimer;
	clock_t current;
	clock_t last;
	clock_t t;
public:
	GameWindowManager gwm;
	void updateContacts();
	bool switchWorldON;
	int switchScene;
	Camera camera;
	GeneralGlWindow GGLWindow;
	void loadObj(std::string fileName);
	void loadInstant(std::string fileName);
	void addAmbientLight();
	void addDiffuseLight();
	void loadTexture(GeneralGlWindow::RenderableInfo*);
	void loadTex(GeneralGlWindow::RenderableInfo*, std::string fileName);
	void lightPass();
	void saveScene(std::string);
	void loadScene(std::string);
	void loadSceneFromProject(char* tempBuffer);
	GeneralGlWindow::RenderableInfo* window;
	GeneralGlWindow::ShaderInfo* deferredPass;
	void loadModel();
	void loadInstantModel();
	std::vector<GeneralGlWindow::RenderableInfo*> instanceShapes;
	std::vector<DiffuseLight*> diffuseLights;
	std::vector<AmbientLight*> ambientLights;
	bool switchCams;
	glm::vec3 oldCamPosition;
	glm::vec3 oldCamDirection;
	~MyGLWindow();
	float yRotation;
	float xRotation;
	glm::vec3 lookAt;
	glm::vec3 oldRotation;
	bool turnAround;

protected:
	void initializeGL();
	void paintGL();
	void keyPressEvent(QKeyEvent*);
	void mouseMoveEvent(QMouseEvent* e);
private slots:
	void myUpdate();
};

