#include "DataLoader.h"

DataLoader::DataLoader()
{
}


DataLoader::~DataLoader()
{
}
bool DataLoader::ReadObjFileTriFace(std::vector<VertexData> &uniqueVertices, std::vector<glm::vec3>&uniqueNormals, std::vector<glm::vec2>&uniquetextCoords, std::vector<unsigned int>& indices, std::string path) {
	std::ifstream inFile;
	//Get the file
	std::size_t found = path.find(".obj");

	//Check if file was found
	if (!(found != std::string::npos))
	{
		std::cout << "Please provide a file with .obj Extension" << std::endl;
		return false;
	}
	//Check if path was empty
	if (path.empty())
	{
		std::cout << "Please provide a file with .obj Extension this path is empty" << std::endl;
		return false;
	}
	else
	{
		//Open the file
		inFile.open(path);
		if (!inFile)
		{
			std::cout << "Unable to the file" << std::endl;
			return false;
		}
		else
		{
			std::string v1;
			while (inFile >> v1)
			{
				//Get the line which starts with v
				if (v1 == "v")
				{
					//Get the vertices values and store them in list
					float x, y, z;
					inFile >> x >> y >> z;
					VertexData data;
					data.position[0] = x ;
					data.position[1] = y ;
					data.position[2] = z ;
					uniqueVertices.push_back(data);
				}
				//Get the line which starts with vn
				else if (v1 == "vn")
				{
					//Get the normal values and store them in list
					float x, y, z;
					inFile >> x >> y >> z;
					glm::vec3 n(x, y, z);
					uniqueNormals.push_back(n);
				}
				//Get the line which starts with vt
				else if (v1 == "vt")
				{
					//Get the textureCoordinates values and store them in list
					float x, y;
					inFile >> x >> y;
					glm::vec2 t(x, y);
					uniquetextCoords.push_back(t);

				}
				//Get the line which starts with f
				else if (v1 == "f")
				{
					uint32_t vp1, vt1, vn1, vp2, vt2, vn2, vp3, vt3, vn3;
					char slash;
					//Fetch the four bundles of indices for current face (v1/vt1/vn1)
					inFile >> vp1 >> slash >> vt1 >> slash >> vn1;
					inFile >> vp2 >> slash >> vt2 >> slash >> vn2;
					inFile >> vp3 >> slash >> vt3 >> slash >> vn3;
					//Arrange the Vertex Index and store it 
					indices.push_back(vp1 - 1);
					indices.push_back(vp2 - 1);
					indices.push_back(vp3 - 1);
					uniqueVertices[vp1 - 1].normal[0] = uniqueNormals[vn1 - 1].x;
					uniqueVertices[vp1 - 1].normal[1] = uniqueNormals[vn1 - 1].y;
					uniqueVertices[vp1 - 1].normal[2] = uniqueNormals[vn1 - 1].z;
					
					uniqueVertices[vp2 - 1].normal[0] = uniqueNormals[vn2 - 1].x;
					uniqueVertices[vp2 - 1].normal[1] = uniqueNormals[vn2 - 1].y;
					uniqueVertices[vp2 - 1].normal[2] = uniqueNormals[vn2 - 1].z;
					
					uniqueVertices[vp3 - 1].normal[0] = uniqueNormals[vn3 - 1].x;
					uniqueVertices[vp3 - 1].normal[1] = uniqueNormals[vn3 - 1].y;
					uniqueVertices[vp3 - 1].normal[2] = uniqueNormals[vn3 - 1].z;
				}

			}
			for (int i = 0; i < uniqueVertices.size(); i++)
			{
				uniqueVertices[i].texCoord[0] = uniquetextCoords[i].x;
                uniqueVertices[i].texCoord[1] = uniquetextCoords[i].y;

			}
			return true;
		}
	}
}

bool DataLoader::ReadObjFileQuadFace(std::vector<VertexData> &uniqueVertices,std::vector<glm::vec3>&uniqueNormals, std::vector<glm::vec2>&uniquetextCoords,std::vector<FaceData>& faceDataList, std::string path)
{
	std::ifstream inFile;
	//Get the file
	std::size_t found = path.find(".obj");
	//Check if file was found
	if (!(found != std::string::npos))
	{
		std::cout << "Please provide a file with .obj Extension" << std::endl;
		return false;
	}
	//Check if path was empty
	if (path.empty())
	{
		std::cout << "Please provide a file with .obj Extension this path is empty" << std::endl;
		return false;
	}
	else
	{
		//Open the file
		inFile.open(path);
		if (!inFile)
		{
			std::cout << "Unable to the file" << std::endl;
			return false;
		}
		else
		{
			std::string v1;
			while (inFile >> v1)
			{
				//Get the line which starts with v
				if (v1 == "v")
				{
					//Get the vertices values and store them in list
					float x, y, z;
					inFile >> x >> y >> z;
					VertexData data;
					data.position[0] = x;
					data.position[1] = y;
					data.position[2] = z;
					uniqueVertices.push_back(data);
				}
				//Get the line which starts with vn
				else if (v1 == "vn")
				{
					//Get the normal values and store them in list
					float x, y, z;
					inFile >> x >> y >> z;
					glm::vec3 n(x, y, z);
					uniqueNormals.push_back(n);
				}
				//Get the line which starts with vt
				else if (v1 == "vt")
				{
					//Get the textureCoordinates values and store them in list
					float x, y;
					inFile >> x >> y;
					glm::vec2 t(x, y);
					uniquetextCoords.push_back(t);

				}
				//Get the line which starts with f
				else if (v1 == "f")
				{
					uint32_t vp1, vt1, vn1, vp2, vt2, vn2, vp3, vt3, vn3,vp4,vt4,vn4;
					char slash;
					inFile >> vp1 >> slash >> vt1 >> slash >> vn1;
					inFile >> vp2 >> slash >> vt2 >> slash >> vn2;
					inFile >> vp3 >> slash >> vt3 >> slash >> vn3;
					inFile >> vp4 >> slash >> vt4 >> slash >> vn4;

					FaceData face;
					face.vertexIndex[0] = vp1-1;
					face.vertexIndex[1] = vp2-1;
					face.vertexIndex[2] = vp3-1;
					face.vertexIndex[3] = vp4-1;

					face.textureCoordIndex[0] = vt1-1;
					face.textureCoordIndex[1] = vt2-1;
					face.textureCoordIndex[2] = vt3-1;
					face.textureCoordIndex[3] = vt4-1;

					face.normalIndex[0] = vn1-1;
					face.normalIndex[1] = vn2-1;
					face.normalIndex[2] = vn3-1;
					face.normalIndex[3] = vn4-1;

					faceDataList.push_back(face);
				}

			}
	
			return true;
		}
	}
}


bool DataLoader::LoadTexture(std::vector<unsigned char>& values, int & width, int & height, int & max_col_val, std::string path)
{
	std::ifstream inFile;
	//open file 
	inFile.open(path);
	// check if opened
	if (!inFile)
	{
		std::cout << "Unable to the file" << std::endl;
		return false;
	}
	else
	{
		int triple = 0;
		// simple string to read the lines
		std::string str;

		int total = 0;

		while (std::getline(inFile, str)) {
			// total is 2 we are at the line that has the rows and columns values
			if (total == 1)
			{
				// find the position of empty 
				int pos = str.find(' ');
				//get the left side sub string
				std::string sub_1 = str.substr(0, pos);
				//get the right side sub string
				std::string sub_2 = str.substr(pos + 1);
				// cast back to int and save in rows
				width = std::stoi(sub_1);
				// cast back to int and save in columns
				height = std::stoi(sub_2);
			}
			else if (total > 2) // if the total is more than 3 then we are reading the rgb values
			{
				values.push_back((unsigned char)std::stoi(str));
			}
			//increment the total
			total++;
		}
	}
	return true;
}