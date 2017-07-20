#version 330 core

out vec4 color;
in vec2 TexCoord;

struct GBuffer{
 sampler2D gPosition;
 sampler2D gNormal;
 sampler2D gRoughMetal;
 sampler2D gAlbedo;
};

uniform GBuffer gbuffer;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
};
#define MAX_LIGHTS 32
uniform Light lights[MAX_LIGHTS];

void main()
{
  if( length(texture(gbuffer.gAlbedo,TexCoord).r)==0)
  discard;

  vec3 c =vec3(.3f);

  for(int i=0; i< MAX_LIGHTS; i++){
    if(length(lights[i].color)==0)
      continue;
    c+=max(0,dot(texture(gbuffer.gNormal,TexCoord).xyz,lights[i].direction))*lights[i].color;
  }

  color=vec4(c,1);
}
