#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texcoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out VS_OUT{
   mat3 TBN;
   vec2 TexCoord;
   vec3 FragPos;
}vs_out;

void main()
{
    gl_Position = projection*view*model* vec4(position, 1.0);

    vs_out.TexCoord =texcoords;

    vec4 pos = view*model*vec4(position, 1.0);

    vs_out.FragPos = vec3(model * vec4(position, 1.0f));

    vec3 norm = normalize(vec3(model * vec4(normal,    0.0)));

    vec3 tan =normalize(vec3(model * vec4(tangent,    0.0)));

    tan = normalize(tan - dot(tan, norm) * norm);
    vec3 binorm =cross(norm,tan);

    vs_out.TBN = mat3(tan,binorm,norm);


}
