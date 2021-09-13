#pragma once
#ifndef _VERTEXDATA_
#define _VERTEXDATA_
class VertexData
{
	//Simple class that has out vertex data
public:
	float position[3];
	float texCoord[2];
	float color[3];
	float normal[3];
	VertexData();
	~VertexData();
};
#endif

