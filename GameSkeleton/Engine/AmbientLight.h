#pragma once

#include <QtOpenGL\qglwidget>
#include <QtGui\qmainwindow.h>
#include <QtGui\qpushbutton.h>
#include "ExportHeader.h"
#include <glm.hpp>
#include <iostream>
#include <vector>

class Component;

class ENGINE_SHARED AmbientLight : public QWidget
{
	Q_OBJECT

public:
	AmbientLight(std::string ID, glm::vec3 col=glm::vec3(1.0f,1.0f,1.0f), float inten=1);
	~AmbientLight();
private:
	std::vector<Component*> components;
	QPushButton* lightDelete;
	QPushButton* lightClone;
public:
	std::string nameID;
	int menuNum;
	bool deleteLight;
	bool cloneLight;
	glm::vec3 color;
	float intensity;
	void toggleMenu();
	void LightUpdate();
	void addComponent(Component* comp);
	void setXColor(float x);
	void setYColor(float y);
	void setZColor(float z);
	bool menuOn;
private slots:
	void ligDelete();
	void ligClone();
};