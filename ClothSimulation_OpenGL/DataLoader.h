#pragma once
#ifndef _DATALOADER_
#define _DATALOADER_
#include<iostream>
#include <string>
#include<vector>
#include<fstream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include"Cloth.h"
#include "VertexData.h"
#include"FaceData.h"
class DataLoader
{
public:
	DataLoader();
	~DataLoader();
	//Read tri face obj
	bool ReadObjFileTriFace(std::vector<VertexData> &uniqueVertices, std::vector<glm::vec3>&uniqueNormals, std::vector<glm::vec2>&uniquetextCoords, std::vector<unsigned int>& indices, std::string path);
	//Read quad faced obj
	bool ReadObjFileQuadFace(std::vector<VertexData> &uniqueVertices, std::vector<glm::vec3>&uniqueNormals, std::vector<glm::vec2>&uniquetextCoords, std::vector<FaceData>& faceDataList, std::string path);
	//Load ppm texture
	bool LoadTexture(std::vector<unsigned char>& values, int & width, int & height, int & max_col_val, std::string path);
};

#endif