#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texcoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out VS_OUT{
   vec3 Normal;
   vec3 Tangent;
   vec2 TexCoord;
   vec3 FragPos;
}vs_out;

void main()
{
    gl_Position = projection*view*model* vec4(position, 1.0);

    vs_out.TexCoord =texcoords;

    vec4 pos = view*model*vec4(position, 1.0);

    vs_out.FragPos = vec3(model * vec4(position, 1.0f));

    vs_out.Normal = mat3(transpose(inverse(model))) * normal;

    vs_out.Tangent =mat3(transpose(inverse(model))) * tangent;

}
