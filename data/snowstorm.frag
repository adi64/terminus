uniform sampler2D snowTexture

varying vec3 v_eye;

void main()
{
    vec3 eye = normalize(v_eye);
    gl_FragColor = vec4(texture(snowTexture, eye), 1.0);
}
