#pragma once

#include <QtOpenGL\qglwidget>
#include <QtGui\qmainwindow.h>
#include <QtGui\qpushbutton.h>
#include "ExportHeader.h"
#include <glm.hpp>
#include <iostream>
#include <vector>

class Component;

class ENGINE_SHARED DiffuseLight : public QWidget
{
	Q_OBJECT

public:
	DiffuseLight(std::string ID, glm::vec3 pos=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 col=glm::vec3(1.0f,1.0f,1.0f), float size=0.0f);
	~DiffuseLight();
private:
	std::vector<Component*> components;
	QPushButton* lightDelete;
	QPushButton* lightClone;
public:
	std::string nameID;
	int menuNum;
	glm::vec3 color;
	glm::vec3 position;
	bool deleteLight;
	bool cloneLight;
	float size;
	void toggleMenu();
	void LightUpdate();
	void addComponent(Component* comp);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setXColor(float x);
	void setYColor(float y);
	void setZColor(float z);
	bool menuOn;
private slots:
	void ligtDelete();
	void ligtClone();
};