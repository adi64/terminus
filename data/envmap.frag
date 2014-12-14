#version 130

uniform sampler2D envmap;
uniform samplerCube cubemap;

uniform int mapping;

out vec4 fragColor;

in vec2 vertPosition;
in vec3 v_eye;

const float PI = 3.14159;

void main()
{
	vec3 eye = normalize(v_eye);
        vec4 color = vec4(texture(cubemap,eye).xyz, 1.0);
	
	//debug: show eye vector
	//color = vec4(eye,0.5);

        //fragColor = color;
        fragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
