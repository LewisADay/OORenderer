#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D DiffuseTexture1;

void main()
{    
    FragColor = texture(DiffuseTexture1, TexCoord);
}