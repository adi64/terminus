#ifdef GL_ES
precision mediump float;
#endif

varying vec3 v_eye;

uniform sampler2D fbo_texture;

uniform float screenWidth;
uniform float screenHeight;

varying vec2 f_texcoord;

//RADIUS of our vignette, where 0.5 results in a circle fitting the screen
const float RADIUS = 0.75;

//softness of our vignette, between 0.0 and 1.0
const float SOFTNESS = 0.45;

void main(void) {

    vec4 texColor = texture2D(fbo_texture, f_texcoord);

    //determine center position
    vec2 position = vec2(float(gl_FragCoord.x) / screenWidth, float(gl_FragCoord.y) / screenHeight) - vec2(0.5);

    //determine the vector length of the center position
    float len = length(position);

    //use smoothstep to create a smooth vignette
    float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);

    //apply the vignette with 50% opacity
    texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.9);
    gl_FragColor = texColor;
}
