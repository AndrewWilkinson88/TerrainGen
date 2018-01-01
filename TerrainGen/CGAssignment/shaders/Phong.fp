#version 150

//Material Properties
uniform vec4 ambientProduct;
uniform vec4 diffuseProduct;
uniform vec4 specularProduct;
uniform float shininess;

//Per fragment coming in
in	vec3	L;
in	vec3	E;
in	vec3	H;
in	vec3	N;
in  vec2	texCoord;

uniform sampler2D texture;

// Output fragment color
out vec4 vFragColor;

void main(void)
{ 
	vec4 color;

	vec4 ambient = ambientProduct;
	float kd = max( dot(L, N), 0.0 );
	vec4 diffuse = kd * diffuseProduct;
	float ks = pow( max( dot(N, H), 0.0), shininess);
	vec4 specular = ks * specularProduct;
	if( dot(L, N) < 0.0 )
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	
	//color = ambient + diffuse + specular;
	//	color.a = 1.0;
	color = vec4(0.5, 0.5, 0.5, 1.0);

	vFragColor = color * texture2D( texture, texCoord );
}