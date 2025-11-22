#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D tex;
uniform float alpha;

void main()
{
	vec4 texColor = texture(tex, TexCoord);
	texColor *= alpha;
	FragColor = texColor;
}
