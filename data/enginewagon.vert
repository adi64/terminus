#version 130

uniform mat4 mvp;

in vec3 a_vertex;
in vec3 a_normal;

out vec3 v_normal;

void main()
{
    v_normal = a_normal;
    gl_Position = mvp * vec4(a_vertex, 1.0);
}
