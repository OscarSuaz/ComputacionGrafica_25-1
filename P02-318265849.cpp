//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"

const float toRadians = 3.14159265f/180.0;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

static const char* vShader_blue = "shaders/shader_blue.vert";
static const char* vShader_cafe = "shaders/shader_cafe.vert";
static const char* vShader_green = "shaders/shader_green.vert";
static const char* vShader_red = "shaders/shader_red.vert";
static const char* vShader_verde = "shaders/shader_verde.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

float angulo = 0.0f;

void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}


void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			//Letra O (24 vertices)
			-0.95f,0.9f,0.0f,		1.0f,0.0f,0.0f,
			-0.65f,0.9f, 0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.8f,0.0f,		1.0f,0.0f,0.0f,
			-0.65F,0.8F,0.0F,		1.0f,0.0f,0.0f,
			-0.65f,0.9f, 0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.8f,0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.3f,0.0f,		1.0f,0.0f,0.0f,
			-0.65f,0.3f, 0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.2f,0.0f,		1.0f,0.0f,0.0f,
			-0.65F,0.2F,0.0F,		1.0f,0.0f,0.0f,
			-0.65f,0.3f, 0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.2f,0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.8f,0.0f,		1.0f,0.0f,0.0f,
			-0.95f,0.3f,0.0f,		1.0f,0.0f,0.0f,
			-0.90F,0.8F,0.0F,		1.0f,0.0f,0.0f,
			-0.90F,0.8F,0.0F,		1.0f,0.0f,0.0f,
			-0.95f,0.3f,0.0f,		1.0f,0.0f,0.0f,
			-0.90F,0.3F,0.0F,		1.0f,0.0f,0.0f,
			-0.70f,0.8f,0.0f,		1.0f,0.0f,0.0f,
			-0.70f,0.3f,0.0f,		1.0f,0.0f,0.0f,
			-0.65F,0.8F,0.0F,		1.0f,0.0f,0.0f,
			-0.65F,0.8F,0.0F,		1.0f,0.0f,0.0f,
			-0.70f,0.3f,0.0f,		1.0f,0.0f,0.0f,
			-0.65F,0.3F,0.0F,		1.0f,0.0f,0.0f,

			//Letra M (18 vertices)
			-0.6f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.55f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.6f,0.2f,0.0f,		0.0f,1.0f,0.0f,
			-0.55f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.6f,0.2f,0.0f,		0.0f,1.0f,0.0f,
			-0.55f,0.2f,0.0f,		0.0f,1.0f,0.0f,
			-0.3f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.35f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.3f,0.2f,0.0f,		0.0f,1.0f,0.0f,
			-0.35f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.3f,0.2f,0.0f,		0.0f,1.0f,0.0f,
			-0.35f,0.2f,0.0f,		0.0f,1.0f,0.0f,
			-0.45,0.5f,0.0f,		0.0f,1.0f,0.0f,
			-0.6f,0.85f,0.0f,		0.0f,1.0f,0.0f,
			-0.55f,0.9f,0.0f,		0.0f,1.0f,0.0f,
			-0.45f,0.5f,0.0f,		0.0f,1.0f,0.0f,
			-0.3f,0.85f,0.0f,		0.0f,1.0f,0.0f,
			-0.35f,0.9f,0.0f,		0.0f,1.0f,0.0f,

			//Letra S (30 vertices)
			-0.2f, 0.9f,0.0f,		0.0f,0.0f,1.0f,
			0.1f,0.9f, 0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.8f,0.0f,		0.0f,0.0f,1.0f,
			0.1F,0.8F,0.0F,			0.0f,0.0f,1.0f,
			0.1f,0.9f, 0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.8f,0.0f,		0.0f,0.0f,1.0f,
			-0.2f, 0.3f,0.0f,		0.0f,0.0f,1.0f,
			0.1f,0.3f, 0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.2f,0.0f,		0.0f,0.0f,1.0f,
			0.1,0.2F,0.0F,			0.0f,0.0f,1.0f,
			0.1f,0.3f, 0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.2f,0.0f,		0.0f,0.0f,1.0f,
			-0.2f, 0.6f,0.0f,		0.0f,0.0f,1.0f,
			0.1f,0.6f, 0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.5f,0.0f,		0.0f,0.0f,1.0f,
			0.1F,0.5F,0.0F,			0.0f,0.0f,1.0f,
			0.1f,0.6f, 0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.5f,0.0f,		0.0f,0.0f,1.0f,
			-0.2f,0.9f,0.0f,		0.0f,0.0f,1.0f,
			-0.2f, 0.5f,0.0f,		0.0f,0.0f,1.0f,
			-0.15F,0.9F,0.0F,		0.0f,0.0f,1.0f,
			-0.15F,0.9F,0.0F,		0.0f,0.0f,1.0f,
			-0.2f, 0.5f,0.0f,		0.0f,0.0f,1.0f,
			-0.15F,0.5F,0.0F,		0.0f,0.0f,1.0f,
			0.05f,0.5f,0.0f,		0.0f,0.0f,1.0f,
			0.05f, 0.2f,0.0f,		0.0f,0.0f,1.0f,
			0.1F,0.5F,0.0F,			0.0f,0.0f,1.0f,
			0.1F,0.5F,0.0F,			0.0f,0.0f,1.0f,
			0.05f,0.2f,0.0f,		0.0f,0.0f,1.0f,
			0.1F,0.2F,0.0F,			0.0f,0.0f,1.0f,
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,432);
	meshColorList.push_back(letras);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader rojo  0
	shader1->CreateFromFiles(vShader_red, fShader);
	shaderList.push_back(*shader1);

	Shader* shader3 = new Shader();//shader azul  1
	shader3->CreateFromFiles(vShader_blue, fShader);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader();//shader verde claro  2
	shader3->CreateFromFiles(vShader_green, fShader);
	shaderList.push_back(*shader3);

	Shader* shader5 = new Shader();//shader café  3
	shader3->CreateFromFiles(vShader_cafe, fShader);
	shaderList.push_back(*shader3);

	Shader* shader6 = new Shader();//shader verde oscuro  4
	shader3->CreateFromFiles(vShader_verde, fShader);
	shaderList.push_back(*shader3);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras   5
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); 
	CrearCubo();
	CrearLetrasyFiguras(); 
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	

	glm::mat4 model(1.0); 
	

	while (!mainWindow.getShouldClose())
	{

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Para las letras hay que usar el shader 5
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.3f, -7.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		// Define color a utilizar Rojo
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		// Casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.35f, -6.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Define color a utilizar verde claro
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		
		//Ventana1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, -0.05f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Ventana 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.3f, -0.05f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Define color azul
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//techo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.1f, -6.0f));
		model = glm::scale(model, glm::vec3(1.9f, 1.4f, 1.4f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Define color café
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		//Tronco Izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, -0.85f, -6.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Tronco Derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.0f, -0.85f, -6.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Define color verde oscuro
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		//Arbol izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, -0.28f, -6.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Arbol derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.0f, -0.28f, -6.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}