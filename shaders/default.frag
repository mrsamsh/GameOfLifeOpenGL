#version 330 core

in vec2 TexCoord;
in vec4 Color;
in float TexIndex;

out vec4 FragColor;

void main()
{
  FragColor = Color;
}
