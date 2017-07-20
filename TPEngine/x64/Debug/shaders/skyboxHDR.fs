#version 330 core
out vec4 FragColor;

in vec3 localPos;

uniform samplerCube skybox;
uniform samplerCube skybox_diffuseIrradiance;
uniform samplerCube skybox_specIrradiance;

void main()
{
    vec3 envColor = texture(skybox, localPos,0).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    FragColor = vec4(envColor, 1.0);
}
