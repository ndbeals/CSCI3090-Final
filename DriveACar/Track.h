#pragma once
#include "Model.h"
class Track :
	public Model
{
public:


	Track();
	Track(GLuint);
	~Track();

	void LoadGeometry();
};

