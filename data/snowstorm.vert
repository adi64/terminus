uniform mat4 mProjectionInv;
uniform mat4 mView;

attribute vec3 a_vertex;

varying vec3 v_eye;
varying vec3 v_vertex;

void main()
{
    v_vertex = a_vertex * 0.5 + 0.5;

    v_eye = ((mProjectionInv * vec4(a_vertex, 1.0)) * mView).xyz;
    gl_Position = vec4(a_vertex.xy, 0.5, 1.0);
}
