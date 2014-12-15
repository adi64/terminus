#version 130

uniform mat4 mProjectionInv;
uniform mat4 mView;

in vec3 a_vertex;

out vec3 v_eye;

void main()
{	
	// ToDo: use the given xy coordinates to retrieve 
	// the vertices eye vector (basically the view frustums edges
	// pointing towards +z) that need to be passed to
	// the fragment stage for accessing the various projection
	// mappings.

        v_eye = ((mProjectionInv * vec4(a_vertex, 1.0)) * mView).xyz;

	// Note: z is already at z = 1.0 (back plane in NDC)
	
        gl_Position = vec4(a_vertex.xy, 0.9999, 1.0);
}
  
