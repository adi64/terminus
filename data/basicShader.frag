precision mediump float;

uniform vec4 fSpecularity;
uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform vec4 fAlpha;

uniform mat3 mViewNorm;

varying vec3 v_normal;
varying vec3 v_position;

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;       //! contains the constant, linear and quadratic attenuation factors
    float spotCutOff;       //! cosine of cut-off angle of spotlight
    int type;
};

const int maxLights = 8;

uniform Light lights[maxLights];

void main()
{

    vec3 emit = cEmit.rgb;

    vec3 color = emit;

    //phong model
    for(int i=0; i<maxLights; i++)
    {
        // don't include invalid lights
        if(lights[i].type == 0)
        {
            continue;
        }

        vec3 v_light = mViewNorm * lights[i].direction;

        vec3 l = normalize(v_light);
        vec3 n = normalize(v_normal);
        vec3 h = normalize(v_light - 2.0 * v_position);

        float fDiffuse = clamp(dot(l, n), 0.0, 1.0);
        vec3 diffuse = cDiffuse * fDiffuse;

        float fSpecular = clamp(dot(h, n), 0.0, 1.0);
        fSpecular = fDiffuse * pow(fSpecular, fSpecularity);
        vec3 specular = cSpecular * fSpecular;

        color += lights[i].color * diffuse + lights[i].color * specular;
    }

    gl_FragColor = vec4(clamp(color, 0.0, 1.0), fAlpha.r);
}
