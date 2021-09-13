#include "DataUnloader.h"



DataUnloader::DataUnloader()
{
}
//Simple Routine to wirte to the obj file
void DataUnloader::UnloadFileToObj(Cloth & customCloth)
{
	std::ofstream WriteToFile;
	WriteToFile.open("CustomCloth.obj");
	for (unsigned i = 0; i < customCloth.uniqueVertices.size(); i++)
	{
		WriteToFile << "v" <<" " << customCloth.uniqueVertices[i].position[0]<< " " << customCloth.uniqueVertices[i].position[1] << " " << customCloth.uniqueVertices[i].position[2] << "\n";
	}
	for (unsigned i = 0; i < customCloth.uniqueTextCoords.size(); i++)
	{
		WriteToFile << "vt" <<" " << customCloth.uniqueVertices[i].texCoord[0] << " " << customCloth.uniqueVertices[i].texCoord[1] << "\n";
	}
	for (unsigned i = 0; i < customCloth.uniqueNormals.size(); i++)
	{
		WriteToFile << "vn" << " " << customCloth.uniqueNormals[i].x << " " << customCloth.uniqueNormals[i].y << " " << customCloth.uniqueNormals[i].z<< "\n";
	}
	for (unsigned i = 0; i < customCloth.faceDataList.size(); i++)
	{
		WriteToFile << "f" << " " << (customCloth.faceDataList[i].vertexIndex[0] +1) <<"/"<< (customCloth.faceDataList[i].textureCoordIndex[0]+1) <<"/" << (customCloth.faceDataList[i].normalIndex[0]+1)<< " ";
		WriteToFile  << (customCloth.faceDataList[i].vertexIndex[1] + 1) <<"/"<< (customCloth.faceDataList[i].textureCoordIndex[1] + 1) <<"/" << (customCloth.faceDataList[i].normalIndex[1] + 1) << " ";
		WriteToFile  << (customCloth.faceDataList[i].vertexIndex[2] + 1) <<"/"<< (customCloth.faceDataList[i].textureCoordIndex[2] + 1) <<"/" << (customCloth.faceDataList[i].normalIndex[2] + 1) << " ";
		WriteToFile  << (customCloth.faceDataList[i].vertexIndex[3] + 1) <<"/"<< (customCloth.faceDataList[i].textureCoordIndex[3] + 1) <<"/" << (customCloth.faceDataList[i].normalIndex[3] + 1) << "\n";
	}

	WriteToFile.close();
}
DataUnloader::~DataUnloader()
{
}
