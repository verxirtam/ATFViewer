#version 400

in vec2 textureCoord;

uniform sampler2D tex;

uniform vec3 stringColor;

layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(stringColor, texture(tex, textureCoord).r);
}

