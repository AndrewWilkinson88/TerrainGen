#version 150

in mat4 model[];
in mat4 view[];
in mat4 persp[];

in vec4 normal[];

//Light
uniform vec4 lightPosition;

out vec3 L;
out vec3 E;
out vec3 H;
out vec3 N;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() 
{
	int i;
	for(i = 0; i < gl_in.length(); i++)
	{		
		//Calculate Lighting
		vec3 pos = (model[i] * gl_in[i].gl_Position).xyz;
		L = normalize( lightPosition.xyz - pos );
		E = normalize( -pos );
		H = normalize( L + E );
		N = normalize( model[i] * normal[i] ).xyz;
			
		gl_Position = persp[i] * view[i] * model[i] * gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}