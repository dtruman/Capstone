#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <direct.h>
#include <shlobj.h>
#include <QtGui\Qwidget.h>
#include <QtGui\Qslider.h>
#include <QtGui\Qpushbutton.h>
#include <QtGui\Qcheckbox.h>
#include <QtGui\qinputdialog.h>
#include "MyGLWindow.h"
#include <QtGui\QHboxLayout>
#include <QtGui\QVboxLayout>
#include <QtGui\qmenubar.h>
#include <QtGui\qlabel.h>
#include "DebugSlider.h"

class MeWidg : public QWidget
{
	Q_OBJECT

public:
	QTimer myTimer;
	QMenuBar* mb;
	QMenu* projectMenu;
	bool testToggle;
	float testRow;
	bool noToggle;
	int sceneNum;

	char *tempBuffer[20];
	int tempNumBytes[20];

	MyGLWindow *gameGLWindow;
	MeWidg();
	void sceneClicked(int num);
	void updateScene(std::string sceneName);
	int currentScene;
	void loadGameProject(std::string projName);
	bool firstRun;

private slots:
	void myUpdate();
	void loadModel();
	void loadALight();
	void loadDLight();
	void loadInstancedModel();
	void saveScene();
	void exportGame();
	void loadScene();
	void addScene();
	void selectScene();
	void saveProject();
	void loadProject();
};