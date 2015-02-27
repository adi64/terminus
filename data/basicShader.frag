precision highp float;

uniform vec4 fSpecularity;
uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform vec4 fAlpha;

uniform mat3 mViewNorm;
uniform mat4 mView;

varying vec3 v_normal;
varying vec3 v_position;

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;       //! contains the constant (x), linear(y) and quadratic (z) attenuation factors
    float spotCutOff;       //! cosine of cut-off angle of spotlight
    int type;
};

const float LIGHT_AMBIENT       = 0.0; //! Ambient light, only color is used.
const float LIGHT_DIRECTIONAL   = 1.0; //! Directional light, defined by direction and color.
const float LIGHT_POINT         = 2.0; //! Point light, defined by position, attenuation and color.
const float LIGHT_SPOT          = 3.0; //! Spot light, defined by position, direction, cut-off angle, attenuation and color.

const int maxLights = 8;

uniform Light lights[maxLights];

void main()
{
    vec3 color = cEmit.rgb;

    for(int i = 0; i < maxLights; i++)
    {
        float type = float(lights[i].type);
        float isLightDPS = step(LIGHT_DIRECTIONAL, type);
        float isLightPS  = step(LIGHT_POINT, type);
        float isLightS   = step(LIGHT_SPOT, type);

        //phong model
        vec3 lightDir = mViewNorm * lights[i].direction;
        vec4 lightPos4 = mView * vec4(lights[i].position, 1.0);
        vec3 lightPos = lightPos4.xyz / lightPos4.w;
        vec3 lightCol = lights[i].color;
        vec3 lightAtt = lights[i].attenuation;
        float lightCut = lights[i].spotCutOff;

        vec3 v = normalize(-v_position);
        vec3 l = normalize(mix(lightDir, lightPos, isLightPS));
        vec3 n = normalize(v_normal);
        vec3 h = normalize(l + v);

        float dist = length(lightPos);
        float fAttenuation = mix(1.0, 1.0 / (lightAtt.x + lightAtt.y * dist + lightAtt.z * dist * dist), isLightPS);
        float fSpot = mix(1.0, step(lightCut, dot(v, l)), isLightS);
        float fDiffuse = mix(1.0, clamp(dot(l, n), 0.0, 1.0), isLightDPS) * fAttenuation * fSpot;
        float fSpecular = mix(0.0, pow(clamp(dot(h, n), 0.0, 1.0), fSpecularity.r), isLightDPS) * fSpot;

        vec3 diffuse = cDiffuse.rgb * fDiffuse;
        vec3 specular = cSpecular.rgb * fSpecular;
        color += lights[i].color * (diffuse + specular);
    }

    gl_FragColor = vec4(clamp(color, 0.0, 1.0), fAlpha.r);
}
