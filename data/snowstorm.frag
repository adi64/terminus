precision highp float;

uniform sampler2D snowTexture;

varying vec3 v_eye;
varying vec3 v_vertex;

void main()
{
    vec3 eye = normalize(v_eye);
    vec4 snow = texture(snowTexture, v_vertex.xy);
    if(snow.a > 0.7)
    {
        snow.a = smoothstep(0.0, 1.0, snow.a);
    }
    snow.a = pow(snow.a, 8.0);
    snow.a = 0.0;

    gl_FragColor = vec4(1.0 - snow.rgb, snow.a);
}
