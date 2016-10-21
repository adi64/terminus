#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 u_mProjectionInv;
uniform mat4 u_mView;

layout (location = 0) in vec3 a_position;

out vec3 v_eye;

void main()
{
    //---world space---
    v_eye = ((u_mProjectionInv * vec4(a_vertex, 1.0)) * u_mView).xyz;

    //---screen space---
    gl_Position = vec4(a_vertex.xy, 0.9999, 1.0);
}
  
