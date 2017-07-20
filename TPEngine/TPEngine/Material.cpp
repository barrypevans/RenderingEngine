#include "Material.h"


Material::Material(Shader* s) {
	if (shader != NULL)
		delete(shader);
	shader = s;
	shader->Init();
}

Material::~Material()
{

}

void Material::Bind()
{
	int texCounter = 0;
	shader->Bind();
	for (int i = 0; i < activeTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texCounter);
		activeTextures[i]->Bind();
		glUniform1i(glGetUniformLocation(shader->shaderProgram, activeTextureLocations[i].c_str()), texCounter);
		//activeTextures[i]->UnBind();
		texCounter++;
	}

	for (int i = 0; i < activeCubemap.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texCounter);
		activeCubemap[i]->Bind();
		glUniform1i(glGetUniformLocation(shader->shaderProgram, activeCubemapLocations[i].c_str()), texCounter);
		texCounter++;
	}

	for (int i = 0; i < activeCubemapHDR.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texCounter);
		activeCubemapHDR[i]->Bind();
		glUniform1i(glGetUniformLocation(shader->shaderProgram, activeCubemapHDRLocations[i].c_str()), texCounter);
		texCounter++;

		if (activeCubemapHDR[i]->hasIrradianceMap()) {
			glActiveTexture(GL_TEXTURE0 + texCounter);
			activeCubemapHDR[i]->BindDiffuseIrradiance();
			glUniform1i(glGetUniformLocation(shader->shaderProgram, (activeCubemapHDRLocations[i] + "_diffuseIrradiance").c_str()), texCounter);
			texCounter++;

			glActiveTexture(GL_TEXTURE0 + texCounter);
			activeCubemapHDR[i]->BindSpecIrradiance();
			glUniform1i(glGetUniformLocation(shader->shaderProgram, (activeCubemapHDRLocations[i] + "_specIrradiance").c_str()), texCounter);
			texCounter++;
		}
	}


	for (int i = 0; i < uniforms.size(); i++) {
		std::string loc = uniformLocations[i];
		Uniform u = uniforms[i];
		switch (uniforms[i].type) {
		case	Uniform::UType::Matrix : BindMat(loc, u.mat); break;
		case	Uniform::UType::MatrixGLM:BindMatGLM(loc, u.glmmat); break;
		case	Uniform::UType::Vec2:BindVector(loc, u.v2); break;
		case	Uniform::UType::Vec3:BindVector(loc, u.v3); break;
		case	Uniform::UType::Vec4:BindVector(loc, u.v4); break;
		case	Uniform::UType::Float:BindFloat(loc, u.f); break;
		}
		
	}
	shader->Bind();
}
void Material::UnBind()
{
	shader->UnBind();
	int texCounter = 0;
	for (int i = 0; i < activeTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		activeTextures[i]->UnBind();
		
	}
	for (int i = 0; i < activeTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texCounter);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		texCounter++;
	}
	for (int i = 0; i < activeTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texCounter);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		texCounter++;
	}
}

void Material::SetFloat(std::string location, float val)
{
	for (int i = 0; i < uniformLocations.size(); i++)
		if (location.compare(uniformLocations[i]) == 0)
		{
			uniforms[i].f=val;
			uniforms[i].type = Uniform::UType::Float;
			return;
		}


	uniforms.push_back(Uniform(val, Uniform::UType::Float));
	uniformLocations.push_back(location);



}

void Material::SetVector(std::string location, Vector4 val)
{
	for (int i = 0; i < uniformLocations.size(); i++)
		if (location.compare(uniformLocations[i]) == 0)
		{
			uniforms[i].v4 = val;
			uniforms[i].type = Uniform::UType::Vec4;
			return;
		}


	uniforms.push_back(Uniform(val, Uniform::UType::Vec4));
	uniformLocations.push_back(location);

}

void Material::SetVector(std::string location, Vector3 val)
{
	for (int i = 0; i < uniformLocations.size(); i++)
		if (location.compare(uniformLocations[i]) == 0)
		{
			uniforms[i].v3 = val;
			uniforms[i].type = Uniform::UType::Vec3;
			return;
		}


	uniforms.push_back(Uniform(val, Uniform::UType::Vec3));
	uniformLocations.push_back(location);

}

void Material::SetVector(std::string location, Vector2 val)
{
	for (int i = 0; i < uniformLocations.size(); i++)
		if (location.compare(uniformLocations[i]) == 0)
		{
			uniforms[i].v2 = val;
			uniforms[i].type = Uniform::UType::Vec2;
			return;
		}


	uniforms.push_back(Uniform(val, Uniform::UType::Vec2));
	uniformLocations.push_back(location);

}

void Material::SetMat(std::string location, Matrix4x4 val)
{
	for (int i = 0; i < uniformLocations.size(); i++)
		if (location.compare(uniformLocations[i])==0)
		{
			uniforms[i].mat = val;
			uniforms[i].type = Uniform::UType::Matrix;
			return;
		}


	uniforms.push_back(Uniform(val, Uniform::UType::Matrix));
	uniformLocations.push_back(location);

}

void Material::SetMatGLM(std::string location, glm::mat4x4 val)
{
	for (int i = 0; i < uniformLocations.size(); i++)
		if (location.compare(uniformLocations[i]) == 0)
		{
			uniforms[i].glmmat = val;
			uniforms[i].type = Uniform::UType::MatrixGLM;
			return;
		}


	uniforms.push_back(Uniform(val, Uniform::UType::MatrixGLM));
	uniformLocations.push_back(location);

}


void Material::BindFloat(std::string location, float val) {
	shader->Bind();
	GLint loc = glGetUniformLocation(shader->shaderProgram, location.c_str());
	glUniform1f(loc, val);
	shader->UnBind();
}

void Material::BindVector(std::string location, Vector4 val)
{
	shader->Bind();
	GLint loc = glGetUniformLocation(shader->shaderProgram, location.c_str());
	glUniform4f(loc, val.x, val.y, val.z, val.w);
	shader->UnBind();
}

void Material::BindVector(std::string location, Vector3 val)
{
	shader->Bind();
	GLint loc = glGetUniformLocation(shader->shaderProgram, location.c_str());
	glUniform3f(loc, val.x, val.y, val.z);
	shader->UnBind();
}

void Material::BindVector(std::string location, Vector2 val)
{
	shader->Bind();
	GLint loc = glGetUniformLocation(shader->shaderProgram, location.c_str());
	glUniform2f(loc, val.x, val.y);
	shader->UnBind();
}

void Material::BindMat(std::string location, Matrix4x4 val)
{
	shader->Bind();
	GLuint loc = glGetUniformLocation(shader->shaderProgram, location.c_str());
	glUniformMatrix4fv(loc, 1, GL_TRUE, val.data);
	shader->UnBind();
}
void Material::BindMatGLM(std::string location, glm::mat4x4 val)
{
	shader->Bind();
	GLuint loc = glGetUniformLocation(shader->shaderProgram, location.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	shader->UnBind();

}

void Material::SetTexture(std::string location, Texture* tex)
{
	
	for (int i = 0; i < activeTextures.size(); i++) 
		if (tex == activeTextures[i]) 
			return;
	

		activeTextures.push_back(tex);
		activeTextureLocations.push_back(location);
		
	

	
}


void Material::SetCubeMap(std::string location, CubeMap* tex)
{

	for (int i = 0; i < activeCubemap.size(); i++)
		if (tex == activeCubemap[i])
			return;


	activeCubemap.push_back(tex);
	activeCubemapLocations.push_back(location);

}

void Material::SetCubeMap(std::string location, CubeMapHDR* tex)
{

	for (int i = 0; i < activeCubemapHDR.size(); i++)
		if (tex == activeCubemapHDR[i])
			return;


	activeCubemapHDR.push_back(tex);
	activeCubemapHDRLocations.push_back(location);

}