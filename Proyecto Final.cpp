/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
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
//para probar el importer
//#include<assimp/Importer.hpp>

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

//variables para animación
float movCoche;
float movOffset; //Variable complementaria 
float rotllanta;
float rotllantaOffset;
bool avanza;

//Variable animacion dadoTen
float movYD;
float movOffYD;

float rotYD;
float rotOffYD;

float rotXD;
float rotOffXD;

//Varobles animacion Jetski
int circuito;

float movJet;
float movOffJet;
float rotJet;
float rotOffJet;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

//Modelos Crouis
Model Puerta;

Model Trajinera;

Model Lirio;
Model Azucena;

Model Carreta;
Model Col;

Model casaAvatar;
Model Pulpo;

Model CuerpoJake;
Model BrazoDJake;
Model BrazoIJake;

Model FuerteArb;

Model casaTroq1;
Model casaTroq2;
Model casaTroq3;
Model casaTroq4;
Model casaTroq5;
Model casaTroq6;
Model casaTroq7;
Model casaTroq8;
Model casaTroq9;
Model casaTroq10;

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
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

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
	

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso2.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();


	//Modelos
	Puerta = Model();
	Puerta.LoadModel("Models/Modelo Puerta/Estructura Puerta.obj");

	Trajinera = Model();
	Trajinera.LoadModel("Models/trajinera-OBJ/Trajinera.obj");

	Lirio = Model();
	Lirio.LoadModel("Models/Lirio de fuego/Lirio de Fuego.obj");
	Azucena = Model();
	Azucena.LoadModel("Models/Azucena Panda/Azucena Panda.obj");

	Carreta = Model();
	Carreta.LoadModel("Models/Carreta/Carreta.obj");
	Col = Model();
	Col.LoadModel("Models/Carreta/Col.obj");

	casaAvatar = Model();
	casaAvatar.LoadModel("Models/Casa avatar/Modelo Casa.obj");

	Pulpo = Model();
	Pulpo.LoadModel("Models/Purple Pentapus/Purple pentapus.obj");

	CuerpoJake = Model();
	CuerpoJake.LoadModel("Models/Modelo_jake_el_perro_brazos_separados/Modelo_jake_el_perro.obj");
	BrazoDJake = Model();
	BrazoDJake.LoadModel("Models/Modelo_jake_el_perro_brazos_separados/Modelo_jake_el_perro_brazo_Derecho.obj");
	BrazoIJake = Model();
	BrazoIJake.LoadModel("Models/Modelo_jake_el_perro_brazos_separados/Modelo_jake_el_perro_brazo_Izquierdo.obj");

	FuerteArb = Model();
	FuerteArb.LoadModel("Models/Modelo_02_Fuerte_del_arbol/Fachada_Fuerte_del_arbol.obj");

	
	casaTroq1 = Model();
	casaTroq1.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_marco_ventana_pared01.obj");

	casaTroq2 = Model();
	casaTroq2.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_pared.obj");

	casaTroq3 = Model();
	casaTroq3.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_pared02.obj");

	casaTroq4 = Model();
	casaTroq4.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_pared03.obj");

	casaTroq5 = Model();
	casaTroq5.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_pared04.obj");

	casaTroq6 = Model();
	casaTroq6.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_piso.obj");

	casaTroq7 = Model();
	casaTroq7.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_puerta.obj");

	casaTroq8 = Model();
	casaTroq8.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_Techo.obj");

	casaTroq9 = Model();
	casaTroq9.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_ventana_pared01.obj");

	casaTroq10 = Model();
	casaTroq10.LoadModel("Models/Modelo_01_Casa_tronquitos/Fachada_ventana_puerta.obj");




	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/negx_dia.jpg");
	skyboxFaces.push_back("Textures/Skybox/posx_dia.jpg");
	skyboxFaces.push_back("Textures/Skybox/negy_dia.jpg");
	skyboxFaces.push_back("Textures/Skybox/posy_dia.jpg");
	skyboxFaces.push_back("Textures/Skybox/negz_dia.jpg");
	skyboxFaces.push_back("Textures/Skybox/posz_dia.jpg");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movCoche = 0.0f;
	movOffset = 0.5f;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;

	//Asignacion variables Ejercicio Practica 9
	//Ejercicio 1
	movYD = 0.0f;
	movOffYD = 0.1f;

	rotYD = 0.0f;
	rotOffYD = 1.0;

	rotXD = 0.0f;
	rotOffXD = 0.8f;

	//Ejercicio 2
	//animacion;
	circuito = 1;

	movJet = 0.0f;
	movOffJet = 0.5f;
	rotJet = 0.0f;
	rotOffJet = 0.5f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Animacion Ejercicio 9.1
		if (mainWindow.getReset() == true)
		{
			movYD = 0.0f;
			rotYD = 0.0f;
			mainWindow.getAnimD() == false;
		}
		else
		{
			if (mainWindow.getAnimD() == true)
			{
				if (movYD >= -8.5f)
				{
					movYD -= movOffYD * deltaTime;
					rotYD += rotOffYD * deltaTime;
					rotXD -= rotOffXD * deltaTime;
				}
			}
		}

		//Animacion Ejercicio 9.2
		if (circuito == 1)
		{
			if (movJet > -100.0f)
			{
				movJet -= movOffJet * deltaTime;

			}
			else
			{
				circuito = 2;

			}
		}
		else if (circuito == 2)
		{
			if (rotJet < 180.0f)
			{
				rotJet += rotOffJet * deltaTime;

			}
			else
			{
				circuito = 3;
			}
		}
		
		else if (circuito == 3)
		{
			if (circuito == 3 && movJet < 100)
			{
				movJet += movOffJet * deltaTime;
			}
			else
			{
				circuito = 4;

			}
		}

		else if (circuito == 4)
		{
			if (rotJet > -180)
			{
				rotJet -= rotOffJet * deltaTime;
			}
			else
			{
				circuito = 1;
			}
		}
		
		
		if (avanza)
		{
			if (movCoche > -300)
			{
				movCoche -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
				rotllanta += rotllantaOffset * deltaTime;
			}
			//rotllanta += rotllantaOffset * deltaTime;
			else
			{
				avanza = false;
			}
		}
		else
		{
			if (movCoche < 300.0f)
			{
				movCoche += movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
				rotllanta -= rotllantaOffset * deltaTime;
			}
			//rotllanta += rotllantaOffset * deltaTime;
			else
			{
				avanza = true;
			}
		}
		/**
		if (movCoche < 30.0f && movCoche > -300)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}
		//rotllanta += rotllantaOffset * deltaTime;
		*/

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

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

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
		
	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.RenderModel();

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, -2.0f, -30.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carreta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoJake.RenderModel();

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDJake.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIJake.RenderModel();
		
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(-80.0f, -10.0f, -30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Azucena.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(-20.0f, -10.0f, -30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lirio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(-100.0f, -10.0f, -45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Azucena.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(75.0f, -10.0f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lirio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(50.0f, -10.0f, -70.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Azucena.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(-15.0f, -10.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lirio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaAvatar.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -60.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trajinera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -1.0f, -60.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trajinera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.0f, -60.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trajinera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -1.0f, -55.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pulpo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, -1.0f, -5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq1.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq2.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq3.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq4.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq5.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq6.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq8.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq9.RenderModel();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq10.RenderModel();

		modelaux = model;

		model = glm::translate(model, glm::vec3(-10.1f, 4.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTroq7.RenderModel();
		model = modelaux;

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
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

