#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gRoughMetal;
layout (location = 3) out vec4 gAlbedo;


in VS_OUT{
   mat3 TBN;
   vec2 TexCoord;
   vec3 FragPos;
}fs_in;


struct Material{
  sampler2D DiffuseMap;
  sampler2D NormalMap;
  sampler2D RoughnessMap;
  sampler2D MetallicMap;
  vec3 DiffuseVal;
  float RoughnessVal;
  float MetallicVal;
  vec4 flag;
};
uniform Material material;

void main()
{
  vec3  A = material.flag.r != 0 ?  pow( texture(material.DiffuseMap,fs_in.TexCoord).xyz,vec3(2.2)) : max(vec3(.001f),material.DiffuseVal.xyz);
  float R = material.flag.g != 0 ? texture(material.RoughnessMap,fs_in.TexCoord).x  : material.RoughnessVal;
  float M = material.flag.b != 0 ? texture(material.MetallicMap,fs_in.TexCoord).x   : material.MetallicVal;
  vec3 N  = texture(material.NormalMap,fs_in.TexCoord).rgb;
  N = material.flag.a  != 0 ?  normalize(fs_in.TBN*normalize(N * 2.0 - 1.0)):fs_in.TBN[2] ;

  gPosition   = fs_in.FragPos;
  gNormal     = N;
  gRoughMetal = vec4(R,M,0,1);
  gAlbedo     = vec4(A,1);

}
