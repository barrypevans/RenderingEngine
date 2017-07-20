#pragma once
#include <vector>
#include "Vertex.h"
#include "GL\glew.h"
class Mesh_Data {
public:
	std::vector<Vertex> verticies;

	std::vector<GLuint> indices;



};