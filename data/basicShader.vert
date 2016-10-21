#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 u_mModel;
uniform mat4 u_mView;
uniform mat4 u_mProjection;
uniform mat3 u_mModelNorm;
uniform mat3 u_mViewNorm;

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

    v_normalCamSpace = u_mViewNorm * u_mModelNorm * a_normal;
    vec4 position4 = u_mView * u_mModel * vec4(a_position, 1.0);
    v_positionCamSpace = position4.xyz / position4.w;

    gl_Position = u_mProjection * u_mView * u_mModel * vec4(a_position, 1.0);
}
