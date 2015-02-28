precision highp float;

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
//LightFormat:
////position3 type1;
////direction3 intensity1;
////color3 cutoff1;

varying vec3 v_normal;
varying vec3 v_position;

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
        vec3 v = normalize(-v_position);
        vec3 l = normalize(mix(lightDir, lightPos - v_position, isLightPS));
        vec3 n = normalize(v_normal);
        vec3 h = normalize(l + v);
        vec3 d = normalize(lightDir);

        float dist = length(lightPos - v_position);
        float fAttenuation = mix(1.0, 1.0 / (1.0 + linAttenuation * dist + quadAttenuation * dist * dist), isLightPS);
        float fSpot = mix(1.0, smoothstep(cutoffBegin, cutoffEnd, dot(d, l)), isLightS);
        float fDiffuse = mix(1.0, clamp(dot(l, n), 0.0, 1.0), isLightDPS);
        float fSpecular = mix(0.0, pow(clamp(dot(h, n), 0.0, 1.0), fSpecularity.r), isLightDPS);

        vec3 diffuse = cDiffuse.rgb * fDiffuse;
        vec3 specular = cSpecular.rgb * fSpecular;
        color += lightColor * (diffuse + specular) * fAttenuation * fSpot;
    }

    gl_FragColor = vec4(clamp(color, 0.0, 1.0), fAlpha.r);
}
