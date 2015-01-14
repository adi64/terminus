precision highp float;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat3 mModelNorm;
uniform mat3 mViewNorm;

uniform vec3 lightDirection;

uniform sampler2D levelMap;
uniform vec4 texInfo;//x - offset x; y - offset z; z - size x; w - size z
uniform vec4 posInfo;//x - triangle width; y - triangle height; z - patch displacement x; w - patch displacement z

attribute vec3 a_position;
attribute vec3 a_texCoord;
attribute vec3 a_normal;

varying vec3 v_normal;// x - encoded texCoord offset 0; y - encoded texCoord offset 1
varying vec3 v_position;
varying vec3 v_light;
//varying vec2 v_tc;

vec2 dec(float offset)
{
    return vec2(floor( mod( offset,     3.0)) - 1,
                 floor( mod( offset/3.0, 3.0)) - 1);
}

vec3 getDisplacement(vec2 coord)
{
    return texture2DLod(levelMap, (coord + texInfo.xy) / texInfo.zw, 0).xyz * 200.0 - 100.0;
}

void main()
{
    //model space
    vec3 dispVertex = getDisplacement(a_texCoord.xy);
    vec3 dispPatch = vec3(posInfo.z, 0.0, posInfo.w);
    vec3 position = a_position + dispVertex + dispPatch;
    vec2 offset0 = dec(a_normal.x),
          offset1 = dec(a_normal.y);
    vec3 dispV0 = getDisplacement(a_texCoord.xy + offset0),
          dispV1 = getDisplacement(a_texCoord.xy + offset1);
    vec3 baseV0 = vec3(posInfo.x * offset0.x, 0.0, posInfo.y * offset0.y),
          baseV1 = vec3(posInfo.x * offset1.x, 0.0, posInfo.y * offset1.y);
    vec3 normal = normalize( cross(
                    baseV0 + dispV0 - dispVertex,
                    baseV1 + dispV1 - dispVertex));
//    vec3 position = vec3(posInfo.z, 0.0, posInfo.w) + a_position;
//    vec3 normal = vec3(0.0, 1.0, 0.0);
//    v_tc = (a_texCoord.xy + texInfo.xy) / texInfo.zw;
    //camera space:
    v_normal = mViewNorm * mModelNorm * normal;
    vec4 positionTmp = mView * mModel * vec4(position, 1.0);
    v_position = positionTmp.xyz / positionTmp.w;
    v_light = mViewNorm * lightDirection;
    //screen space:
    gl_Position = mProjection * mView * mModel * vec4(position, 1.0);
}
