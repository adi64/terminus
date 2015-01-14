precision highp float;

uniform sampler2D levelMap;

uniform vec4 fSpecularity;
uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform vec4 fAlpha;

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_light;
varying float v_shade;
//varying vec2 v_tc;

void main()
{
    vec3 l = normalize(v_light);
    vec3 n = normalize(v_normal);
    vec3 h = normalize(v_light - 2.0 * v_position);

    vec3 emit = cEmit.rgb * (0.7 + 0.3 * v_shade);

    float fDiffuse = clamp(dot(l, n), 0.0, 1.0) * (0.9 + 0.1 * v_shade);
    vec3 diffuse = cDiffuse.rgb * fDiffuse;

    float fSpecular = clamp(dot(h, n), 0.0, 1.0);
    fSpecular = fDiffuse * pow(fSpecular, fSpecularity.r);
    vec3 specular = cSpecular.rgb * fSpecular;

    vec3 color = emit + diffuse + specular;

    gl_FragColor = vec4(color, fAlpha.r);
    //gl_FragColor = vec4(texture2D(levelMap, v_tc).rgb, fAlpha.r);
}
