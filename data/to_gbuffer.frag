#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform float objectId;

in vec3 v_normalCamSpace;
in vec3 v_positionCamSpace;
in vec3 v_emitColor;
in vec3 v_diffuseColor;
in float v_alpha;
in vec3 v_specularColor;
in float v_specularity;

layout (location = 0) out vec4 o_Normal_Depth;
layout (location = 1) out vec4 o_CamCoord_Id;
layout (location = 2) out vec4 o_Emit;
layout (location = 3) out vec4 o_Diffuse_Alpha;
layout (location = 4) out vec4 o_Specular_Specularity;

void main()
{
    o_Normal_Depth = vec4(v_normalCamSpace, gl_FragCoord.z);
    o_CamCoord_Id = vec4(v_positionCamSpace, objectId);
    o_Emit = vec4(v_emitColor, 0.0);
    o_Diffuse_Alpha = vec4(v_diffuseColor, v_alpha);
    o_Specular_Specularity = vec4(v_specularColor, v_specularity);

    gl_FragDepth = gl_FragCoord.z;
}
