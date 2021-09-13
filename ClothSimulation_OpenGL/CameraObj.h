#pragma once
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<iostream>
class CameraObj
{
public:
	// Camera Settings
	float CameraSetting_Yaw = -90.0f;
	float CameraSetting_Pitch = 0.0f;
	float CameraSetting_Speed = 150.0f;
	float CameraSetting_Sensitivity = 0.3f;
	float CameraSetting_Zoom = 45.0f;
	//Camera Members/Properties
	glm::vec3 Camera_Postion;
	glm::vec3 Camera_Front_Vector;
	glm::vec3 Camera_Up_Vector;
	glm::vec3 Camera_Right_Vector;
	glm::vec3 Camera_World_Up_Vector;

	enum MovementDirectionStates
	{
		Forward,Backward,Left,Right
	};
	CameraObj();
	CameraObj(float world_position_x, float world_position_y, float world_position_z, float vector_up_x, float vector_up_y, float vector_up_z);
	glm::mat4 GetEulerViewMatix();
	void ProcessKeyBoardInput(MovementDirectionStates state, float deltatime);
	void ProcessMouseInput(float offset_x,float offset_y);
	void ProcessMouseScrollInput(float offset_y);
	void UpdateCameraVector();
	~CameraObj();
};

