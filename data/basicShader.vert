uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat3 mModelNorm;
uniform mat3 mViewNorm;

uniform vec3 lightDirection;

attribute vec3 a_position;
attribute vec3 a_texCoord;
attribute vec3 a_normal;

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_light;

void main()
{
    //camera space:
    v_normal = mViewNorm * mModelNorm * a_normal;
    vec4 position = mView * mModel * vec4(a_position, 1.0);
    v_position = position.xyz / position.w;
    v_light = mViewNorm * lightDirection;
    //screen space:
    gl_Position = mProjection * mView * mModel * vec4(a_position, 1.0);
}
