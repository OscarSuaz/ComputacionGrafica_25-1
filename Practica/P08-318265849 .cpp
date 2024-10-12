#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>


#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;


Model Kitt_M;
Model Cofre_M;
Model Llanta_M;
Model Blackhawk_M;
Model Lampara_M;
Model Camara_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight

PointLight pointLights_L[MAX_POINT_LIGHTS_L];
PointLight pointLights_P[MAX_POINT_LIGHTS_P];
PointLight pointLights[MAX_POINT_LIGHTS];

SpotLight spotLights_F[MAX_SPOT_LIGHTS];
SpotLight spotLights_B[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,

		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,

	};

	GLfloat cubo_vertices[] = {

		// DOWN
		//x		y		z		S		T			NX		NY		NZ
		0.f, 0.0f,  0.0f,	    0.0f,   0.0f,		0.0f,	1.0f,	-1.0f,	//0
		1.f,  0.0f,  0.0f,		0.2f,	0.0f,		0.0f,	1.0f,	-1.0f,	//1
		0.5f,  0.64f,  1.0f,	0.1f,	0.2f,		0.0f,	1.0f,	-1.0f,  //2

		1.0f,  0.0f,  0.0f,		0.2f,	0.0f,		-1.0f,	1.0f,	-1.0f,	//3
		1.31f, 0.95f, 0.0f,	    0.4f,	0.0f,		-1.0f,	1.0f,	-1.0f,	//4
		0.5f,  0.64f,  1.0f,	0.3f,	0.2f,		-1.0f,	1.0f,	-1.0f,  //5

		1.31f, 0.95f, 0.0f,	    0.4f,	0.0f,		-1.0f,	-1.0f,	1.0f,	//6
		0.5f,  1.54f, 0.0f,	    0.6f,	0.0f,		-1.0f,	-1.0f,	1.0f,	//7
		0.5f,  0.64f,  1.0f,	0.5f,	0.2f,		-1.0f,	-1.0f,	1.0f,  //8

		0.5f,  1.54f, 0.0f,	    0.6f,	0.0f,		1.0f,	-1.0f,	1.0f,	//9
		-0.31f,0.95f, 0.0f,	    0.8f,	0.0f,		1.0f,	-1.0f,	1.0f,  //10
		0.5f,  0.64f,  1.0f,	0.7f,	0.2f,		1.0f,	-1.0f,	1.0f,  //11

		-0.31f,0.95f, 0.0f,	    0.8f,	0.0f,		1.0f,	1.0f,	-1.0f,  //12
		0.0f,  0.0f,  0.0f,	    1.0f,	0.0f,		1.0f,	1.0f,	-1.0f,  //13
		0.5f,  0.64f,  1.0f,	0.9f,	0.2f,		1.0f,	1.0f,	-1.0f,  //14


		//UP
		0.f, 0.0f,  0.0f,	    0.0f,   1.0f,		0.0f,	1.0f,	1.0f,	//15
		1.f,  0.0f,  0.0f,		0.2f,	1.0f,		0.0f,	1.0f,	1.0f,	//16
		0.5f,  0.64f,  -1.0f,	0.1f,	0.8f,		0.0f,	1.0f,	1.0f,  //17
	
		1.0f,  0.0f,  0.0f,		0.2f,	1.0f,		-1.0f,	1.0f,	1.0f,	//18
		1.31f, 0.95f, 0.0f,	    0.4f,	1.0f,		-1.0f,	1.0f,	1.0f,	//19
		0.5f,  0.64f,  -1.0f,	0.3f,	0.8f,		-1.0f,	1.0f,	1.0f,  //20

		1.31f, 0.95f, 0.0f,	    0.4f,	1.0f,		-1.0f,	-1.0f,	1.0f,	//21
		0.5f,  1.54f, 0.0f,	    0.6f,	1.0f,		-1.0f,	-1.0f,	1.0f,	//22
		0.5f,  0.64f,  -1.0f,	0.5f,	0.8f,		-1.0f,	-1.0f,	1.0f,  //23

		0.5f,  1.54f, 0.0f,	    0.6f,	1.0f,		1.0f,	-1.0f,	-1.0f,	//24
		-0.31f,0.95f, 0.0f,	    0.8f,	1.0f,		1.0f,	-1.0f,	-1.0f,  //25
		0.5f,  0.64f,  -1.0f,	0.7f,	0.8f,		1.0f,	-1.0f,	-1.0f,  //26

		-0.31f,0.95f, 0.0f,	    0.8f,	1.0f,		1.0f,	1.0f,	1.0f,  //27
		0.0f,  0.0f,  0.0f,	    1.0f,	1.0f,		1.0f,	1.0f,	1.0f,  //28
		0.5f,  0.64f,  -1.0f,	0.9f,	0.8f,		1.0f,	1.0f,	1.0f,  //29		
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 270, 30);
	meshList.push_back(dado);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/Dado_textura.png");
	dadoTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/Coche_completo_texturixado.obj");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre_texturizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_texturzada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/Lampara_texturizada.obj");
	Camara_M = Model();
	Camara_M.LoadModel("Models/phone.obj");

	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.2f, 0.2f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount_L = 0;
	unsigned int pointLightCount_P = 0;
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights_L[0] = PointLight(1.0f, 1.0f, 0.0f,
		0.5f, 0.9f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.5f, 0.1f);
	pointLightCount_L++;

	pointLights_P[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.1f);
	pointLightCount_P++;

	pointLights[0] = pointLights_L[0];
	pointLightCount++;
	pointLights[1] = pointLights_P[0];
	pointLightCount++;

	unsigned int spotLightCount = 0;
	unsigned int spotLightCount_B = 0;
	unsigned int spotLightCount_F = 0;
	//cofre
	spotLights_F[0] = SpotLight(0.0f, 1.0f, 0.0f,
		0.1f, 0.2f,
		20.0f, 10.0f, 0.0f,
		-5.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.3f,
		5.0f);
	spotLightCount_F++;
	//luz frontal
	spotLights_F[1] = SpotLight(1.0f, 0.0f, 0.0f,
		5.0f, 0.2f,
		0.0f, 0.0f, 0.0f,
		-5.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.1f,
		5.0f);
	spotLightCount_F++;

	spotLights_B[0] = spotLights_F[0];
	spotLightCount_B++;

	spotLights_B[1] = SpotLight(0.0f, 0.0f, 1.0f,
		5.0f,0.2f,
		0.0f,0.0f,0.0f,
		5.0f,-1.0f,0.0f,
		1.0f,0.0f,0.1f,
		5.0f);
	spotLightCount_B++;
	spotLightCount = spotLightCount_B + spotLightCount_F;

	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		
		if (mainWindow.getluz() && mainWindow.getphone()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount-2);
			if (mainWindow.getluz()) {
				shaderList[0].SetPointLights(pointLights_L, pointLightCount_L);
			}
			if (mainWindow.getphone()) {
				shaderList[0].SetPointLights(pointLights_P, pointLightCount_P);
			}
		}
		
		


		if (mainWindow.getsentido() > 0) {
			shaderList[0].SetSpotLights(spotLights_F, spotLightCount_F);
		}
		else {
			if (mainWindow.getsentido() != 0) {
				shaderList[0].SetSpotLights(spotLights_B, spotLightCount_B);
				
			}
			else {
				shaderList[0].SetSpotLights(spotLights_F, spotLightCount_F - 1);
			}
		}




		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.5f + mainWindow.getmuevex(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		spotLights_F[1].SetFlash(glm::vec3(model[3].x - (3.0f), model[3].y, model[3].z - (1.0f)), glm::vec3(model[1][0]- 1.0f,model[1][1]- 4.0f,model[1][2]));
		spotLights_B[1].SetFlash(glm::vec3(model[3].x + (4.0f), model[3].y, model[3].z - (1.0f)), glm::vec3(model[1][0]+1.0f, model[1][1]-4.0f, model[1][2]));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//cofre
		model = glm::translate(model, glm::vec3(-0.7f,0.35f,0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getRotaCofre()), glm::vec3(0.0f,0.0f,1.0f));
		
		//Luz de cofre
		spotLights_F[0].SetFlash(glm::vec3(model[3].x - (2.0f), model[3].y, model[3].z - (1.0f)), glm::vec3((model[1].x + 1.0f) * (-1.0), model[1].y * (-1.0), model[1].z));
		spotLights_B[0].SetFlash(glm::vec3(model[3].x - (2.0f),model[3].y,model[3].z - (1.0f)), glm::vec3((model[1].x+1.0f) * (-1.0), model[1].y * (-1.0), model[1].z));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//Altura y rotación correcta de las llantas
		model = modelaux;
		model = glm::rotate(model, toRadians * 90, glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
		modelaux = model;

		//Llanta delantera izquierda
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, -1.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 1.35f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, 0.0f, -1.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, 0.0f, 1.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.0f, -10.0f));
		pointLights_L[0].SetPos(glm::vec3(model[3][0], model[3][1] + 5.0f, model[3][2]));
		pointLights[0].SetPos(glm::vec3(model[3][0], model[3][1] + 5.0f, model[3][2]));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		//FLash telefono
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 4.0f, -10.0f));
		pointLights_P[0].SetPos(glm::vec3(model[3][0], model[3][1] + 5.0f, model[3][2]));
		pointLights[1].SetPos(glm::vec3(model[3][0], model[3][1] + 5.0f, model[3][2]));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camara_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
