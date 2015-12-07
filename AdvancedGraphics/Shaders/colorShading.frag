#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D sampler;
uniform float time;

void main() {
	vec4 textureColor = texture(sampler, fragmentUV);
	//color = textureColor*fragmentColor;
	color = 1-textureColor*vec4(fragmentColor.r*(cos(fragmentPosition.x*4.0+time)+1.0)*0.5, fragmentColor.g*(cos(fragmentPosition.y*8.0+time)+1.0)*0.5, fragmentColor.b*(cos(fragmentPosition.x*2.0+time)+1.0)*0.5, fragmentColor.a);
}