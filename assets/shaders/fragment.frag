#version 450 core

in vec3 vNormal;
out vec4 FragColor;

uniform vec4 uColor;

void main()
{
    // Pintar con colores enviados por uniform
    FragColor =uColor;
}