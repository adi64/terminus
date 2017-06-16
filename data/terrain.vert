#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 u_mModel;
uniform mat4 u_mView;
uniform mat4 u_mProjection;
uniform mat3 u_mModelNorm;
uniform mat3 u_mViewNorm;

uniform vec4 fSpecularitySnow;
uniform vec4 cEmitSnow;
uniform vec4 cDiffuseSnow;
uniform vec4 cSpecularSnow;
uniform vec4 fSpecularityRock;
uniform vec4 cEmitRock;
uniform vec4 cDiffuseRock;
uniform vec4 cSpecularRock;
uniform vec4 fAlpha;

uniform sampler2D levelMap;
uniform vec4 texInfo; //x - offset x; y - offset z; z - size x; w - size z
uniform vec4 posInfo; //x - triangle width; y - triangle height; z - patch displacement x; w - patch displacement z

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_texCoord;
layout (location = 2) in vec3 a_normal; // x - encoded offset to adjacent vertex 0; y - encoded offset to adjacent vertex 0

out vec3 v_normalCamSpace;
out vec3 v_positionCamSpace;
out vec3 v_emitColor;
out vec3 v_diffuseColor;
out float v_alpha;
out vec3 v_specularColor;
out float v_specularity;

//decode packed adjacent-vertex-offset
vec2 decodeOffset(float offset)
{
    return vec2(floor(mod(offset, 3.0)) - 1.0, floor(mod(offset/3.0, 3.0)) - 1.0);
}

//read displacement vector from the terrain map
vec3 displacement(vec2 coord)
{
    vec3 raw = textureLod(levelMap, (coord + texInfo.xy) / texInfo.zw, 0.0).xyz;
    return vec3(raw.x - 0.5, raw.y * 200.0, raw.z - 0.5);
}

//calculate model space vector corresponding to texture coordinate offsets
vec3 texOffset2Vector(vec2 texOffset, float isOddRow)
{
    return vec3((texOffset.x + texOffset.y * isOddRow * 0.5) * posInfo.x, 0.0, texOffset.y * posInfo.y);
}

void main()
{
    //---model space---
    //extract parameters
    vec3 dPatch = vec3(posInfo.z, 0.0, posInfo.w);
    vec2 texOffset1 = decodeOffset(a_normal.x),
          texOffset2 = decodeOffset(a_normal.y);
    float isOddRow = a_normal.z;
    //offset position attribute with current patch position and displacement from the terrain map
    vec3 dVertex = displacement(a_texCoord.xy);
    vec3 position = a_position + dVertex + dPatch;
    //calculate vectors to the other triangle vertices and the face normal
    vec3 dVertex1 = texOffset2Vector(texOffset1, isOddRow) + displacement(a_texCoord.xy + texOffset1),
          dVertex2 = texOffset2Vector(texOffset2, isOddRow) + displacement(a_texCoord.xy + texOffset2);
    vec3 normal = normalize(cross(dVertex1 - dVertex, dVertex2 - dVertex));

    //use pseudo pseudorandom function on normal to provide a face specific factor to fragment shader
    float shade = mod(normal.y * 10000.0 , 1.0);
    //steepness dependent factor to distinguish snowy and rocky triangles
    float rockyness = smoothstep(0.20, 0.35, abs(normal.y));

    v_emitColor = mix(cEmitRock.rgb, cEmitSnow.rgb, rockyness) * mix(0.8, 1.0, shade);
    v_diffuseColor = mix(cDiffuseRock.rgb, cDiffuseSnow.rgb, rockyness) * mix(0.9, 1.0, shade);
    v_specularColor = mix(cSpecularRock.rgb, cSpecularSnow.rgb, rockyness);
    v_specularity = mix(fSpecularityRock.r, fSpecularitySnow.r, rockyness);
    v_alpha = fAlpha.r;

    //---camera space---
    //transform the relevant vectors for phong model in fragment shader
    v_normalCamSpace = u_mViewNorm * u_mModelNorm * normal;
    vec4 position4 = u_mView * u_mModel * vec4(position, 1.0);
    v_positionCamSpace = position4.xyz / position4.w;

    //---screen space---
    gl_Position = u_mProjection * u_mView * u_mModel * vec4(position, 1.0);
}
