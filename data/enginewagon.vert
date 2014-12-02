uniform mat4 mvp;
in vec3 a_vertex;

void main()
{
    gl_Position = mvp * vec4(a_vertex, 1.0);
}
