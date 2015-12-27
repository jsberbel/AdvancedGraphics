#version 130
//Fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color; //gets outputted to the screen for each pixel

uniform sampler2D sampler;

void main() {
	vec4 textureColor = texture(sampler, fragmentUV);
	//color = textureColor*fragmentColor;
	color = fragmentColor * textureColor;
}