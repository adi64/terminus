precision highp float;

uniform vec4 fSpecularitySnow;
uniform vec4 cAmbientSnow;
uniform vec4 cDiffuseSnow;
uniform vec4 cSpecularSnow;
uniform vec4 fSpecularityRock;
uniform vec4 cAmbientRock;
uniform vec4 cDiffuseRock;
uniform vec4 cSpecularRock;
uniform vec4 fAlpha;

uniform mat3 mViewNorm;

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

const int maxLights = 8;

uniform Light lights[maxLights];

void main()
{    
    //modify material colors for more variety among triangles
    vec3 cAmbient = mix(cAmbientRock.rgb, cAmbientSnow.rgb, v_color) * mix(0.8, 1.0, v_shade);
    vec3 cDiffuse = mix(cDiffuseRock.rgb, cDiffuseSnow.rgb, v_color) * mix(0.9, 1.0, v_shade);
    vec3 cSpecular = mix(cSpecularRock.rgb, cSpecularSnow.rgb, v_color);
    float fSpecularity = mix(fSpecularityRock.r, fSpecularitySnow.r, v_color);

    vec3 color = cAmbient;

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

    //use material specific transparency
    gl_FragColor = vec4(color, fAlpha.r);
}
