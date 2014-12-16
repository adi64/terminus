precision mediump float;

uniform samplerCube cubemap;

varying vec3 v_eye;

void main()
{
	vec3 eye = normalize(v_eye);
        vec4 color = vec4(textureCube(cubemap, eye).xyz, 1.0);
        gl_FragColor = color;
}
