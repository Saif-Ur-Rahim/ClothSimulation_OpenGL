#pragma once
class FaceData
{
	//Simple class that show the base face data 
public:
	int vertexIndex[4];
	int textureCoordIndex[4];
	int normalIndex[4];

	FaceData();
	~FaceData();
};

