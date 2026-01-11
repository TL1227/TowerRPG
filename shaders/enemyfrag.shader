#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D tex;
uniform float alpha;

//lighting
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	vec4 texColor = texture(tex, TexCoord);
	texColor *= alpha;
	FragColor = texColor * vec4(lightColor, 1.0);
}
