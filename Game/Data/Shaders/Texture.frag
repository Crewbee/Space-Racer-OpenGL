#version 120

uniform sampler2D u_TextureSampler;
uniform float u_AlphaChannel;

varying vec2 v_UV;

void main()
{
	vec4 fragCol = texture2D( u_TextureSampler, v_UV );

	vec4 result = vec4(fragCol.r, fragCol.g, fragCol.b, fragCol.a * u_AlphaChannel);

	gl_FragColor = result;
}
