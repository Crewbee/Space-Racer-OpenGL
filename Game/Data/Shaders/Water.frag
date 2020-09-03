#version 120

uniform sampler2D u_Texture;

varying vec2 v_UV;
varying vec4 v_Color;
varying float v_Time;

void main()
{
	vec2 UVOffset = v_UV;

	//Calculate Texture UV ripples
	UVOffset.x += sin(UVOffset.x * 0.5f + v_Time * 1.0f) * 0.1f;
	UVOffset.y += sin((UVOffset.y - UVOffset.x) * 0.315f + v_Time * 0.75f) * 0.05f; 

	vec4 color = texture2D(u_Texture, UVOffset);
	color.a = 1.0f;

//	color += vec4 (0.0f, 0.0f, 0.6f, 0.0f); 

	gl_FragColor = color;
}
