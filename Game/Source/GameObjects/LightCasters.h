#pragma once
#include "GamePCH.h"

struct PointLight
{
	vec3 Position;
	vec3 LightColor;

	float Strength;
	float Distance;

	float linear;
	float quadratic;
};
struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	vec3 LightColor;

	float innerCutOff;
	float outerCutOff;

	float Strength;
	float Distance;

	float linear;
	float quadratic;

};
