precision mediump float;

uniform samplerCube cubemap;

varying vec3 v_eye;

void main()
{
	vec3 eye = normalize(v_eye);
        vec4 color = textureCube(cubemap, eye);
        gl_FragColor = color;
}
