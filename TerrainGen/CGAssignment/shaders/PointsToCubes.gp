#version 330

in mat4 model[];
in mat4 view[];
in mat4 persp[];

in vec4 normal[];

//Block Size
uniform float size;

//Light
uniform vec4 lightPosition;

out vec3 L;
out vec3 E;
out vec3 H;
out vec3 N;
out vec2 texCoord;

layout(points) in;
layout(triangle_strip, max_vertices = 36) out;

vec4 calculateNormal( vec4 a, vec4 b, vec4 c)
{
	vec3 norm1 = b.xyz - a.xyz;
	vec3 norm2 = c.xyz - a.xyz;
	
	return vec4( cross(norm1, norm2), 0.0 );
}

void addFace( vec4 a, vec4 b, vec4 c, vec4 d, int side )
{
	/*
	 b -- a		first triangle: a, b, c	
	 |    |		second triangle: d, c, b
	 |    |
	 d -- c
	*/
	////
	//Triangle 1
	////
	vec3 pos = (model[0] * a).xyz;
	L = normalize( lightPosition.xyz - pos );
	E = normalize( -pos );
	H = normalize( L + E );
	N = normalize( model[0] * calculateNormal(a, b, c) ).xyz;
	gl_Position = persp[0] * view[0] * model[0] * a;
	
	if( side == 5 ) //bottom
		texCoord = vec2(0.333, 1.0);
	else if (side == 4) //top
		texCoord = vec2(1.0, 1.0);
	else
		texCoord = vec2(0.666, 1.0);
	EmitVertex();

	pos = (model[0] * b).xyz;
	L = normalize( lightPosition.xyz - pos );
	E = normalize( -pos );
	H = normalize( L + E );
	N = normalize( model[0] * calculateNormal(a, b, c) ).xyz;
	gl_Position = persp[0] * view[0] * model[0] * b;
	
	if( side == 5 ) //bottom
		texCoord = vec2(0.0, 1.0);
	else if (side == 4) //top
		texCoord = vec2(0.666, 1.0);
	else
		texCoord = vec2(0.333, 1.0);
	EmitVertex();

	pos = (model[0] * c).xyz;
	L = normalize( lightPosition.xyz - pos );
	E = normalize( -pos );
	H = normalize( L + E );
	N = normalize( model[0] * calculateNormal(a, b, c) ).xyz;
	gl_Position = persp[0] * view[0] * model[0] * c;

	if( side == 5 ) //bottom
		texCoord = vec2(0.333, 0.0);
	else if (side == 4) //top
		texCoord = vec2(1.0, 0.0);
	else
		texCoord = vec2(0.666, 0.0);
	EmitVertex();
	EndPrimitive();
	
	/*
	 b -- a		first triangle: a, b, c	
	 |    |		second triangle: d, c, b
	 |    |
	 d -- c
	*/
	////
	//Triangle 2
	////
	pos = (model[0] * d).xyz;
	L = normalize( lightPosition.xyz - pos );
	E = normalize( -pos );
	H = normalize( L + E );
	N = normalize( model[0] * calculateNormal(d, c, b) ).xyz;
	gl_Position = persp[0] * view[0] * model[0] * d;

	if( side == 5 ) //bottom
		texCoord = vec2(0.0, 0.0);
	else if (side == 4) //top
		texCoord = vec2(0.666, 0.0);
	else
		texCoord = vec2(0.333, 0.0);
	EmitVertex();

	pos = (model[0] * c).xyz;
	L = normalize( lightPosition.xyz - pos );
	E = normalize( -pos );
	H = normalize( L + E );
	N = normalize( model[0] * calculateNormal(d, c, b) ).xyz;
	gl_Position = persp[0] * view[0] * model[0] * c;
	
	if( side == 5 ) //bottom
		texCoord = vec2(0.333, 0.0);
	else if (side == 4) //top
		texCoord = vec2(1.0, 0.0);
	else
		texCoord = vec2(0.666, 0.0);
	EmitVertex();

	pos = (model[0] * b).xyz;
	L = normalize( lightPosition.xyz - pos );
	E = normalize( -pos );
	H = normalize( L + E );
	N = normalize( model[0] * calculateNormal(d, c, b) ).xyz;
	gl_Position = persp[0] * view[0] * model[0] * b;
	
	if( side == 5 ) //bottom
		texCoord = vec2(0.0, 1.0);
	else if (side == 4) //top
		texCoord = vec2(0.666, 1.0);
	else
		texCoord = vec2(0.333, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main() 
{
	float hsize = size / 2.0;
	vec4 vert = gl_in[0].gl_Position;

	//Front
	vec4 a = vec4( vert.x + hsize, vert.y + hsize, vert.z + hsize, 1.0 ); //( +, +, + )
	vec4 b = vec4( vert.x - hsize, vert.y + hsize, vert.z + hsize, 1.0 ); //( -, +, + )
	vec4 c = vec4( vert.x + hsize, vert.y - hsize, vert.z + hsize, 1.0 ); //( +, -, + )
	vec4 d = vec4( vert.x - hsize, vert.y - hsize, vert.z + hsize, 1.0 ); //( -, -, + )
	
	//Back
	vec4 w = vec4( vert.x - hsize, vert.y + hsize, vert.z - hsize, 1.0 ); //( -, +, - );
	vec4 x = vec4( vert.x + hsize, vert.y + hsize, vert.z - hsize, 1.0 ); //( +, +, - );
	vec4 y = vec4( vert.x - hsize, vert.y - hsize, vert.z - hsize, 1.0 ); //( -, -, - );
	vec4 z = vec4( vert.x + hsize, vert.y - hsize, vert.z - hsize, 1.0 ); //( +, -, - );
	
	//z+ , abcd
	addFace(  a,  b,  c,  d, 0 );

	//z- , wxyz 
	addFace(  w,  x,  y,  z, 1 );

	//x+ , xazc
	addFace(  x,  a,  z,  c, 2 );

	//x- , bwdy
	addFace(  b,  w,  d,  y, 3 );

	//y+ , xwab
	addFace(  x,  w,  a,  b, 4 );

	//y- , cdzy
	addFace(  c,  d,  z,  y, 5 );
}