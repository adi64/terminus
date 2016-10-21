#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 mProjectionInv;
uniform mat4 mView;
uniform sampler2D fbo_texture;

layout (location = 0) in vec3 a_vertex;

out vec3 v_eye;
out vec2 v_texcoord;

void main()
{
    //---world space---
    v_eye = ((mProjectionInv * vec4(a_vertex, 1.0)) * mView).xyz;

    //---screen space---
    gl_Position = vec4(a_vertex.xy, 0.9999, 1.0);

    v_texcoord = (a_vertex.xy + 1.0) / 2.0;
}
  
