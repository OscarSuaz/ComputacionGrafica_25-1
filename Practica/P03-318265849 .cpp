//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.37f, 0.0f,	//2
		0.0f,0.0f,0.84f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

void CrearPiramideTriangularInvertida() {
	{
		unsigned int indices_piramide_triangular[] = {
				0,1,2,
				1,3,2,
				3,0,2,
				1,0,3

		};
		GLfloat vertices_piramide_triangular[] = {
			-0.5f, -0.5f,0.0f,	//0
			0.5f,-0.5f,0.0f,	//1
			0.0f,0.37f, 0.0f,	//2
			0.0f,0.0f,-0.84f,	//3

		};
		Mesh* obj1 = new Mesh();
		obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
		meshList.push_back(obj1);

	}
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(1200, 1200);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	
	CrearPiramideTriangular();//índice 0 en MeshList
	CrearPiramideTriangularInvertida();//indice 1 
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.5f);

	

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		//Pramide negro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -5.5f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//color rojo
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		{
			//pico
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 1.25f, -5.15f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.6f, -5.63f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_rigt
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.4f, 0.67f, -5.58f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.4f, 0.67f, -5.58f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_rigt_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.4f, 0.02f, -6.05f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_left_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.4f, 0.02f, -6.05f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.1f, -6.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.8f, 0.1f, -6.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.8f, 0.1f, -6.0f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
		}

		//verde
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		{
			//pico
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.02f, 1.25f, -5.1f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.05f, 0.62f, -4.75f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.42f, 0.67f, -5.5f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.44f, 0.05f, -5.13f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.8f, 0.1f, -5.85f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.05f, 0.0f, -4.42f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
		}

		//amarillo
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		{
			//pico
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.02f, 1.25f, -5.1f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.05f, 0.63f, -4.75f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.42f, 0.67f, -5.5f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.44f, 0.05f, -5.13f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.8f, 0.1f, -5.85f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.05f, 0.0f, -4.42f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
		}

		//azul
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		{
			//pico
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -0.125f, -4.42f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.36f, -0.07f, -5.13f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//2_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.36f, -0.07f, -5.13f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_center
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -0.05f, -5.85f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_left
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.75f, -0.025f, -5.85f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			//3_right
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.75f, -0.025f, -5.85f));
			model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
			model = glm::rotate(model, 35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		