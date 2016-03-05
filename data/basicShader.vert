#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat3 mModelNorm;
uniform mat3 mViewNorm;

uniform vec3 lightDirection;

attribute vec3 a_position;
attribute vec3 a_texCoord;
attribute vec3 a_normal;

varying vec3 v_normalC;
varying vec3 v_positionC;
varying vec3 v_positionW;

void main()
{
    //---model space---
    vec4 position4 = mModel * vec4(a_position, 1.0);
    v_positionW = position4.xyz / position4.w;

    //---camera space---
    v_normalC = mViewNorm * mModelNorm * a_normal;
    position4 = mView * mModel * vec4(a_position, 1.0);
    v_positionC = position4.xyz / position4.w;

    //---screen space---
    gl_Position = mProjection * mView * mModel * vec4(a_position, 1.0);
}
