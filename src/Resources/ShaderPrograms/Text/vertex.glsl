#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 texCoords;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0, 1.0);
	texCoords = aTexCoords;
}