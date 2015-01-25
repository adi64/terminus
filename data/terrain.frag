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

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_light;
varying float v_color;
varying float v_shade;

float zfar = 1024.0;        //yeah, this shouldn't be hardcoded in the end, but its for Early Access and i don't have time right now
float znear = 0.2;

void main()
{
    //modify material colors for more variety among triangles
    vec3 cAmbient = mix(cAmbientRock.rgb, cAmbientSnow.rgb, v_color) * mix(0.8, 1.0, v_shade);
    vec3 cDiffuse = mix(cDiffuseRock.rgb, cDiffuseSnow.rgb, v_color) * mix(0.9, 1.0, v_shade);
    vec3 cSpecular = mix(cSpecularRock.rgb, cSpecularSnow.rgb, v_color);
    float fSpecularity = mix(fSpecularityRock.r, fSpecularitySnow.r, v_color);

    //phong model
    vec3 l = normalize(v_light);
    vec3 n = normalize(v_normal);
    vec3 h = normalize(v_light - 2.0 * v_position);

    float fDiffuse = clamp(dot(l, n), 0.0, 1.0);
    vec3 diffuse = cDiffuse * fDiffuse;

    float fSpecular = clamp(dot(h, n), 0.0, 1.0);
    fSpecular = fDiffuse * pow(fSpecular, fSpecularity);
    vec3 specular = cSpecular * fSpecular;

    vec3 color = cAmbient + diffuse + specular;

    //adjusting mist
    vec4 mistColor = vec4(0.35, 0.35, 0.5, 1.0);
    float depth = gl_FragCoord.z;
    float density = 3.0;
    float linDepth = - znear * depth / (zfar * depth - zfar - znear * depth);       //lineraize depth value
    float mistiness = 1 - exp(-pow(density * linDepth, 2));

    color = mix(color, mistColor, mistiness);

    //use material specific transparency
    gl_FragColor = vec4(color, fAlpha.r);
}
