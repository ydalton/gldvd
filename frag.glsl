#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 ourColor;

void main()
{
	vec4 texColor = texture(ourTexture, TexCoord);
	if(texColor.a < 0.2)
		discard;
	FragColor = texColor * ourColor;
}
