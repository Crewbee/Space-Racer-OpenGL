#version 120

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_UV;

uniform mat4 u_WorldPosition;
uniform mat4 u_NormalMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 u_CameraPos;

uniform float u_Time;

varying vec2 v_UV;
varying vec3 v_Normal;
varying vec3 v_FragPos;
varying vec3 v_CameraPos;
varying vec3 v_ObjPos;

void main()
{
	vec4 objectPos = vec4( a_Position, 1.0 );

	vec4 normalDir = vec4(a_Normal, 1.0);
	normalDir = u_NormalMatrix * normalDir;

	vec4 worldPos = u_WorldPosition * objectPos;

	vec4 viewPos = u_ViewMatrix * worldPos;

	vec4 clipPos = u_ProjectionMatrix * viewPos;

	vec3 FragPos = vec3(worldPos.x, worldPos.y, worldPos.z);

    gl_Position = clipPos;

	v_Normal = normalDir.xyz;
    v_UV = a_UV;
	v_FragPos = FragPos;
	v_ObjPos = objectPos.xyz;
	v_CameraPos = u_CameraPos;
}
