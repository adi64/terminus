#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D inputTexture;
uniform sampler2D u_depthTexture;

uniform mat4 u_mViewProjectionInverted;
uniform mat4 u_mPreviousViewProjection;

uniform int u_motionBlurFactor;

in vec2 v_texcoord;

layout (location = 0) out vec4 f_fragColor;

void main(void) {
    float depthValue = texture2D(u_depthTexture, v_texcoord).a;

    vec4 viewportPosition = vec4(
                v_texcoord.x * 2.0 - 1.0,
                (1.0 - v_texcoord.y) * 2.0 - 1.0,
                depthValue,
                1.0);

    vec4 D = viewportPosition * u_mViewProjectionInverted;
    vec4 worldPos = D / D.w;
    vec4 currentPos = viewportPosition;
    vec4 previousPos = worldPos * u_mPreviousViewProjection;
    previousPos /= previousPos.w;

    vec2 velocity = (currentPos - previousPos).xy / 2.0;

    vec4 finalColor = texture2D(inputTexture, v_texcoord);

    vec2 shiftedTexCoord = v_texcoord + velocity;

    for (int i=1; i<u_motionBlurFactor; ++i)
    {
        vec4 currentColor = texture2D(inputTexture, shiftedTexCoord);
        finalColor += currentColor;
        shiftedTexCoord += velocity;
    }

    finalColor = finalColor / float(u_motionBlurFactor);

    f_fragColor = finalColor;
}
