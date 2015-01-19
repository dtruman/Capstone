#version 400

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 UV;

uniform mat4 modelToWorldMatrix;
uniform mat4 fullTransformMatrix;

out vec4 pos;
out vec4 norm;
out vec2 fragUV;

void main()
{
	gl_Position=fullTransformMatrix*vec4(position,1.0f);
	norm=modelToWorldMatrix*vec4(normal,0.0f);
	pos=modelToWorldMatrix*vec4(position,1.0f);
	fragUV=UV;
}