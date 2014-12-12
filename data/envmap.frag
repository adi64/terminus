#version 140

uniform sampler2D envmap;
uniform samplerCube cubemap;

uniform int mapping;

out vec4 fragColor;

in vec2 vertPosition;
in vec3 v_eye;

// Task_2_1 - ToDo Begin
// Implement the four requested projection mappings.

const float PI = 3.14159;

vec4 env(in vec3 eye)
{
	vec4 color;
	
	if(0 == mapping) 		// cube
	{
		// use texture function with the cubemap sampler
		color = vec4(texture(cubemap,eye));
	}
	else if(1 == mapping) 	// polar
	{
		// okay so for some reason y and z are switched
		
		// use texture function with the envmap sampler
		float u  = 0.5 *(1.0 / PI) * atan(eye.x, eye.z);
		float vh = 2.0 * (1.0 / PI) * asin(eye.y); //vh wie HALBTEXTUR (wir haben nur den himmel in polartextur)

		// and for some reason we have to subtract the value from 1.0 to get the desired result.
		// cost me only about 4 hours to get that -.-
		u = 1.0 - u;
		vh = 1.0 - vh;
		
		// repeat u (horizontal) - it's a 360° view ;)
		u = fract(u);
		
		vec2 uv = vec2(u, vh);
		
		// fade to gray at the bottom because our texture only contains a picture of the sky
		vec4 bottomcolor = vec4(0.4, 0.4, 0.4, 1.0);
		vec4 skycolor = vec4(texture(envmap,uv));
		float bottomEdge = 0.025;
		float mixRatio = step(bottomEdge, eye.y) + (1-step(bottomEdge, eye.y))*smoothstep(0.0, bottomEdge, eye.y);
		
		color = mix(bottomcolor, skycolor, mixRatio);
		
	}	
	else if(2 == mapping) 	// paraboloid
	{
		float m = 2.0 + 2.0 * eye.y; // z
		float u = 0.5 + eye.x * (1.0 / m);
		float v = 0.5 + eye.z * (1.0 / m); // y
		color = vec4(texture(envmap, vec2(1.0 - u, 1.0 - v)));
	}
	else if(3 == mapping) 	// sphere
	{
		float m = 2.0 * sqrt((eye.x*eye.x) + (eye.y*eye.y) + ((1.0 - eye.z)*(1.0 - eye.z)));
		float u = 0.5 - (eye.x * (1.0 / m));
		float v = 0.5 + (eye.y * (1.0 / m));
		
		color = vec4(texture(envmap,vec2(1.0 - u, 1.0 - v)));
	}
	return color;
}

// Task_2_1 - ToDo End


void main()
{
	vec3 eye = normalize(v_eye);
	vec4 color = env(eye);
	
	//debug: show eye vector
	//color = vec4(eye,0.5);

	fragColor = color;
}
