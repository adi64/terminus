#version 130

uniform samplerCube cubemap;

out vec4 fragColor;

in vec2 vertPosition;
in vec3 v_eye;

void main()
{
	vec3 eye = normalize(v_eye);
        vec4 color = vec4(texture(cubemap,eye).xyz, 1.0);

        fragColor = color;
}
