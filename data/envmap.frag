#ifdef GL_ES
precision mediump float;
#endif

uniform samplerCube cubemap;

varying vec3 v_eye;

void main()
{
    // read fragment color from the environment map
    vec3 eye = normalize(v_eye);
    gl_FragColor = textureCube(cubemap, eye).bgra;
}
