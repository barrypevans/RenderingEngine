#pragma once
#include "Mesh_Data.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "3dMath.h"
#include "Vertex.h"
class OBJLoader {
public:
	static Mesh_Data* Load(std::string fileName);
	static void processVertex(std::string* input, std::vector<float>* verts);
	static void processNormals(std::string* input, std::vector<float>* normals);
	static void processTexcoords(std::string* input, std::vector<float>* Texcoords);
	static void processFaces(std::string* input, std::vector<GLuint>* indices, std::vector<GLuint>* normalindices, std::vector<GLuint>* Texcoords);
	static void processSingleFace(std::string* input, std::vector<GLuint>* indices, std::vector<GLuint>* normalindices, std::vector<GLuint>* Texcoords);
	static std::vector < std::string > split(std::string str, char delimiter);
	static std::vector<Vector3> RenderListv3(std::vector<GLuint> index, std::vector<float> list);
	static std::vector<Vector2> RenderListv2(std::vector<GLuint> index, std::vector<float> list);
	static std::vector<Vector3> CalculateTangents(std::vector<Vector3> renderedVerts, std::vector<Vector2> renderedUVs);
};