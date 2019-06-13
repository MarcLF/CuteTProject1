#version 330

in Data
{
    vec2 texCoords;
} FSIn;

out vec4 outColor;

uniform sampler2D sampleTexture;
uniform sampler2D blurTexture;
uniform sampler2D depthTexture;

uniform float farPlane;
uniform float nearPlane;

uniform float farFallOff;
uniform float nearFallOff;

void main(void)
{
    float depth = texture2D(depthTexture, FSIn.texCoords).r;

    if(depth < nearFallOff)
    {
        vec4 blured = texture2D(blurTexture, FSIn.texCoords);
        vec4 light = texture2D(sampleTexture, FSIn.texCoords);
       
        float ratio = (nearFallOff - depth) /(nearFallOff - nearPlane);
        outColor = mix(light, blured, ratio);
       
        if(depth < nearPlane)
             outColor = texture2D(blurTexture, FSIn.texCoords);
    }
    else if (depth > farFallOff)
    {
        vec4 blured = texture2D(blurTexture, FSIn.texCoords);
        vec4 light = texture2D(sampleTexture, FSIn.texCoords);
        float ratio = (farPlane - depth) /(farPlane - farFallOff);
      
        outColor = mix(blured, light, ratio);
     
        if(depth > farPlane)
            outColor = texture2D(blurTexture, FSIn.texCoords);
    }
    else
    {
        outColor = texture2D(sampleTexture, FSIn.texCoords);
    }

}
