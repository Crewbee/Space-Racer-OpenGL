#version 120
struct DirectionLight
{
	vec3 Direction;
	vec3 lightColor;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight
{
	vec3 position;
	vec3 lightColor;

	vec3 diffuse;
	vec3 specular;

	float specularstr;
	float constant;
	float linear;
	float quadratic;

};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 lightColor;

	float innercutOff;
	float outercutOff;

	vec3 diffuse;
	vec3 specular;

	float specularstr;
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D u_Texture;

uniform DirectionLight u_DirectionalLight;
uniform PointLight u_PointLights[3];
uniform SpotLight u_SpotLights;
uniform float u_AlphaOverride;

varying vec2 v_UV;
varying vec3 v_Normal;
varying vec3 v_FragPos;
varying vec3 v_CameraPos;

vec3 CalculateDirectionalLighting(DirectionLight aLight, vec3 normal, vec3 viewDirection, vec3 FragColor);

vec3 CalculatePointLighting(PointLight aLight, vec3 normal, vec3 FragPos, vec3 viewDirection, vec3 FragColor);

vec3 CalcutlateSpotLighting(SpotLight aLight, vec3 normal, vec3 FragPos, vec3 viewDirection, vec3 FragColor);

void main()
{
	//Attributes
	vec3 Fragnorm = normalize(v_Normal);
	vec3 viewDir = normalize(v_CameraPos - v_FragPos);
	vec4 FragmentColor = texture2D(u_Texture, v_UV);
	vec3 FragCol = FragmentColor.xyz;

	//Ambient Light Global
	float ambient_Str = 0.1;
	vec3 Light_Color = vec3(1.0, 1.0, 0.8);

	//Initialize Lighted Fragment with global
	vec3 FinalLighting = (ambient_Str * Light_Color) * FragCol;

	//Calculate Point Light Input
	for(int i = 0; i < 3; i++)
	{
		FinalLighting += CalculatePointLighting(u_PointLights[i], Fragnorm, v_FragPos, viewDir, FragCol);
	}
	//Calculate Spot Light Input
	FinalLighting += CalcutlateSpotLighting(u_SpotLights, Fragnorm, v_FragPos, viewDir, FragCol);

	//Calculate Global Directional
	FinalLighting += CalculateDirectionalLighting(u_DirectionalLight, Fragnorm, viewDir, FragCol);

	//Finalize Fragment Color
	vec4 result = vec4(FinalLighting, (FragmentColor.w * u_AlphaOverride));
	//Pass the Color to the Fragment
	gl_FragColor = result;
}
vec3 CalculateDirectionalLighting(DirectionLight aLight, vec3 normal, vec3 viewDirection, vec3 FragColor)
{
	//Calculate Ambient
	aLight.ambient = 0.1 * aLight.lightColor;

	//Calculate Diffuse
	vec3 LightDirection = normalize(-aLight.Direction);
	float diffuse = max(dot(normal, LightDirection), 0.0);
	aLight.diffuse = diffuse * aLight.lightColor * FragColor;

	//Specular Light1
	vec3 ReflectionDir = reflect(-LightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, ReflectionDir), 0.0), 32);
	aLight.specular = specularPower * aLight.lightColor * FragColor;

	return vec3(aLight.ambient + aLight.diffuse + aLight.specular);

}
vec3 CalculatePointLighting(PointLight aLight, vec3 normal,  vec3 FragPos, vec3 viewDirection, vec3 FragColor)
{
	//Calculate Diffuse
	vec3 LightDirection = normalize(aLight.position - FragPos);
	float diffuse = max(dot(normal, LightDirection), 0.0);
	aLight.diffuse = diffuse * aLight.lightColor * FragColor;

	//Specular Light1
	vec3 ReflectionDir = reflect(-LightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, ReflectionDir), aLight.specularstr), 32);
	aLight.specular = specularPower * aLight.lightColor * FragColor;

	//Calculate attenuation
    float dist = length(aLight.position - FragPos);
    float attenuation = 1.0 / (aLight.constant + aLight.linear * dist + aLight.quadratic * (dist * dist)); 
	
	aLight.diffuse *= attenuation;
	aLight.specular *= attenuation;

	return vec3(aLight.diffuse + aLight.specular);

}
vec3 CalcutlateSpotLighting(SpotLight aLight, vec3 normal, vec3 FragPos, vec3 viewDirection, vec3 FragColor)
{
	vec3 lightDir = normalize(aLight.position - FragPos);

	float theta = dot(lightDir, normalize(-aLight.direction));
	float epsilon = aLight.innercutOff - aLight.outercutOff;
	float intensity = clamp((theta - aLight.outercutOff) / epsilon, 0.0, 1.0);  
	
	//Calculate Diffuse
	float diffuse = max(dot(normal, lightDir), 0.0);
	aLight.diffuse = diffuse * aLight.lightColor * FragColor;

	//Specular Light1
	vec3 ReflectionDir = reflect(-lightDir, normal);
	float specularPower = pow(max(dot(viewDirection, ReflectionDir), aLight.specularstr), 32);
	aLight.specular = specularPower * aLight.lightColor * FragColor;

	float dist = length(aLight.position - FragPos);
    float attenuation = 1.0 / (aLight.constant + aLight.linear * dist + aLight.quadratic * (dist * dist)); 
	
	aLight.diffuse *= intensity;
	aLight.specular *= intensity;

	aLight.diffuse *= attenuation;
	aLight.specular *= attenuation;

	return vec3(aLight.diffuse + aLight.specular);
}
