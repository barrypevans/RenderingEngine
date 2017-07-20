#include "Texture.h"
#include "stb_image.h"
#include <iostream>
Texture::Texture(std::string fileName)
{

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	imagedata = stbi_load(fileName.c_str(), &width, &height, &nrComponents, STBI_rgb);
	stbi_set_flip_vertically_on_load(false);
	//imagedata = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	//std::cout << "Image Size: " << width << std::endl;
	glGenTextures(1, &handle);
	Bind();
	if (NULL!=imagedata)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
			GLfloat fLargest;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load image." << std::endl;
	}

	UnBind();
}

Texture::Texture(GLuint h)
{

	handle = h;
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
	stbi_image_free(imagedata);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D,handle);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
