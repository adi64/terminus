#version 130

uniform mat4 cameraProjectionInverted;
uniform mat4 cameraView;

in vec2 a_vertex;

out vec3 v_eye;
out vec2 vertPosition;

void main()
{	
	// ToDo: use the given xy coordinates to retrieve 
	// the vertices eye vector (basically the view frustums edges
	// pointing towards +z) that need to be passed to
	// the fragment stage for accessing the various projection
	// mappings.

	v_eye = ((cameraProjectionInverted * vec4(a_vertex, 1.0, 1.0)) * cameraView).xyz;
	

	// Task_2_1 - ToDo End
	
	// Note: z is already at z = 1.0 (back plane in NDC)
	
	gl_Position = vec4(a_vertex, 0.99, 1.0);
	vertPosition = a_vertex;
}
  
