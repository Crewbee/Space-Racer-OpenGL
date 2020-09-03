#version 120

attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

uniform float u_Time;
uniform vec3 u_RipplePostion;
uniform float u_RippleRange;
uniform mat4 u_WorldPosition;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 u_Origin[4];

varying vec2 v_UV;
varying vec4 v_Color;
varying float v_Time;
varying float v_Height;

void main()
{
	// Store object space position.
	vec4 objectpos = vec4(a_Position, 1.0f);

	vec3 u_RipplePostion = vec3(0, 0, 0);

	float freq = 3;
	float amplitude = 0.1;
	v_Height = 0;

	float dist = distance( u_RipplePostion, objectpos.xyz );

	float MaxDist = 4;
	float falloffPerc = 1 - dist/MaxDist;

	float offset = sin( dist * freq - u_Time) * falloffPerc;

	objectpos.y += offset * amplitude;

	v_Height += offset;
	
	// Calculate world space position.
	vec4 worldpos = u_WorldPosition * objectpos;

	// Calculate view space position.
	vec4 viewpos = u_ViewMatrix * worldpos;

	// Calculate clip space position.
	vec4 clipPos = u_ProjectionMatrix * viewpos;

	// Store final position in gl_Position
    gl_Position = clipPos;

	v_UV = a_UV;
	v_Color = a_Color;
	v_Time = u_Time;
}
