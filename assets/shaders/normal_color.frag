#version 460 core

in vec3 vNormal;
out vec4 FragColor;

uniform vec3 uColor; // Color base opcional si decides usarlo

void main()
{
    // Verificación gráfica de normales (mapea la dirección al espacio de color RGB)
    FragColor = vec4(vNormal * 0.5 + 0.5, 1.0);
}