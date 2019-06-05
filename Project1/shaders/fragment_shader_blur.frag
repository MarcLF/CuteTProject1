#version 330

layout(location = 0)out vec4 outColor;

in Data
{
    vec2 textCoord;
} FSIn;

uniform sampler2D albedoTexture;

uniform int blurActivated;

uniform vec2 blurDir;

uniform float radius;

uniform float resolution;

void main(void)
{
    if(blurActivated == 1)
    {
        vec4 sum = vec4(0.0);

        float blur = radius/resolution;

        float hstep = blurDir.x;
        float vstep = blurDir.y;

        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x - 4.0*blur*hstep, FSIn.textCoord.y - 4.0*blur*vstep)) * 0.0162162162;
        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x - 3.0*blur*hstep, FSIn.textCoord.y - 3.0*blur*vstep)) * 0.0540540541;
        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x - 2.0*blur*hstep, FSIn.textCoord.y - 2.0*blur*vstep)) * 0.1216216216;
        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x - 1.0*blur*hstep, FSIn.textCoord.y - 1.0*blur*vstep)) * 0.1945945946;

        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x, FSIn.textCoord.y)) * 0.2270270270;

        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x + 1.0*blur*hstep, FSIn.textCoord.y + 1.0*blur*vstep)) * 0.1945945946;
        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x + 2.0*blur*hstep, FSIn.textCoord.y + 2.0*blur*vstep)) * 0.1216216216;
        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x + 3.0*blur*hstep, FSIn.textCoord.y + 3.0*blur*vstep)) * 0.0540540541;
        sum += texture2D(albedoTexture, vec2(FSIn.textCoord.x + 4.0*blur*hstep, FSIn.textCoord.y + 4.0*blur*vstep)) * 0.0162162162;

        outColor = outColor * vec4(sum.rgb, 1.0);
    }
}
