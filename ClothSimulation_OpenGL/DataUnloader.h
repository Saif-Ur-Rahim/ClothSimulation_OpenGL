#pragma once
#ifndef _DATAUNLOADER_
#define _DATAUNLOADER_
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "Cloth.h"
#include "FaceData.h"
class DataUnloader
{
public:
	DataUnloader();
	//Function write the obj
	void UnloadFileToObj(Cloth & customCloth);
	~DataUnloader();
};
#endif
