#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

const int lightCount = 8;
const int lightComponents = 3;
const float LIGHT_AMBIENT       = 0.0; //! Ambient light, only color is used.
const float LIGHT_DIRECTIONAL   = 1.0; //! Directional light, defined by direction and color.
const float LIGHT_POINT         = 2.0; //! Point light, defined by position, attenuation and color.
const float LIGHT_SPOT          = 3.0; //! Spot light, defined by position, direction, cut-off angle, attenuation and color.
uniform vec4 u_light[lightCount * lightComponents];
//light format:
//  +0 - position3 type1;
//  +1 - direction3 intensity1;
//  +2 - color3 cutoff1;

uniform sampler2D u_Normal_Depth;
uniform sampler2D u_CamCoord_Id;
uniform sampler2D u_Emit;
uniform sampler2D u_Diffuse_Alpha;
uniform sampler2D u_Specular_Specularity;

uniform mat4 u_mViewInv;
uniform mat4 u_mView;
uniform mat3 u_mViewNorm;

in vec2 v_texcoord;

layout (location = 0) out vec4 f_fragColor;


void main()
{
    vec3 camCoord = texture(u_CamCoord_Id, v_texcoord).rgb;
    vec3 camNormal = texture(u_Normal_Depth, v_texcoord).rgb;
    vec3 cDiffuse = texture(u_Diffuse_Alpha, v_texcoord).rgb;
    float fAlpha = texture(u_Diffuse_Alpha, v_texcoord).a;
    vec3 cSpecular = texture(u_Specular_Specularity, v_texcoord).rgb;
    float fSpecularity = texture(u_Specular_Specularity, v_texcoord).a;
    vec3 color = texture(u_Emit, v_texcoord).rgb;

    for(int i = 0; i < lightCount; i++)
    {
        //extract light parameters
        int iBase = i * lightComponents;
        float type = u_light[iBase].w;
        float isLightDPS = float(step(LIGHT_DIRECTIONAL, type));
        float isLightPS  = float(step(LIGHT_POINT, type));
        float isLightS   = float(step(LIGHT_SPOT, type));

        vec4 lightPos4 = u_mView * vec4(u_light[iBase].xyz, 1.0);
        vec3 lightPos = lightPos4.xyz / lightPos4.w;
        vec3 lightDir = u_mViewNorm * u_light[iBase+1].xyz;
        vec3 lightColor = u_light[iBase+2].rgb;
        float intensity = u_light[iBase+1].w;
        float linAttenuation = 2.0 / intensity;
        float quadAttenuation = 1.0 / (intensity * intensity);
        float cutoff = u_light[iBase+2].w;
        float cutoffEnd = floor(cutoff) * 3.141593 / 180.0;
        float cutoffBegin = cutoffEnd * (1.0 - fract(cutoff) * 2.0);

        //phong model
        vec3 v = normalize(-camCoord);
        vec3 l = normalize(mix(-lightDir, lightPos - camCoord, isLightPS));
        vec3 n = normalize(camNormal);
        vec3 h = normalize(l + v);
        vec3 d = normalize(-lightDir);

        float dist = length(lightPos - camCoord);
        float fAttenuation = mix(1.0, 1.0 / (1.0 + linAttenuation * dist + quadAttenuation * dist * dist), isLightPS);

        float fSpot = mix(1.0, smoothstep(cos(cutoffEnd), cos(cutoffBegin), clamp(dot(l, d), 0.0, 1.0)), isLightS);
        float fDiffuse = mix(1.0, clamp(dot(l, n), 0.0, 1.0), isLightDPS);
        float fSpecular = mix(0.0, pow(clamp(dot(h, n), 0.0, 1.0), fSpecularity), isLightDPS);

        vec3 diffuse = cDiffuse.rgb * fDiffuse;
        vec3 specular = cSpecular.rgb * fSpecular;
        color += lightColor * (diffuse + specular) * fAttenuation * fSpot;
    }

    //fog calculation
    vec4 worldCoord4 = u_mViewInv * vec4(camCoord, 1.0);
    vec3 worldCoord = worldCoord4.xyz / worldCoord4.w;
    float fFogHeight = smoothstep(25.0, 100.0, worldCoord.y);
    vec3 fogColor = mix(vec3(0.3, 0.33, 0.4), vec3(0.35, 0.4, 0.5), fFogHeight);
    float fDensity = mix(0.02, 0.002, fFogHeight);
    float fFog = 1.0 - exp(-pow(fDensity * (-camCoord.z), 2.0));

    f_fragColor = vec4(mix(color, fogColor, fFog), fAlpha);
}
