#version 330
uniform float size;

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

void main() 
{
	float hsize = size / 2.0;
	
	vec4 vert = gl_in[0].gl_Position;
	vec4 a = vec4( vert.x + hsize, vert.y + hsize, vert.z, 1.0 ); //( +, +, + )
	vec4 b = vec4( vert.x - hsize, vert.y + hsize, vert.z, 1.0 ); //( -, +, + )
	vec4 c = vec4( vert.x + hsize, vert.y - hsize, vert.z, 1.0 ); //( +, -, + )
	vec4 d = vec4( vert.x - hsize, vert.y - hsize, vert.z, 1.0 ); //( -, -, + )
	
	//Triangle 1
	gl_Position = a;
	EmitVertex();
	
	gl_Position = b;
	EmitVertex();
	
	gl_Position = c;
	EmitVertex();
	EndPrimitive();
	
	//Triangle 2
	gl_Position = d;
	EmitVertex();
	
	gl_Position = c;
	EmitVertex();
	
	gl_Position = b;
	EmitVertex();
	EndPrimitive();
}