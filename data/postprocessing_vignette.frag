#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D inputTexture;

in vec2 v_texcoord;

layout (location = 0) out vec4 f_fragColor;

uniform float screenWidth;
uniform float screenHeight;

//RADIUS of our vignette, where 0.5 results in a circle fitting the screen
const float RADIUS = 0.75;

//softness of our vignette, between 0.0 and 1.0
const float SOFTNESS = 0.45;

void main(void) {

    vec4 texColor = texture(inputTexture, v_texcoord);

    //determine center position
    vec2 position = vec2(float(gl_FragCoord.x) / screenWidth, float(gl_FragCoord.y) / screenHeight) - vec2(0.5);

    //determine the vector length of the center position
    float len = length(position);

    //use smoothstep to create a smooth vignette
    float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);

    //apply the vignette with 50% opacity
    texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.9);
    f_fragColor = texColor;
}
