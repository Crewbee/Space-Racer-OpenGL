#version 120
uniform sampler2D u_Texture;
varying vec2 v_UV;
uniform float u_BlurStrength;

vec4 AddHorizontalBlur(vec4 Fragcolor, vec2 FragUV, sampler2D aTexture, float Offset, float BlurStrenth);
vec4 AddVerticalBlur(vec4 FragColor, vec2 FragUV, sampler2D aTexture, float Offset, float BlurStrenth);

void main()
{
    vec4 col = texture2D(u_Texture, v_UV);

	col += AddHorizontalBlur(col, v_UV, u_Texture, 0.12, u_BlurStrength);
	col += AddVerticalBlur(col, v_UV, u_Texture, 0.12, u_BlurStrength);

    gl_FragColor = col;
} 

vec4 AddHorizontalBlur(vec4 Fragcolor, vec2 FragUV, sampler2D aTexture, float Offset, float BlurStrenth)
{
	//Left
	Fragcolor += texture2D(aTexture, vec2(FragUV.x - Offset * 0.5, FragUV.y)) * 0.04;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x - Offset * 0.25, FragUV.y)) * 0.09;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x - Offset * 0.1, FragUV.y)) * 0.12;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x - Offset * 0.05, FragUV.y)) * 0.15;

	//Right
	Fragcolor += texture2D(aTexture, vec2(FragUV.x + Offset * 0.5, FragUV.y)) * 0.04;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x + Offset * 0.25, FragUV.y)) * 0.09;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x + Offset * 0.1, FragUV.y)) * 0.12;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x + Offset * 0.05, FragUV.y)) * 0.15;

	Fragcolor *= BlurStrenth;

	return Fragcolor;
}

vec4 AddVerticalBlur(vec4 Fragcolor, vec2 FragUV, sampler2D aTexture, float Offset, float BlurStrenth)
{
	//Down
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y - Offset * 0.5)) * 0.04;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y - Offset * 0.25)) * 0.09;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y - Offset * 0.1)) * 0.12;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y - Offset * 0.05)) * 0.15;

	//Up
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y + Offset *  0.5)) * 0.04;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y + Offset *  0.25)) * 0.09;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y + Offset *  0.1)) * 0.12;
	Fragcolor += texture2D(aTexture, vec2(FragUV.x, FragUV.y + Offset *  0.05)) * 0.15;

	Fragcolor *= BlurStrenth;

	return Fragcolor;
}

