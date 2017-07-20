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

uniform vec3 CameraPosition;

float PI= 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGXX(vec3 N, vec3 H, float roughness){
  float a = roughness*roughness;
  float a2 = a*a;
  float nh = dot(N,H);
  float nh2 = nh*nh;
  float denom =(nh2)*(a2-1)+1;
  denom=PI*denom*denom;
  return a2/(denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
void main()
{



  if( length(texture(gbuffer.gAlbedo,TexCoord).r)==0)
  discard;

  vec3 Lo=vec3(0);

  vec3 P = texture(gbuffer.gPosition,TexCoord).xyz;
  vec3 N = normalize(texture(gbuffer.gNormal,TexCoord).xyz);
  vec3 V = normalize(CameraPosition - P);
  float metallic =  texture(gbuffer.gRoughMetal,TexCoord).g;
  float roughness = texture(gbuffer.gRoughMetal,TexCoord).r;
  vec3 albedo = texture(gbuffer.gAlbedo,TexCoord).xyz;

  for(int i=0; i< MAX_LIGHTS; i++){

    if(length(lights[i].color)==0)
      continue;


      vec3 LightColor = lights[i].color;
      vec3 wi = normalize(lights[i].position - P);
      vec3 H = normalize(wi + V);
      float cosTheta = max(dot(H,V),0.0f);
      float distance = length(wi);
      float attenuation = 1 /(distance*distance);
      vec3 radiance = LightColor * attenuation;
      //.04f is the surface reflection at zero incidence for dielectrics
      //albedo is the value for metals
      vec3 F0 = mix( vec3(.04f), albedo, metallic );
      vec3 F = fresnelSchlick( cosTheta, F0 );

      float NDF = DistributionGXX(N,H,roughness);
      float G   = GeometrySmith(N, V, wi, roughness);
      vec3 numerator = NDF*G*F;
      float denomenator = 4 * max(0.0f,dot(N,wi)) * max(0.0f,dot(N,V))+.001f;
      vec3 specular = numerator / denomenator;

      vec3 kS = F;
      vec3 kD = vec3(1.0) - kS;
      kD *= 1.0f - metallic;

      float nl = max(0.0f, dot(N,wi));

      Lo += ( kD * albedo / PI + specular ) * radiance * nl;


    }

      vec3 ambient = vec3(0.03f) * albedo;

      color = vec4( ambient + Lo ,1);

      //Tonemapping
      color = color / (color + vec4(1.0));
      color = pow(color, vec4(1.0/2.2));
}
