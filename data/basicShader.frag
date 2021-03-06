#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 fSpecularity;
uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform vec4 fAlpha;

uniform mat3 mViewNorm;
uniform mat4 mView;

const int lightCount = 8;
const int lightComponents = 3;
const float LIGHT_AMBIENT       = 0.0; //! Ambient light, only color is used.
const float LIGHT_DIRECTIONAL   = 1.0; //! Directional light, defined by direction and color.
const float LIGHT_POINT         = 2.0; //! Point light, defined by position, attenuation and color.
const float LIGHT_SPOT          = 3.0; //! Spot light, defined by position, direction, cut-off angle, attenuation and color.
uniform vec4 light[lightCount * lightComponents];
//light format:
//  +0 - position3 type1;
//  +1 - direction3 intensity1;
//  +2 - color3 cutoff1;

varying vec3 v_normalC;
varying vec3 v_positionC;
varying vec3 v_positionW;

void main()
{
    vec3 color = cEmit.rgb;

    for(int i = 0; i < lightCount; i++)
    {
        //extract light parameters
        int iBase = i * lightComponents;
        float type = light[iBase].w;
        float isLightDPS = float(step(LIGHT_DIRECTIONAL, type));
        float isLightPS  = float(step(LIGHT_POINT, type));
        float isLightS   = float(step(LIGHT_SPOT, type));

        vec4 lightPos4 = mView * vec4(light[iBase].xyz, 1.0);
        vec3 lightPos = lightPos4.xyz / lightPos4.w;
        vec3 lightDir = mViewNorm * light[iBase+1].xyz;
        vec3 lightColor = light[iBase+2].rgb;
        float intensity = light[iBase+1].w;
        float linAttenuation = 2.0 / intensity;
        float quadAttenuation = 1.0 / (intensity * intensity);
        float cutoff = light[iBase+2].w;
        float cutoffEnd = floor(cutoff) * 3.141593 / 180.0;
        float cutoffBegin = cutoffEnd * (1.0 - fract(cutoff) * 2.0);

        //phong model
        vec3 v = normalize(-v_positionC);
        vec3 l = normalize(mix(-lightDir, lightPos - v_positionC, isLightPS));
        vec3 n = normalize(v_normalC);
        vec3 h = normalize(l + v);
        vec3 d = normalize(-lightDir);

        float dist = length(lightPos - v_positionC);
        float fAttenuation = mix(1.0, 1.0 / (1.0 + linAttenuation * dist + quadAttenuation * dist * dist), isLightPS);

        float fSpot = mix(1.0, smoothstep(cos(cutoffEnd), cos(cutoffBegin), clamp(dot(l, d), 0.0, 1.0)), isLightS);
        float fDiffuse = mix(1.0, clamp(dot(l, n), 0.0, 1.0), isLightDPS);
        float fSpecular = mix(0.0, pow(clamp(dot(h, n), 0.0, 1.0), fSpecularity.r), isLightDPS);

        vec3 diffuse = cDiffuse.rgb * fDiffuse;
        vec3 specular = cSpecular.rgb * fSpecular;
        color += lightColor * (diffuse + specular) * fAttenuation * fSpot;
    }

    //fog calculation
    float fFogHeight = smoothstep(25.0, 100.0, v_positionW.y);
    vec3 fogColor = mix(vec3(0.3, 0.33, 0.4), vec3(0.35, 0.4, 0.5), fFogHeight);
    float fDensity = mix(0.02, 0.002, fFogHeight);
    float fFog = 1.0 - exp(-pow(fDensity * (-v_positionC.z), 2.0));

    gl_FragColor = vec4(mix(color, fogColor, fFog), fAlpha.r);
}
