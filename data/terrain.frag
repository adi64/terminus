precision highp float;

uniform vec4 fSpecularity;
uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform vec4 fAlpha;

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_light;
varying float v_color;
varying float v_shade;

void main()
{
    //modify material colors for more variety among triangles
    vec3 emit = cEmit.rgb * v_color;//* mix(0.8, 1.0, v_shade);
    vec3 diffuse = cDiffuse.rgb * v_color;//mix(0.9, 1.0, v_shade);
    vec3 specular = cSpecular.rgb;

    //phong model
    vec3 l = normalize(v_light);
    vec3 n = normalize(v_normal);
    vec3 h = normalize(v_light - 2.0 * v_position);

    float fDiffuse = clamp(dot(l, n), 0.0, 1.0);
    diffuse = diffuse * fDiffuse;

    float fSpecular = clamp(dot(h, n), 0.0, 1.0);
    fSpecular = fDiffuse * pow(fSpecular, fSpecularity.r);
    specular = specular * fSpecular;

    vec3 color = emit + diffuse + specular;

    //use material specific transparency
    gl_FragColor = vec4(color, fAlpha.r);
}
