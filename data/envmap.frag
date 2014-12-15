#version 130

uniform samplerCube cubemap;

in vec3 v_eye;

void main()
{
	vec3 eye = normalize(v_eye);
        vec4 color = vec4(texture(cubemap, eye).xyz, 1.0);

        gl_FragColor = color;
}
