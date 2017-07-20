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
void main()
{
  if( length(texture(gbuffer.gAlbedo,TexCoord).r)==0)
  discard;

  if(TexCoord.y>.5&&TexCoord.x>.5)
    color =  texture(gbuffer.gNormal,TexCoord);
  else if(TexCoord.y<.5&&TexCoord.x>.5)
    color =  texture(gbuffer.gPosition,TexCoord);
  else if(TexCoord.y<.5&&TexCoord.x<.5)
    color =  texture(gbuffer.gRoughMetal,TexCoord);
  else
    color =  texture(gbuffer.gAlbedo,TexCoord);
  /*vec4 normal = texture(gbuffer.gNormal,TexCoord);
float nl = max(0,dot(normal.xyz, vec3(1/sqrt(3),1/sqrt(3),0)));
  color =vec4(nl)+.3f;*/
}
