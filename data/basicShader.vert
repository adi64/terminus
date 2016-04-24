#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat3 mModelNorm;
uniform mat3 mViewNorm;

uniform vec4 cEmit;
uniform vec4 cDiffuse;
uniform vec4 fAlpha;
uniform vec4 cSpecular;
uniform vec4 fSpecularity;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_texCoord;
layout (location = 2) in vec3 a_normal;

out vec3 v_normalCamSpace;
out vec3 v_positionCamSpace;
out vec3 v_emitColor;
out vec3 v_diffuseColor;
out float v_alpha;
out vec3 v_specularColor;
out float v_specularity;

void main()
{
    v_emitColor = cEmit.rgb;
    v_diffuseColor = cDiffuse.rgb;
    v_alpha = fAlpha.r;
    v_specularColor = cSpecular.rgb;
    v_specularity = fSpecularity.r;

    v_normalCamSpace = mViewNorm * mModelNorm * a_normal;
    vec4 position4 = mView * mModel * vec4(a_position, 1.0);
    v_positionCamSpace = position4.xyz / position4.w;

    gl_Position = mProjection * mView * mModel * vec4(a_position, 1.0);
}
