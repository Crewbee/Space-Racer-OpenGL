#version 120

attribute vec3 a_Position;

uniform mat4 u_WorldPosition;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec4 u_Color;

//varying vec2 v_UV;
varying vec4 v_Color;

void main()
{
	// Store object space position.
	vec3 objectpos = a_Position;

	// Calculate world space position.
	
	vec4 worldpos = u_WorldPosition * vec4(objectpos, 1);

	// Calculate view space position.
	vec4 viewpos = u_ViewMatrix * worldpos;


	// Calculate clip space position.
	vec4 clippos = u_ProjectionMatrix * viewpos;

	// Store final position in gl_Position
    gl_Position = clippos;

//	v_UV = a_UV;
	v_Color = u_Color;
}
