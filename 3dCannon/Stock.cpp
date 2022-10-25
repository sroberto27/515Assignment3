#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include <vector>
#include <random>
#include "Stock.h"
using namespace std;






Stock::Stock() {
	initStock(0, 0, 0, 0, 0, 0, 0, 0);
}
Stock::Stock(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy) {
	initStock(initx, inity, x1, y1, x2, y2, endx, endy);
}


float Stock::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Stock::setVertices(int num) {
	numVertices = num;
}


void Stock::readPointFileStock(string f) {
	int counter = 0;
	float xt = 0;
	float yt = 0;
	string line = "";
	ifstream myfile(f);
	ifstream myfile2(f);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			counter++;
		}
		myfile.close();
	}
	else cout << "Unable to open file";

	for (int i = 0; i <= counter; i++) { verticesTemp.push_back(glm::vec3()); }

	int temp = 0;
	if (myfile2.is_open())
	{
		while (getline(myfile2, line)) {

			string val = "";
			for (auto x : line)
			{
				if (x == ' ')
				{
					xt = stod(val);
					val = "";
					if (temp == 0)
					{
						inner = xt;
					}
					if (temp == counter - 1)
					{
						outer = xt;
					}
				}
				else {
					val = val + x;
				}
			}
			yt = stod(val);
			verticesTemp[temp] = glm::vec3(xt, yt, 0);
			cout << " temVErtread x" << xt << "  y  " << yt << endl;

			temp++;


		}
		myfile2.close();
	}
	else
		cout << "Unable to open file";
	//numVertices = counter;
	prec = (outer - inner) * 1000;
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	//prec = (outer- inner)*1000;
	cout << "pass by" << endl;
	//numVertices = (numVertices + 1) *prec;
	//numIndices = prec * numVertices*6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { sTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { tTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }
	int cuatro = verticesTemp.size() / 4;
	int cuattroCounter = cuatro;
	float amt = 1;
	for (size_t i = 0; i < verticesTemp.size(); i++)
	{

		amt = (float)toRadians((float)i * 360.0f / (prec));



		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 initPos(rMat * glm::vec4(0, 0, 0, 1.0f));
		vertices[i] = glm::vec3(initPos + glm::vec3(verticesTemp[i].x, verticesTemp[i].y, 0));
		//vertices[i] = verticesTemp[i];
		cout << "--xt: " << verticesTemp[i].x << " yt: " << verticesTemp[i].y << " vertex num: " << i;
		cout << "--vertexs " << vertices[i].x << " , " << vertices[i].y << " , " << vertices[i].z << endl;
		texCoords[i] = glm::vec2(0.0f, ((float)i / (float)verticesTemp.size()));

		rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));

		sTangents[i] = glm::vec3(glm::vec3(0.0f, -1.0f, 0.0f));
		normals[i] = glm::cross(tTangents[i], sTangents[i]);
	}

}
void Stock::initStock(float initx, float inity, float x1, float y1, float x2, float y2, float endx, float endy) {
	float x[4], y[4];



	// calculate first ring
	readPointFileStock("Stock.txt");

	// rotate the first ring about Y to get the other rings
	int cuatro = verticesTemp.size() / 4;
	int cuattroCounter = cuatro;
	float amt = 1;
	for (int ring = 1; ring < prec; ring++) {
		for (int i = 0; i < verticesTemp.size(); i++) {

			amt = (float)toRadians((float)ring * 360.0f / (prec));
			cuattroCounter += cuatro;



			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			//vertices[0] = glm::vec3(inner + outer + 0.01, 0.0f, 0.0f);
			vertices[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

			texCoords[ring*(prec + 1) + i] = glm::vec2((float)ring*2.0f / (float)prec, texCoords[i].t);
			//if (texCoords[ring*(prec + 1) + i].s > 1.0) texCoords[ring*(prec+1)+i].s -= 1.0f; // deleted and added someting at end of inint

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			sTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			tTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(1.0f, 0.0f, 0.0f));
			normals[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(normals[i], 1.0f));
		}
	}
	// calculate triangle indices

	for (int ring = 0; ring < prec; ring++) {
		for (int i = 0; i < prec; i++) {
			indices[((ring*prec + i) * 2) * 3 + 0] = ring * (prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 2] = ring * (prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 0] = 0xFFFF;
			indices[((ring*prec + i) * 2 + 1) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2 + 1) * 3 + 2] = (ring + 1)*(prec + 1) + i + 1;

		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//seen to not be needed

}
int Stock::getNumVertices() { return numVertices; }
int Stock::getNumIndices() { return numIndices; }
std::vector<int> Stock::getIndices() { return indices; }
std::vector<glm::vec3> Stock::getVertices() { return vertices; }
std::vector<glm::vec2> Stock::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Stock::getNormals() { return normals; }
std::vector<glm::vec3> Stock::getStangents() { return sTangents; }
std::vector<glm::vec3> Stock::getTtangents() { return tTangents; }