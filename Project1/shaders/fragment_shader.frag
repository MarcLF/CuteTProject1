#version 330

layout(location = 0)out vec4 outColor;

in Data
{
    vec3 normalLocalspace;
    vec2 textCoord;
    vec3 positionViewspace;
} FSIn;

uniform mat4 worldViewMatrix;

uniform sampler2D albedoTexture;

uniform int rendererMode;

void main(void)
{
    float ambientTerm = 0.05f;

    vec4 albedo;
    vec4 ambient;

    albedo = texture2D(albedoTexture, FSIn.textCoord);
    ambient = albedo * ambientTerm;
    ambient.a = 1.0f;

    if(rendererMode == 0)
    {
        outColor.rgb = ambient.rgb + albedo.rgb;
        outColor.a = 1.0f;
    }
    else if (rendererMode == 1)
    {
        vec3 lightDir = vec3(0.0f,1.0f,0.0f);
        vec3 lightColor = vec3(1.0f,1.0f,1.0f);

        outColor.rgb = ambient.rgb + albedo.rgb * dot(lightDir,FSIn.normalLocalspace) * lightColor;
        outColor.a = 1.0f;
    }
    else if(rendererMode == 2)
    {
        outColor.r = FSIn.normalLocalspace.r;
        outColor.g = FSIn.normalLocalspace.g;
        outColor.b = FSIn.normalLocalspace.b;
        outColor.a = 1.0f;
    }
    else if(rendererMode == 3)
    {
        outColor = vec4(vec3(gl_FragCoord.z) * vec3(0.36f, 0.75f, 0.72f), gl_FragCoord.z);
    }
}
