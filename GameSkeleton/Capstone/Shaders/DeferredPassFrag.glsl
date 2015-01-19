#version 400

in vec4 pos;
in vec4 norm;
in vec2 fragUV;

out vec4 theFinalColor;

uniform sampler2D tDiffuse;

void main()
{
	gl_FragData[0]=vec4(texture2D(tDiffuse, fragUV));
	gl_FragData[1]=vec4(pos);
	gl_FragData[2]=vec4(norm);
}