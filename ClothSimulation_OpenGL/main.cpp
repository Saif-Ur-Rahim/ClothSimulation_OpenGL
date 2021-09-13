#include<GL/glew.h>
#include<GL/GLU.h>
#include<GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_opengl3.h"
#include<iostream>
#include<string>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<unordered_map>
#include"CameraObj.h"
#include "GlobalVariables.h"
#include <stdio.h>
#include<map>
#include <windows.h>
#include <shobjidl.h> 
#include <fstream>
#include "Cloth.h"
#include"SkyBox.h"
#include "VertexData.h"
#include "ObjSpring.h"
#include "Sphere.h"
#include"DataUnloader.h"
#include<string>
#include<memory.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>
#include<time.h>


//Overriding the imGUI list box to support vector input for the UI
namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}
}


// GLFW CallBacks 
void InitializeWindowSystem();
void Resize_Callback(GLFWwindow* window, int width, int height);
void Mouse_Movement_CallBack(GLFWwindow* window, double xpos, double ypos);
void Mouse_Scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset);
void Mouse_Click_CallBack(GLFWwindow* window, int button, int action, int mods);
void oneTimeKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void MainUIRender();
void InitailizeCloth(std::string clothPath);
void InitailizeScene();
void flipImage(int width, int height, char *data);
int saveImage(const char *filename);
const char* generateImageName();
int captureScreenshot();
void GenerateVideo();
// Mouse and Screen 
GLFWwindow* window;
int ScreenWidth = 1280;
int ScreenHeight = 720;
float lastX = ScreenWidth / 2.0f;
float lastY = ScreenHeight / 2.0f;
bool firstMouse = true;
bool moveMouse = false;

//Constants
char KS_VALUE[100] = {};
char KD_VALUE[100] = {};
char filePath[500] = { 'c','l','o','t','h','3','.','o','b','j' };

//Process Input
void processInput(GLFWwindow *window);

//Main Camera 
CameraObj MainCamera(0.0f,-6.0f,20.0f, 0.0f, 1.0f, 0.0f);

float FPS = 60.0f;

//Editor States 
enum EditorStates
{
	Play_State,
	Pause_State,
	Restart_State,
	Stop_State
};
//Current Editor State
EditorStates CurrentEditorState;
//Timer Status
bool TimerBool =false;
//Current Frame 
float currentFrame = 0.0f;
//Wind Force
glm::vec3 windForce;
//Constants
float ks = 100.0f;
float kd =  0.2f;
//Main Cloth
std::shared_ptr<Cloth> customcloth;
//Main Sphere
Sphere sphere;
//Data Writer
DataUnloader dataUnloader;
//Current selected particle
int selectedItem =0;
//UI List
std::vector<std::string>listBoxLabels;
//Physics Engine flags
bool collisionStatic = false;
bool enableCollision = false;
bool rotateSphere = false;
//Video Recorder Properties
bool recordVideo = false;
int snapCount = 0;
int imageCount = 0;

int main(void)
{
    //Initialize the Main scene
	windForce = glm::vec3(0.0f, 0.0f, 0.0f);
	InitializeWindowSystem();
	CurrentEditorState = EditorStates::Pause_State;
	glm::mat4 projection(1.0f);
	glm::mat4 view(1.0f);
	SkyBox sky;
	InitailizeScene();

	//Main glfw Loop
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
	
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Calculation of delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//Point size increase for debuging
		glPointSize(10);
		//Process use Inputs
		processInput(window);
		//Get the view from the camera
		view = MainCamera.GetEulerViewMatix();
		//Set projection matrix
		projection = glm::perspective(glm::radians(MainCamera.CameraSetting_Zoom), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100000000000000000.0f);
		//Run the engine
		if (CurrentEditorState == EditorStates::Play_State)
		{
			if (customcloth != nullptr)
			{
				if (customcloth->loaded)
				{
					//Apply Accumulated Forces
					for (int i = 0; i < customcloth->ObjParticles.size(); i++)
					{
						customcloth->ObjParticles[i].em_removeForceSum();
						customcloth->ObjParticles[i].em_applyForce(glm::vec3(0.0f, -10.0f, 0.0f));
						customcloth->ObjParticles[i].em_applyForce(windForce);
					}
					//Run the springs
					for (int i = 0; i < customcloth->ObjSprings.size(); i++)
					{
						customcloth->ObjSprings[i].em_runSpringForceSystem(ks, kd, true);
					}
					//Run the physics loop and update velocity and positions
					for (int i = 0; i < customcloth->ObjParticles.size(); i++)
					{
						customcloth->ObjParticles[i].em_runPhysicsLoop(0.1f);

					}
					//Check Collisions
					if(enableCollision)
					for (int i = 0; i < customcloth->ObjParticles.size(); i++)
					{
						customcloth->ObjParticles[i].CheckCollision(sphere.worldPosition, 1.0f, collisionStatic,rotateSphere);
					}

				}
			}

		}
		//Check if cloth is loaded and render it
		if(customcloth !=nullptr)
		customcloth->Render(view, projection, MainCamera.Camera_Postion);
		//Check if collision is enabled
		if (enableCollision)
		sphere.Render(view, projection, MainCamera.Camera_Postion, rotateSphere);
	    //Render the sky box
		sky.Render(view, projection);
		//Setup the view port
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
	   
		//Render UI
		MainUIRender();
		//Maintain FPS
		while (glfwGetTime() < lastFrame + 1.0 / FPS) {
		 // sleep
		}
		lastFrame += 1.0 / FPS;

		currentFrame++;
		
	   glfwSwapBuffers(window);
	   glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void oneTimeKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

//Process input and move camera
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Forward, deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Backward, deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Left, deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		MainCamera.ProcessKeyBoardInput(CameraObj::Right, deltaTime);
		
	}

}

//Initailize the main glfw window and opengl 
void InitializeWindowSystem()
{
	//Initialize GLFW and OPENGL Pointers
	if (!glfwInit())
		std::cout << "failed to load up glfw" << std::endl;
	window = glfwCreateWindow(ScreenWidth, ScreenHeight, "AnimationSystem", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "failed to load up glfw" << std::endl;
		std::cout << "terminating" << std::endl;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Resize_Callback);
	glfwSetCursorPosCallback(window, Mouse_Movement_CallBack);
	glfwSetMouseButtonCallback(window, Mouse_Click_CallBack);
	glfwSetScrollCallback(window, Mouse_Scroll_CallBack);
	glfwSetKeyCallback(window, oneTimeKeyCallBack);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//Setup GLEW 
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "error occured in glew" << std::endl;
	}
	//Setup ImGUI
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}
//Main UI Render Function
void MainUIRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Main GUI");
	//File loading UI Elments
	ImGui::InputText("File", filePath, sizeof(filePath));
	if (ImGui::Button("Load File"))
	{
		CurrentEditorState = EditorStates::Pause_State;
		int size = sizeof(filePath) / sizeof(char);
		std::string path = "";
		for (int i = 0; i < size; i++) {
			path = path + filePath[i];
		}
		InitailizeCloth(path);
	}
	//FPS Elements
	ImGui::Text("Current FPS: %f",(float)FPS);
	ImGui::Text("Current Frame: %f",(float)currentFrame);
	ImGui::Text("FPS Limiter");
	ImGui::SliderFloat("FPS", &FPS, 1.0f, 120.0f);
	//Physics elments
	ImGui::Checkbox("Static Collision", &collisionStatic);
	ImGui::Checkbox("Enable Collision", &enableCollision);
	ImGui::Checkbox("Rotate", &rotateSphere);
	ImGui::PushItemWidth(150);
	ImGui::Text("KS = %f", (float)ks);
	ImGui::SameLine();
	if (ImGui::Button("+ 1.0f"))
	{
		ks++;
	}
	ImGui::SameLine();

	if (ImGui::Button("- 1.0f"))
	{
		ks--;
	}
	ImGui::Text("KD = %f", (float)kd);
	ImGui::SameLine();
	if (ImGui::Button("+ 0.1f"))
	{
		kd+=0.1f;
	}
	ImGui::SameLine();
	if (ImGui::Button("- 0.1f"))
	{
		kd -= 0.1f;
	}

	ImGui::Text("WindX = %f", (float)windForce.x);
	ImGui::SameLine();
	if (ImGui::Button("+ X"))
	{
		windForce.x += 1.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("- X"))
	{
		windForce.x -= 1.0f;
	}
	ImGui::Text("WindY = %f", (float)windForce.y);
	ImGui::SameLine();
	if (ImGui::Button("+ Y"))
	{
		windForce.y += 1.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("- Y"))
	{
		windForce.y -= 1.0f;
	}
	ImGui::Text("WindZ = %f", (float)windForce.z);
	ImGui::SameLine();
	if (ImGui::Button("+ Z"))
	{
		windForce.z += 1.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("- Z"))
	{
		windForce.z -= 1.0f;
	}
	ImGui::PushItemWidth(350);
	//Editor State Elements
	if (ImGui::Button("Play"))
	{
		CurrentEditorState = EditorStates::Play_State;
	}	
	if (ImGui::Button("Pause"))
	{
		CurrentEditorState = EditorStates::Pause_State;
	}

	//Cloth Properties Elements
	if (customcloth != nullptr)
	{
		if (customcloth->loaded)
		{
			ImGui::ListBox("Particles", &selectedItem, listBoxLabels);
			ImGui::Text("Selected ID : %f", (float)selectedItem);
			if (customcloth->ObjParticles[selectedItem].em_Static)
				ImGui::Text("Static Status : true");
			else
				ImGui::Text("Static Status : false");
			if (ImGui::Button("Make Static"))
			{
				customcloth->ObjParticles[selectedItem].em_Static = !customcloth->ObjParticles[selectedItem].em_Static;
			}

			if (ImGui::Button("Write Frame To File"))
			{
				dataUnloader.UnloadFileToObj(*customcloth);
			}
		}
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

//Initialize the cloth and delete it if it was previously loaded up so we can easily swap files
void InitailizeCloth(std::string clothPath)
{
	if (customcloth != nullptr)
	{
		customcloth->loaded = false;
		customcloth = nullptr;
	}
	
	listBoxLabels.clear();
	selectedItem = 0;
	customcloth.reset(new Cloth());
	//Create the cloth
	customcloth->CreateObject(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, GL_TRIANGLES, clothPath, "sphere.ppm");
	//Create Particles and springs
	for (int i = 0; i < customcloth->ObjParticles.size(); i++)
	{
		std::string text;
		text = "  ID:" + std::to_string(customcloth->ObjParticles[i].Id);
		text += "  N:" + std::to_string(customcloth->ObjParticles[i].connections.size());
		std::vector<int>::iterator it1 = std::find(customcloth->EdgePointsIds.begin(), customcloth->EdgePointsIds.end(), customcloth->ObjParticles[i].Id);
		if (it1 != customcloth->EdgePointsIds.end())
		{
			text += " Corner";
		}
		listBoxLabels.push_back(text);
	}
	
}
//Initialize scene objects
void InitailizeScene()
{
	sphere.CreateObject(glm::vec3(0.0f, -6.0f, 0.0f), 1.0f, GL_TRIANGLES, "sphere.obj", "shell.ppm");
}
//GLFW Resize
void Resize_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//GLFW Mouse Click Callback
void Mouse_Click_CallBack(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		
		moveMouse = true;
		std::cout << moveMouse << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

		moveMouse = false;
		std::cout << moveMouse << std::endl;
	}

}
//GLFW Mouse movement
void Mouse_Movement_CallBack(GLFWwindow* window, double xpos, double ypos)
{
	
	if (moveMouse)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; 

		lastX = xpos;
		lastY = ypos;
		//std::cout << xoffset << "    " << yoffset << std::endl;
		
		if (xoffset != 0)
		{
			if (xoffset < 0)
			{
				xoffset = xoffset / xoffset;
				xoffset = (-1)* xoffset;
			}
			else
			{
				xoffset = xoffset / xoffset;
			}
		}
		if (yoffset != 0)
		{
			if (yoffset < 0)
			{
				yoffset = yoffset / yoffset;
				yoffset = (-1)* yoffset;
			}
			else
			{
				yoffset = yoffset / yoffset;
			}
		}

		MainCamera.ProcessMouseInput(xoffset, yoffset);
	}
}
//GLFW Mouse scroll
void Mouse_Scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	//MainCamera.ProcessMouseScrollInput(yoffset);
}

