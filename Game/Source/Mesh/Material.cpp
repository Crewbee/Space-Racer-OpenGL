#include "GamePCH.h"
#include "Material.h"
#include "Texture.h"

Material::Material(ShaderProgram * aShader, Texture * aTexture)
{
	m_pShader = aShader;
	m_pTexture = aTexture;
	m_pColor = MyColor(0, 0, 0, 0);
}

Material::Material(ShaderProgram* aShader, MyColor aColor)
{
	m_pShader = aShader;
	m_pColor = aColor;
	m_pTexture = nullptr;
}

Material::~Material()
{

}
