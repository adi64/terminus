uniform mat4 mvp;
attribute vec4 a_vertex;
attribute vec4 a_color;
varying vec4 v_color;

void main() {
    v_color = a_color;
    gl_Position = mvp * a_vertex;
}
