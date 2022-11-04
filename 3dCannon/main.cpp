
/* HEADER-----
Assignment 3 CSCE 515

Name: Roberto Salazar ULID c00416436

date: 10/09/2022

\


*/
#include <stack>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <glm\gtx\quaternion.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Vase.h"
#include "Arrow.h"
#include "Barrel.h"
#include "Trunnion.h"
#include "Cheek.h"
#include "Hub.h"
#include "Stock.h"
#include "Rim.h"
#include "Spoke.h"
#include "Screw.h"
#include "Utils.h"
#include "Axle.h"
using namespace std;
int prevMouseX = 0, prevMouseY = 0;
bool mouseHeld = false;
float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 4
float AngleIncrement = 1.0f;
float cameraX, cameraY, cameraZ;
float torLocX, torLocY, torLocZ;
GLuint renderingProgram;
GLuint vboBarrel[numVBOs];
GLuint vboHub[numVBOs];
GLuint vboSpoke[numVBOs];
GLuint vboScrew[numVBOs];
GLuint vboRim[numVBOs];
GLuint vboTrunnion[numVBOs];
GLuint vboCheek[numVBOs];
GLuint vboStock[numVBOs];
GLuint vboAxle[numVBOs];
GLuint vboArrow[numVBOs];
GLuint vao[numVAOs];
GLuint brickTexture;
GLuint arrowTexturez, arrowTexturex, arrowTexturey;
GLuint metalTexture;
GLuint woodTexture, woodNailTexture;
GLuint innerArrowTexturez, innerArrowTexturex, innerArrowTexturey;
glm::quat yRot, xRot, zRot,wheelRot;
glm::vec4 V;
float rotAmt = 0.0f;
bool inicia = true;
glm::mat4  elevation, azimuth,scale;
// variable allocation for display
GLuint mvLoc, projLoc, mLoc;
GLuint mvALoc, projALoc, mALoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, M;
glm::mat4  MA, MA1, MA2, screwTx;
glm::mat4 temp;
float Ax, Ay, Az,scaleV,wR,trunnionR,screwR;
stack <glm::mat4> mvStack;

Arrow myArrow(10, 1, 30, 30, 10, 1, 160, 1);
Barrel myBarrel(10, 1, 30, 30, 10, 1, 160, 1);
Cheek myCheek(10, 1, 30, 30, 10, 1, 160, 1);
Stock myStock(10, 1, 30, 30, 10, 1, 160, 1);
Trunnion myTrunnion(10, 1, 30, 30, 10, 1, 160, 1);
Screw myScrew(10, 1, 30, 30, 10, 1, 160, 1);
Hub myHub(10, 1, 30, 30, 10, 1, 160, 1);
Rim myRim(1.5,0.2,48);
Spoke mySpoke(10, 1, 30, 30, 10, 1, 160, 1);
Axle myAxle(10, 1, 30, 30, 10, 1, 160, 1);







glm::mat4 buildTranslate(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
	return r;
}

glm::mat4 rotateX(float rad) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, cos(rad), sin(rad), 0,
		0, -sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateY(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), 0, -sin(rad), 0,
		0, 1, 0, 0,
		sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateZ(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), sin(rad), 0, 0,
		-sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 buildScala(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
	return r;
}
//MOUSE hover
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x_pos = 0.0;
	double y_pos = 0.0;
	glfwGetCursorPos(window, &x_pos, &y_pos);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseHeld = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseHeld = false;
		azimuth = elevation = buildTranslate(0, 0, 0);
	}
}
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos) {


	if (mouseHeld)
	{
		(xpos - prevMouseX == 0 ? (azimuth =  rotateY(toRadians(0))) : (xpos - prevMouseX > 0 ? (azimuth = rotateY(toRadians(-5))) : (azimuth = rotateY(toRadians(5)))));
		(ypos - prevMouseY == 0 ? (elevation =  rotateX(toRadians(0))) : (ypos - prevMouseY > 0 ? (elevation =  rotateX(toRadians(-10))) : (elevation = rotateX(toRadians(10)))));
		
		
	}
	prevMouseX = xpos;
	prevMouseY = ypos;
}
void setupVertices(void) {
	std::vector<int> ind;
	std::vector<glm::vec3> vert ;
	std::vector<glm::vec2> tex ;
	std::vector<glm::vec3> norm ;

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	//Barrel SetUP
	ind = myBarrel.getIndices();
	vert = myBarrel.getVertices();
	tex = myBarrel.getTexCoords();
	norm = myBarrel.getNormals();

	for (int i = 0; i < myBarrel.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboBarrel);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboBarrel[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////
	

		//Axle 
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();

	ind = myAxle.getIndices();
	vert = myAxle.getVertices();
	tex = myAxle.getTexCoords();
	norm = myAxle.getNormals();

	for (int i = 0; i < myAxle.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboAxle);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboAxle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Hub SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myHub.getIndices();
	vert = myHub.getVertices();
	tex = myHub.getTexCoords();
	norm = myHub.getNormals();

	for (int i = 0; i < myHub.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboHub);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHub[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Rim SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myRim.getIndices();
	vert = myRim.getVertices();
	tex = myRim.getTexCoords();
	norm = myRim.getNormals();

	for (int i = 0; i < myRim.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboRim);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRim[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Spoke SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = mySpoke.getIndices();
	vert = mySpoke.getVertices();
	tex = mySpoke.getTexCoords();
	norm = mySpoke.getNormals();

	for (int i = 0; i < mySpoke.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboSpoke);

	glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSpoke[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Cheek SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myCheek.getIndices();
	vert = myCheek.getVertices();
	tex = myCheek.getTexCoords();
	norm = myCheek.getNormals();

	for (int i = 0; i < myCheek.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboCheek);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCheek[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Stock SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myStock.getIndices();
	vert = myStock.getVertices();
	tex = myStock.getTexCoords();
	norm = myStock.getNormals();

	for (int i = 0; i < myStock.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboStock);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboStock[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Screw SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myScrew.getIndices();
	vert = myScrew.getVertices();
	tex = myScrew.getTexCoords();
	norm = myScrew.getNormals();

	for (int i = 0; i < myScrew.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboScrew);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboScrew[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Barrel SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myArrow.getIndices();
	vert = myArrow.getVertices();
	tex = myArrow.getTexCoords();
	norm = myArrow.getNormals();

	for (int i = 0; i < myArrow.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboArrow);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Trunnion SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myTrunnion.getIndices();
	vert = myTrunnion.getVertices();
	tex = myTrunnion.getTexCoords();
	norm = myTrunnion.getNormals();

	for (int i = 0; i < myTrunnion.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboTrunnion);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTrunnion[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////








}

void init(GLFWwindow* window) {

	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 12.0f;
	torLocX = 0.0f; torLocY = 0.0f; torLocZ = -0.5f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	//pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);
	vMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	M = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	elevation=azimuth=MA = MA1 = MA2 = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	yRot = xRot = zRot=wheelRot = glm::quat(0, 0, 0, 0);
/*	MA = buildScala(0.5, 0.2, 0.2)*MA;
	MA1 = MA1 * rotateZ(toRadians(90));
	MA1 = buildScala(0.2, 0.5, 0.2)*MA1;
	MA2 = MA2 * rotateY(toRadians(-90));
	MA2 = buildScala(0.2, 0.2, 0.5)*MA2;*/
	Ax = Ay = Az = wR = trunnionR= screwR = 0;
	screwTx = buildTranslate(0, 0, 0);
	V = glm::vec4(0,0,0,0);
	scaleV = 1.0;
	scale = buildScala(scaleV,scaleV,scaleV);
	setupVertices();
	brickTexture = Utils::loadTexture("brick1.jpg");
	arrowTexturey = Utils::loadTexture("brick2.jpg");
	arrowTexturez = Utils::loadTexture("brick3.jpg");
	arrowTexturex = Utils::loadTexture("brick4.jpg");
	metalTexture = Utils::loadTexture("metal.jpg");
	woodTexture = Utils::loadTexture("wood.jpg"); 
	woodNailTexture = Utils::loadTexture("woodNail.jpg");
	innerArrowTexturex = Utils::loadTexture("innerArrow.jpg");
	innerArrowTexturey = Utils::loadTexture("innerArrow2.jpg");
	innerArrowTexturez = Utils::loadTexture("innerArrow3.jpg");
	glEnable(GL_PRIMITIVE_RESTART);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

}
void display(GLFWwindow* window, double currentTime, float xTr, float yTr, float zTr, glm::mat4 &temp) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	//MV
	vMat *= glm::inverse(elevation) *glm::inverse(azimuth)*glm::inverse(scale);
	mvStack.push(vMat);

	
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// Arrows
	//X
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0,0,0);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, arrowTexturex);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Y
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, arrowTexturey);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Z
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, arrowTexturez);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();


	//base
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	temp= mvStack.top();
	temp += ((glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot))*V);
	mvStack.top() = temp;
	mvStack.push(mvStack.top());
	mvStack.top() *= ((glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot))*glm::toMat4(zRot));
	mvStack.push(mvStack.top());
	mvStack.top() *= (glm::toMat4(yRot)*(glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot)));
	mvStack.push(mvStack.top());

	// Arrows BASE
	//X

	mvStack.top() *= buildScala(0.6, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, innerArrowTexturex);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();


	//Y
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(0.6, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, innerArrowTexturey);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Z
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(0.6, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, innerArrowTexturez);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//CArriage rify
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(-45));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-0.5,0,-0.5);
	mvStack.push(mvStack.top());
	//Axle
	//mvStack.top() *= buildScala(1.5, 1.5, 1.5);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboAxle[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myAxle.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	//Hub rigth
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(1, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateX(toRadians(wR));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboHub[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHub[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myHub.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//RimRight
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0.4, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotateX(wR);
	//mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboRim[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRim[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myRim.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	//RimSpokesRight
	//1
	for (int i = 0; i < 4; i++)
	{
		mvStack.push(mvStack.top());
		mvStack.top() *= rotateZ(toRadians((i * 360) / 4));
		mvStack.push(mvStack.top());
		mvStack.top() *= buildTranslate(0.4, 0, 0);
		mvStack.push(mvStack.top());
		mvStack.top() *= rotateX(toRadians(90));
		mvStack.push(mvStack.top());
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
		glBindTexture(GL_TEXTURE_2D, metalTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glPrimitiveRestartIndex(0xFFFF);
		glEnable(GL_PRIMITIVE_RESTART);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSpoke[3]);
		glDrawElements(GL_TRIANGLE_STRIP, mySpoke.getIndices().size(), GL_UNSIGNED_INT, 0);
		mvStack.pop();
	}
	


	//Hub LEfty
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, -2, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(wR));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboHub[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHub[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myHub.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//RimLeft
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0.4, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotateX(wR);
	//mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboRim[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRim[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myRim.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	//RimSpokesLeft
	//1
	for (int i = 0; i < 4; i++)
	{
		mvStack.push(mvStack.top());
		mvStack.top() *= rotateZ(toRadians((i * 360) / 4));
		mvStack.push(mvStack.top());
		mvStack.top() *= buildTranslate(0.4, 0, 0);
		mvStack.push(mvStack.top());
		mvStack.top() *= rotateX(toRadians(90));
		mvStack.push(mvStack.top());
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
		glBindTexture(GL_TEXTURE_2D, metalTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glPrimitiveRestartIndex(0xFFFF);
		glEnable(GL_PRIMITIVE_RESTART);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSpoke[3]);
		glDrawElements(GL_TRIANGLE_STRIP, mySpoke.getIndices().size(), GL_UNSIGNED_INT, 0);
		mvStack.pop();
	}

	//Cheel lefr
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0.3,0.3 , -1.1);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCheek[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myCheek.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Cheel right
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 1, 0);
	mvStack.push(mvStack.top());

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCheek[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myCheek.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();


	//Trunio 
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-1.5,-1, -0.1);
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(1.5, 0.7, 0.5);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateX(toRadians(trunnionR));
	mvStack.push(mvStack.top());

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTrunnion[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myTrunnion.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	
	//barrel
	
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(-90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-1.6,0.68,-0.4);
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(6, 1.4, 3);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboBarrel[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myBarrel.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	//mvStack.pop();
	//Stock
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(1, 0.6, 0.6);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(200));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0,0, -0.9);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboStock[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodNailTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboStock[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myStock.getIndices().size(), GL_UNSIGNED_INT, 0);

	mvStack.pop();

	//Screw
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(250));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-0.6, 0, -0.2);
	mvStack.push(mvStack.top());
	mvStack.top() *= screwTx;
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateX(toRadians(screwR));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(2.5, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboScrew[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myScrew.getIndices().size(), GL_UNSIGNED_INT, 0);

	mvStack.pop();

	


	




	



	

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::mat4 Mtemp = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		Az += 10;
		zRot = glm::quat(cosf(toRadians(Az) / 2), 0, 0, sinf(toRadians(Az) / 2));


	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		Az -= 10;
		zRot = glm::quat(cosf(toRadians(Az) / 2), 0, 0, sinf(toRadians(Az) / 2));
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		Ay += 10;
		yRot = glm::quat(cosf(toRadians(Ay) / 2), 0, sinf(toRadians(Ay) / 2), 0);

	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		Ay -= 10;
		yRot = glm::quat(cosf(toRadians(Ay) / 2), 0, sinf(toRadians(Ay) / 2), 0);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		trunnionR += 5;
		screwR += 10;
		screwTx *= buildTranslate(-0.02, 0, 0);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		trunnionR -= 5;
		screwR -= 10;
		screwTx *= buildTranslate(0.02, 0, 0);
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		wR += 10;
		wheelRot = glm::quat(cosf(toRadians(Ay) / 2), sinf(toRadians(wR) / 2), 0, 0);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		wR -= 10;
		wheelRot = glm::quat(cosf(toRadians(Ay) / 2), sinf(toRadians(wR) / 2), 0, 0);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		scaleV += 0.01;
		scale = buildScala(scaleV, scaleV, scaleV);

	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		scaleV -= 0.01;
		scale = buildScala(scaleV, scaleV, scaleV);

	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		V += glm::vec4(0,0.1,0,0);


	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		V -= glm::vec4(0, 0.1, 0, 0);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		V -= glm::vec4(0.1, 0, 0, 0);
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		V += glm::vec4(0.1, 0, 0, 0);
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		V -= glm::vec4(0, 0, 0.1, 0);
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		V += glm::vec4(0, 0, 0.1, 0);
	}

}


void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);
}





int main(void) {
	Ax = Ay = 0;
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(1500, 1000, "here we go again program3", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	M = buildScala(0.5, 0.5, 0.5) * M;
	mMat = M;
	mvMat = mMat * vMat;
	display(window, glfwGetTime(), Ax, Ay, Az, temp);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_movement_callback);

	while (!glfwWindowShouldClose(window)) {
		mvMat = mMat * vMat;
		glfwSwapBuffers(window);
		display(window, glfwGetTime(), Ax, Ay, Az, temp);
		glfwSetKeyCallback(window, key_callback);
		glfwWaitEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}