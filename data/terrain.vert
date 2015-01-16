precision highp float;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat3 mModelNorm;
uniform mat3 mViewNorm;

uniform vec3 lightDirection;

uniform sampler2D levelMap;
uniform vec4 texInfo; //x - offset x; y - offset z; z - size x; w - size z
uniform vec4 posInfo; //x - triangle width; y - triangle height; z - patch displacement x; w - patch displacement z

attribute vec3 a_position;
attribute vec3 a_texCoord;
attribute vec3 a_normal; // x - encoded offset to adjacent vertex 0; y - encoded offset to adjacent vertex 0

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_light;
varying float v_shade;

//decode packet adjacent-vertex-offset
vec2 dec(float offset)
{
    return vec2(floor(mod(offset, 3.0)) - 1.0, floor(mod(offset/3.0, 3.0)) - 1.0);
}

//extract displacement vector from the terrain map
vec3 getDisplacement(vec2 coord)
{
    vec3 raw = texture2DLod(levelMap, (coord + texInfo.xy) / texInfo.zw, 0.0).xyz;
    return vec3(raw.x - 0.5, raw.y * 200.0, raw.z - 0.5);
}

void main()
{
    //---model space---
    //offset position attribute with current patch position and displacement from the terrain map
    vec3 dispVertex = getDisplacement(a_texCoord.xy);
    vec3 dispPatch = vec3(posInfo.z, 0.0, posInfo.w);
    vec3 position = a_position + dispVertex + dispPatch;
    //calculate vectors to the other triangle vertices and calculate the face normal
    vec2 offset0 = dec(a_normal.x),
          offset1 = dec(a_normal.y);
    vec3 dispV0 = getDisplacement(a_texCoord.xy + offset0),
          dispV1 = getDisplacement(a_texCoord.xy + offset1);
    vec3 baseV0 = vec3((offset0.x + offset0.y * a_normal.z * 0.5) * posInfo.x, 0.0, offset0.y * posInfo.y),
          baseV1 = vec3((offset1.x + offset1.y * a_normal.z * 0.5) * posInfo.x, 0.0, offset1.y * posInfo.y);
    vec3 normal = normalize( cross(
                    baseV0 + dispV0 - dispVertex,
                    baseV1 + dispV1 - dispVertex));
    //use pseudo pseudorandom function on normal to provide a face specific factor to fragment shader
    v_shade = mod(normal.z * 10000.0 , 1.0);

    //---camera space---
    //transform the relevant vectors for phong model in fragment shader
    v_normal = mViewNorm * mModelNorm * normal;
    vec4 positionTmp = mView * mModel * vec4(position, 1.0);
    v_position = positionTmp.xyz / positionTmp.w;
    v_light = mViewNorm * lightDirection;

    //---screen space---
    gl_Position = mProjection * mView * mModel * vec4(position, 1.0);
}
