#include "OBJLoader.h"


Mesh_Data* OBJLoader::Load(std::string fileName)
{
	std::vector<float> positions ;
	std::vector<float> normals;
	std::vector<float> TexCoords;
	std::vector<GLuint> indices;
	std::vector<GLuint> normalindices;
	std::vector<GLuint> TexCoordindices;
	


	std::string txt;
	std::ifstream file(fileName);
	if (file.is_open()) {
		while (file.good())
		{
			getline(file, txt);
			if (txt.length() > 2) {
				if (txt.at(0) == 'v') {
					if (txt.at(1) == ' ') {
						processVertex(&txt, &positions);

					}
					else if (txt.at(1) == 't') {
						processTexcoords(&txt, &TexCoords);

					}
					else if (txt.at(1) == 'n')

						processNormals(&txt, &normals);
				}

				else if (txt.at(0) == 'f')
					processFaces(&txt, &indices, &normalindices, &TexCoordindices);
			}
		}
	}
	else {
		std::cout << "ERROR file not found - Failed to load OBJ:  "<< fileName << std::endl;
	}
	/* Debug Print
	std::cout << "Verts: " << std::endl;
	for (int i = 0; i < positions.size() / 3; i++)
		std::cout << (*((std::vector<Vector3>*)&positions))[i].to_string() << std::endl;

	std::cout << "Indices: " << std::endl;
	for (int i = 0; i < indices.size() ; i++)
		std::cout << indices[i] << std::endl;

	std::cout << "Normal Indices: " << std::endl;
	for (int i = 0; i < normalindices.size(); i++)
		std::cout << normalindices[i] << std::endl;

	std::cout << "TexCoord Indices: " << std::endl;
	for (int i = 0; i < TexCoordindices.size(); i++)
		std::cout << TexCoordindices[i] << std::endl;

	std::cout << "Norms: " << std::endl;
	for (int i = 0; i < normals.size()/3; i++)
		std::cout<<(*((std::vector<Vector3>*)&normals))[i].to_string()<< std::endl;

	std::cout << "TexCoords: " << std::endl;
	for (int i = 0; i < TexCoords.size() / 2; i++)
		std::cout << (*((std::vector<Vector2>*)&TexCoords))[i].to_string() << std::endl;
		*/
	Mesh_Data* data= new Mesh_Data();
	std::vector<Vertex> verticies;
	std::vector<Vector3> renderedNorms = RenderListv3(normalindices, normals);
	std::vector<Vector3> renderedPositions = RenderListv3(indices, positions);
	std::vector<Vector2> renderedtexcoords = RenderListv2( TexCoordindices, TexCoords);
	std::vector<Vector3> tangents = CalculateTangents(TexCoordindices.empty() ? (*(std::vector<Vector3>*)&positions) : renderedPositions, TexCoordindices.empty() ? (*(std::vector<Vector2>*)&TexCoords) : renderedtexcoords);
	for (int i = 0; i < renderedPositions.size() ; i++) {
	
		verticies.push_back(Vertex(renderedPositions[i],normals.empty()? Vector3(0,0,0) : (normalindices.empty() ? (*((std::vector<Vector3>*)&normals))[i] : renderedNorms[i]), tangents[i],TexCoords.empty()?Vector2(0,0): (TexCoordindices.empty()? (*((std::vector<Vector2>*)&TexCoords))[i] : renderedtexcoords[i])));
	}
	//std::cout << "normsize: " << normalindices.size() << std::endl;
	//std::cout << "possize: " << indices.size() << std::endl;
	data->indices = indices;
	data->verticies = verticies;
	return data;
}

void OBJLoader::processVertex(std::string* input, std::vector<float>* verts)
{
	input->erase(input->begin());

	std::vector<std::string> comps = OBJLoader::split(*input, ' ');
	
	verts->push_back(strtof(comps.at(1).c_str(), NULL));
	verts->push_back(strtof(comps.at(2).c_str(), NULL));
	verts->push_back(strtof(comps.at(3).c_str(), NULL));
	

}

void OBJLoader::processNormals(std::string * input, std::vector<float>* normals)
{
	input->erase(input->begin());
	input->erase(input->begin());

	std::vector<std::string> comps = OBJLoader::split(*input, ' ');

	normals->push_back(strtof(comps.at(1).c_str(), NULL));
	normals->push_back(strtof(comps.at(2).c_str(), NULL));
	normals->push_back(strtof(comps.at(3).c_str(), NULL));
}

void OBJLoader::processTexcoords(std::string * input, std::vector<float>* Texcoords)
{

	input->erase(input->begin());
	input->erase(input->begin());
	std::vector<std::string> comps = OBJLoader::split(*input, ' ');

	Texcoords->push_back(strtof(comps.at(1).c_str(), NULL));
	Texcoords->push_back(strtof(comps.at(2).c_str(), NULL));

}

void OBJLoader::processFaces(std::string * input, std::vector<GLuint>* indices, std::vector<GLuint>* normalindices, std::vector<GLuint>* TexCoordindices)
{
	input->erase(input->begin());
	std::vector<std::string> comps = OBJLoader::split(*input, ' ');

	for (std::string s : comps) {
		processSingleFace(&s, indices, normalindices, TexCoordindices);
	}

}


void OBJLoader::processSingleFace(std::string * input, std::vector<GLuint>* indices, std::vector<GLuint>* normalindices, std::vector<GLuint>* TexCoordindices)
{
	std::vector<std::string> comps = OBJLoader::split(*input, '/');
	if (comps.size() >= 1)
		indices->push_back(std::stoi(comps.at(0).c_str(), NULL)-1);
	if (comps.size() >= 2)
		if(!comps.at(1).empty())
			TexCoordindices->push_back(strtof(comps.at(1).c_str(), NULL)-1);
	if (comps.size() >= 3)
		normalindices->push_back(strtof(comps.at(2).c_str(), NULL) - 1);
}

std::vector<std::string> OBJLoader::split(std::string str, char delimiter) {
	std::vector<std::string> in;
	std::stringstream ss(str); // Turn the string into a stream.
	std::string tok;

	while (getline(ss, tok, delimiter)) {
		in.push_back(tok);
	}

	return in;
}


std::vector<Vector3> OBJLoader::RenderListv3( std::vector<GLuint> index, std::vector<float> list) {
	std::vector<Vector3> result;
	std::vector<Vector3> list3 = (*((std::vector<Vector3>*)&list));
	for (int i = 0; i < index.size(); i++) {
		result.push_back(list3[index[i]]);
	}
	return result;
}

std::vector<Vector2> OBJLoader::RenderListv2(std::vector<GLuint> index, std::vector<float> list) {
	std::vector<Vector2> result;
	std::vector<Vector2> list2 = (*((std::vector<Vector2>*)&list));
	for (int i = 0; i < index.size(); i++) {
		result.push_back(list2[index[i]]);
	}
	return result;
}

std::vector<Vector3> OBJLoader::CalculateTangents(std::vector<Vector3> renderedVerts, std::vector<Vector2> renderedUVs)
{
	std::vector<Vector3> result;

	for (int i = 0; i < renderedVerts.size(); i+=3 ) {
		Vector3 v1 = renderedVerts[i];
		Vector3 v2 = renderedVerts[i+1];
		Vector3 v3 = renderedVerts[i+2];
		Vector2 uv1 = renderedUVs[i];
		Vector2 uv2 = renderedUVs[i+1];
		Vector2 uv3 = renderedUVs[i+2];
		Vector3 edge1 = v2 - v1;
		Vector3 edge2 = v3 - v1;
		Vector2 deltaUV1 = uv2 - uv1;
		Vector2 deltaUV2 = uv3 - uv1;
		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		Vector3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent.Normalize();
		result.push_back(tangent);
		result.push_back(tangent);
		result.push_back(tangent);
	}

	return result;
}
