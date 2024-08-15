#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <Windows.h>//Importe para utilizar Sleep()
#include <cstdlib> //Importe para utilizar rand() y srand()
#include <ctime>//Importe para utilizar time()


//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;
float red = 0.0f, green = 0.0f, blue = 0.0f;
int choice = 0;

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);	 			\n\
}";



void CrearTriangulo()
{
	GLfloat vertices[] = {
		//Letra O (24 vertices)
		-0.95f, 0.9f,0.0f,
		-0.65f,0.9f, 0.0f,
		-0.95f,0.8f,0.0f,
		-0.65F,0.8F,0.0F,
		-0.65f,0.9f, 0.0f,
		-0.95f,0.8f,0.0f,
		-0.95f, 0.3f,0.0f,
		-0.65f,0.3f, 0.0f,
		-0.95f,0.2f,0.0f,
		-0.65F,0.2F,0.0F,
		-0.65f,0.3f, 0.0f,
		-0.95f,0.2f,0.0f,
		-0.95f,0.8f,0.0f,
		-0.95f, 0.3f,0.0f,
		-0.90F,0.8F,0.0F,
		-0.90F,0.8F,0.0F,
		-0.95f, 0.3f,0.0f,
		-0.90F,0.3F,0.0F,
		-0.70f,0.8f,0.0f,
		-0.70f, 0.3f,0.0f,
		-0.65F,0.8F,0.0F,
		-0.65F,0.8F,0.0F,
		-0.70f, 0.3f,0.0f,
		-0.65F,0.3F,0.0F,

		//Letra M (18 vertices)
		-0.6f,0.9f,0.0f,
		-0.55f,0.9f,0.0f,
		-0.6f,0.2f,0.0f,
		-0.55f,0.9f,0.0f,
		-0.6f,0.2f,0.0f,
		-0.55f,0.2f,0.0f,
		-0.3f,0.9f,0.0f,
		-0.35f,0.9f,0.0f,
		-0.3f,0.2f,0.0f,
		-0.35f,0.9f,0.0f,
		-0.3f,0.2f,0.0f,
		-0.35f,0.2f,0.0f,
		-0.45,0.5f,0.0f,
		-0.6f,0.85f,0.0f,
		-0.55f,0.9f,0.0f,
		-0.45f,0.5f,0.0f,
		-0.3f,0.85f,0.0f,
		-0.35f,0.9f,0.0f,
		
		//Letra S (30 vertices)
		-0.2f, 0.9f,0.0f,
		0.1f,0.9f, 0.0f,
		-0.2f,0.8f,0.0f,
		0.1F,0.8F,0.0F,
		0.1f,0.9f, 0.0f,
		-0.2f,0.8f,0.0f,
		-0.2f, 0.3f,0.0f,
		0.1f,0.3f, 0.0f,
		-0.2f,0.2f,0.0f,
		0.1,0.2F,0.0F,
		0.1f,0.3f, 0.0f,
		-0.2f,0.2f,0.0f,
		-0.2f, 0.6f,0.0f,
		0.1f,0.6f, 0.0f,
		-0.2f,0.5f,0.0f,
		0.1F,0.5F,0.0F,
		0.1f,0.6f, 0.0f,
		-0.2f,0.5f,0.0f,
		-0.2f,0.9f,0.0f,
		-0.2f, 0.5f,0.0f,
		-0.15F,0.9F,0.0F,
		-0.15F,0.9F,0.0F,
		-0.2f, 0.5f,0.0f,
		-0.15F,0.5F,0.0F,
		0.05f,0.5f,0.0f,
		0.05f, 0.2f,0.0f,
		0.1F,0.5F,0.0F,
		0.1F,0.5F,0.0F,
		0.05f,0.2f,0.0f,
		0.1F,0.2F,0.0F,

		// Letra A (24 Vertices)
		0.15f, 0.9f,0.0f,
		0.45f,0.9f, 0.0f,
		0.15f,0.8f,0.0f,
		0.45F,0.8F,0.0F,
		0.45f,0.9f, 0.0f,
		0.15f,0.8f,0.0f,
		0.15f, 0.6f,0.0f,
		0.45f,0.6f, 0.0f,
		0.15f,0.5f,0.0f,
		0.45F,0.5F,0.0F,
		0.45f,0.6f, 0.0f,
		0.15f,0.5f,0.0f,
		0.15f,0.8f,0.0f,
		0.15f, 0.2f,0.0f,
		0.20F,0.8F,0.0F,
		0.20F,0.8F,0.0F,
		0.15f, 0.2f,0.0f,
		0.20F,0.2F,0.0F,
		0.40f,0.8f,0.0f,
		0.40f, 0.2f,0.0f,
		0.45F,0.8F,0.0F,
		0.45F,0.8F,0.0F,
		0.40f, 0.2f,0.0f,
		0.45F,0.2F,0.0F,

	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CambiarColor() { //Función para el cambio aleatorio de color de fondio de pantalla
	choice = (rand() % 3)+1;//Genera número aleatorio entre 1 y 3
	printf("%i",choice);//Muestra en terminal numero generado
	
	//Selector que cambia el color de la ventana en base al número aleatorio
	switch (choice)
	{
	case 1://Azul
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
	break;
	case 2://Verde
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	break;
	case 3://Rojo
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	break;
	default://error
		printf("Algo salió mal");
		break;
	}
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}


	glViewport(0, 0, BufferWidth, BufferHeight);


	CrearTriangulo();
	CompileShaders();

	//Creación de semilla de numeros aleatorios utilizando la fecha actual del equipo
	srand((unsigned)time(NULL));


	while (!glfwWindowShouldClose(mainWindow))
	{
	
		glfwPollEvents();

		//Llamada a función de color aleatorio
		CambiarColor();
		//utilización de variables RGB
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);//72 para letras "OMS", 96 para letras "OMSA"
		glBindVertexArray(0);

		glfwSwapBuffers(mainWindow);
		Sleep(2000);//Espera un lapso de 2 segundos antes de volver a ejecutar el ciclo

	}


	return 0;
}