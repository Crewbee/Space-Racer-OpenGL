#version 120
uniform float u_AlphaOverride;

varying vec4 v_Color;

void main()
{
	vec4 color = v_Color;

	color.w *= u_AlphaOverride;

	gl_FragColor = color;
}
