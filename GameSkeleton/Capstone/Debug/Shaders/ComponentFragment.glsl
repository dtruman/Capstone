#version 400

in vec2 fragUV;

uniform sampler2D texture;

out vec4 theFinalColor;

void main()
{
	theFinalColor = vec4(vec3(texture2D(texture, fragUV)), 1.0f);
}