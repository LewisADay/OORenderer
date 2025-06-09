#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	// linearly interpolate between texture and set colour (80% texture, 20% vertex colour)
	FragColor = mix(texture(texture1, TexCoord), vec4(ourColor, 1.0f), 0.2f);
}
