#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <fstream>
#include <string>
#include <iostream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include <vector>
#include <random>
using namespace std;
class Spoke
{
private:
	int numIndices;
	int prec;
	int numVertices;
	float inner;
	float outer;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> sTangents;
	std::vector<glm::vec3> tTangents;

	void initSpoke(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy);
	float toRadians(float degrees);

public:
	std::vector<glm::vec3> verticesTemp;
	int numVerticesOrigin;
	void readPointFileSpoke(string f);
	Spoke();
	Spoke(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	void setVertices(int num);
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getStangents();
	std::vector<glm::vec3> getTtangents();
};