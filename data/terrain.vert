precision mediump float;

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

varying vec3 v_normalC;
varying vec3 v_positionC;
varying vec3 v_positionW;
varying float v_color;
varying float v_shade;


//decode packed adjacent-vertex-offset
vec2 decodeOffset(float offset)
{
    return vec2(floor(mod(offset, 3.0)) - 1.0, floor(mod(offset/3.0, 3.0)) - 1.0);
}

//read displacement vector from the terrain map
vec3 displacement(vec2 coord)
{
    vec3 raw = texture2DLod(levelMap, (coord + texInfo.xy) / texInfo.zw, 0.0).xyz;
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
    v_shade = mod(normal.y * 10000.0 , 1.0);
    //steepness dependent factor to distinguish snowy and rocky triangles
    v_color = smoothstep(0.20, 0.35, abs(normal.y));

    //---world space---
    vec4 positionW4 = mModel * vec4(position, 1.0);
    v_positionW = positionW4.xyz / positionW4.w;

    //---camera space---
    //transform the relevant vectors for phong model in fragment shader
    v_normalC = mViewNorm * mModelNorm * normal;
    vec4 positionC4 = mView * mModel * vec4(position, 1.0);
    v_positionC = positionC4.xyz / positionC4.w;

    //---screen space---
    gl_Position = mProjection * mView * mModel * vec4(position, 1.0);
}
