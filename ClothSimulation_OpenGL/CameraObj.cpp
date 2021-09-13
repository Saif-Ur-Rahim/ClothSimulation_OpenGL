#include "CameraObj.h"

CameraObj::CameraObj(float world_position_x, float world_position_y, float world_position_z, float vector_up_x, float vector_up_y, float vector_up_z)
{

	Camera_Front_Vector = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera_Postion = glm::vec3(world_position_x, world_position_y, world_position_z);
	Camera_World_Up_Vector = glm::vec3(vector_up_x, vector_up_y, vector_up_z);
		
	UpdateCameraVector();

}

glm::mat4 CameraObj::GetEulerViewMatix()
{
	return glm::lookAt(Camera_Postion,Camera_Postion + Camera_Front_Vector, Camera_Up_Vector);
}

void CameraObj::ProcessKeyBoardInput(MovementDirectionStates state, float deltatime)
{
	//std::cout << deltatime << std::endl;
	float Camera_Velocity = CameraSetting_Speed * deltatime;
	if (state == Forward)
		Camera_Postion += Camera_Front_Vector * Camera_Velocity;
	if (state == Backward)
		Camera_Postion -= Camera_Front_Vector * Camera_Velocity;
	if (state == Left)
		Camera_Postion -= Camera_Right_Vector * Camera_Velocity;
	if (state == Right)
		Camera_Postion += Camera_Right_Vector * Camera_Velocity;

}

void CameraObj::ProcessMouseInput(float offset_x, float offset_y)
{
	offset_x = offset_x * CameraSetting_Sensitivity;
	offset_y = offset_y * CameraSetting_Sensitivity;
	CameraSetting_Yaw = CameraSetting_Yaw + offset_x;
	CameraSetting_Pitch = CameraSetting_Pitch + offset_y;

	if (CameraSetting_Pitch > 89.0f)
		CameraSetting_Pitch = 89.0f;
	if (CameraSetting_Pitch < -89.0f)
		CameraSetting_Pitch = -89.0f;
	UpdateCameraVector();
}

void CameraObj::ProcessMouseScrollInput(float offset_y)
{
	if (CameraSetting_Zoom >= 1.0f && CameraSetting_Zoom <= 45.0f)
	{
		CameraSetting_Zoom -= offset_y;
	}
	if (CameraSetting_Zoom <= 1.0f)
	{
		CameraSetting_Zoom = 1.0f;
	}
	if (CameraSetting_Zoom >= 45.0f)
	{
		CameraSetting_Zoom = 45.0f;
	}
}

void CameraObj::UpdateCameraVector()
{
     glm::vec3 Temp_Front;
	 Temp_Front.x = cos(glm::radians(CameraSetting_Yaw)) * cos(glm::radians(CameraSetting_Pitch));
	 Temp_Front.y = sin(glm::radians(CameraSetting_Pitch));
	 Temp_Front.z = sin(glm::radians(CameraSetting_Yaw)) * cos(glm::radians(CameraSetting_Pitch));
	 Camera_Front_Vector = glm::normalize(Temp_Front);
	 Camera_Right_Vector = glm::normalize(glm::cross(Camera_Front_Vector,Camera_World_Up_Vector));
	 Camera_Up_Vector = glm::normalize(glm::cross(Camera_Right_Vector, Camera_Front_Vector));
}

CameraObj::CameraObj()
{
}


CameraObj::~CameraObj()
{
}
