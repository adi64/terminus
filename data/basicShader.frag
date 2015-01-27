precision mediump float;

uniform vec4 fSpecularity;
uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform vec4 fAlpha;

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_light;

float zfar = 1024.0;        //yeah, this shouldn't be hardcoded in the end, but its for Early Access and i don't have time right now
float znear = 0.2;

void main()
{
    vec3 l = normalize(v_light);
    vec3 n = normalize(v_normal);
    vec3 h = normalize(v_light - 2.0 * v_position);

    vec3 emit = cEmit.xyz;

    float fDiffuse = clamp(dot(l, n), 0.0, 1.0);
    vec3 diffuse = cDiffuse.xyz * fDiffuse;

    float fSpecular = clamp(dot(h, n), 0.0, 1.0);
    fSpecular = fDiffuse * pow(fSpecular, fSpecularity.x);
    vec3 specular = cSpecular.xyz * fSpecular;

    vec3 color = emit + diffuse + specular;

    //adjusting mist
    vec4 mistColor = vec4(0.35, 0.35, 0.5, 1.0);
    float depth = gl_FragCoord.z;
    float density = 3.0;
    float linDepth = - znear * depth / (zfar * depth - zfar - znear * depth);       //lineraize depth value
    float mistiness = 1.0 - exp(-pow(density * linDepth, 2.0));

    color = mix(color, mistColor.xyz, mistiness);

    gl_FragColor = vec4(clamp(color, 0.0, 1.0), fAlpha.x);
}
