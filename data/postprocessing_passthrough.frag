#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D inputTexture;

in vec2 v_texcoord;

layout (location = 0) out vec4 f_fragColor;

void main(void) {
    f_fragColor = vec4(texture2D(inputTexture, v_texcoord).rgb, 1.0);
}