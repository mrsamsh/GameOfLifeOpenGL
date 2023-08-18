#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTexIndex;

uniform mat4 projection;

out vec2 TexCoord;
out vec4 Color;
out float TexIndex;

void main()
{
  gl_Position = projection * vec4(aPosition, 1.0);
  TexCoord = aTexCoord;
  Color = aColor;
  TexIndex = aTexIndex;
}
