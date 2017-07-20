#include "CubeMap.h"
#include "stb_image.h"
CubeMap::CubeMap(std::string filename) 
{
	
	glGenTextures(1, &handle);
	glActiveTexture(GL_TEXTURE0);

	int width, height, nrComponents;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

	std::vector<std::string> faces;
	faces.push_back("/right.jpg");
	faces.push_back("/left.jpg");
	faces.push_back("/top.jpg");
	faces.push_back("/bottom.jpg");
	faces.push_back("/back.jpg");
	faces.push_back("/front.jpg");
	
	for (GLuint i = 0; i < faces.size(); i++)
	{
	
		
		image = stbi_load((filename + faces[i]).c_str(), &width, &height, &nrComponents, STBI_rgb);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
		stbi_image_free(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Bind() {

	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
}