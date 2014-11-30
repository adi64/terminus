uniform mat4 mvp;
attribute vec4 a_vertex;

void main() {
    gl_Position = mvp * a_vertex;
}
