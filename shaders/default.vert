#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTranslation;
layout (location = 2) in vec4 aColor;

uniform mat4 projection;

out vec4 Color;

void main()
{
  vec2 translated_pos = aPosition + aTranslation;
  gl_Position = projection * vec4(translated_pos, 0, 1.0);
  Color = aColor;
}
