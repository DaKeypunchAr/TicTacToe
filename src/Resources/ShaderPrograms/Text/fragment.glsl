#version 450 core

uniform sampler2D uTexture;
uniform vec4 uColor;

in vec2 texCoords;

out vec4 oColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture, texCoords).r);
	oColor = uColor * sampled;
}