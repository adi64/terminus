precision highp float;

uniform vec4 fSpecularitySnow;
uniform vec4 cEmitSnow;
uniform vec4 cDiffuseSnow;
uniform vec4 cSpecularSnow;
uniform vec4 fSpecularityRock;
uniform vec4 cEmitRock;
uniform vec4 cDiffuseRock;
uniform vec4 cSpecularRock;
uniform vec4 fAlpha;

uniform mat3 mViewNorm;
uniform mat4 mView;

varying vec3 v_normal;
varying vec3 v_position;
varying float v_color;
varying float v_shade;

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;       //! contains the constant, linear and quadratic attenuation factors
    float spotCutOff;       //! cosine of cut-off angle of spotlight
    int type;
};

const int LIGHT_AMBIENT       = 0; //! Ambient light, only color is used.
const int LIGHT_DIRECTIONAL   = 1; //! Directional light, defined by direction and color.
const int LIGHT_POINT         = 2; //! Point light, defined by position, attenuation and color.
const int LIGHT_SPOT          = 3; //! Spot light, defined by position, direction, cut-off angle, attenuation and color.

const int maxLights = 8;

uniform Light lights[maxLights];

void main()
{    
    //modify material colors for more variety among triangles
    vec3 cEmit = mix(cEmitRock.rgb, cEmitSnow.rgb, v_color) * mix(0.8, 1.0, v_shade);
    vec3 cDiffuse = mix(cDiffuseRock.rgb, cDiffuseSnow.rgb, v_color) * mix(0.9, 1.0, v_shade);
    vec3 cSpecular = mix(cSpecularRock.rgb, cSpecularSnow.rgb, v_color);
    float fSpecularity = mix(fSpecularityRock.r, fSpecularitySnow.r, v_color);

    vec3 color = cEmit.rgb;

    for(int i = 0; i < maxLights; i++)
    {
        int type = lights[i].type;
        float isLightDPS = float(step(LIGHT_DIRECTIONAL, type));
        float isLightPS  = float(step(LIGHT_POINT, type));
        float isLightS   = float(step(LIGHT_SPOT, type));

        //phong model
        vec3 lightDir = mViewNorm * lights[i].direction;
        vec4 lightPos4 = mView * vec4(lights[i].position, 1.0);
        vec3 lightPos = lightPos4.xyz / lightPos4.w;
        vec3 lightCol = lights[i].color;
        vec3 lightAtt = lights[i].attenuation;
        float lightCut = lights[i].spotCutOff;

        vec3 v = normalize(-v_position);
        vec3 l = normalize(mix(lightDir, lightPos - v_position, isLightPS));
        vec3 n = normalize(v_normal);
        vec3 h = normalize(l + v);
        vec3 d = normalize(lightDir);

        float dist = length(lightPos - v_position);
        float fAttenuation = mix(1.0, 1.0 / (lightAtt.x + lightAtt.y * dist + lightAtt.z * dist * dist), isLightPS);
        float fSpot = mix(1.0, smoothstep(lightCut * 0.25, lightCut, dot(d, l)), isLightS);
        float fDiffuse = mix(1.0, clamp(dot(l, n), 0.0, 1.0), isLightDPS);
        float fSpecular = mix(0.0, pow(clamp(dot(h, n), 0.0, 1.0), fSpecularity), isLightDPS);

        vec3 diffuse = cDiffuse.rgb * fDiffuse;
        vec3 specular = cSpecular.rgb * fSpecular;
        color += lightCol * (diffuse + specular) * fAttenuation * fSpot;
    }

    //use material specific transparency
    gl_FragColor = vec4(color, fAlpha.r);
}
