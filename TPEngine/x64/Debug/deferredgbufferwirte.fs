#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gTangent;
layout (location = 3) out vec4 gRoughMetal;
layout (location = 4) out vec4 gAlbedo;


in VS_OUT{
   vec3 Normal;
   vec3 Tangent;
   vec2 TexCoord;
   vec3 FragPos;
}fs_in;


struct Material{
  sampler2D DiffuseMap;
  sampler2D NormalMap;
  sampler2D RoughnessMap;
  sampler2D MetallicMap;
  vec4 DiffuseVal;
  vec2 RoughnessVal;
  vec2 MetallicVal;
};
uniform Material material;

void main()
{
  vec3  A = material.DiffuseVal.w   == 0 ?  texture(material.DiffuseMap,fs_in.TexCoord).xyz : material.DiffuseVal.xyz;
  float R = material.RoughnessVal.y == 0 ? texture(material.RoughnessMap,fs_in.TexCoord).x  : material.RoughnessVal.x;
  float M = material.MetallicVal.y  == 0 ? texture(material.MetallicMap,fs_in.TexCoord).x   : material.MetallicVal.x;

  gPosition   = fs_in.FragPos;
  gNormal     = fs_in.Normal;
  gTangent    = fs_in.Tangent;
  gRoughMetal = vec4(R*.1f,M,0,1);
  gAlbedo     = vec4(A,1);

}
