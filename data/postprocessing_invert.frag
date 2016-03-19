#ifdef GL_ES
precision mediump float;
#endif

varying vec3 v_eye;

uniform sampler2D fbo_texture;

varying vec2 f_texcoord;

void main(void) {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) - texture2D(fbo_texture, f_texcoord);
}
