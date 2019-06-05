#version 330 core

in Data
{
    vec2 texCoord;
} FSIn;

out vec4 outColor;

uniform sampler2D colorTexture;

void main(void)
{
    outColor = texture2D(colorTexture, FSIn.texCoord);
}
