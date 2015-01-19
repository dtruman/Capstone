#pragma once
#include "ObjectHandle.h"
#include "AmbientLight.h"
#include "DiffuseLight.h"
#include "ExportHeader.h"

class ENGINE_SHARED Component
{
	ObjectHandle* parent;
public:
	virtual void Initialize(ObjectHandle*, int)=0;
	virtual void InitializeLight(AmbientLight*, int)=0;
	virtual void InitializeDLight(DiffuseLight*, int)=0;
	virtual void update(float dt)=0;
	std::string type;
};