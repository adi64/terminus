#version 130

uniform mat4 mProjectionInv;
uniform mat4 mView;

in vec3 a_vertex;

out vec3 v_eye;

void main()
{
        v_eye = ((mProjectionInv * vec4(a_vertex, 1.0)) * mView).xyz;
        gl_Position = vec4(a_vertex.xy, 0.9999, 1.0);
}
  
