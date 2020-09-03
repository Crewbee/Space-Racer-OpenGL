#pragma once
class ShaderProgram;
class Texture;

class Material
{
protected:

	ShaderProgram* m_pShader;
	Texture* m_pTexture;
	MyColor m_pColor;

public:
	Material(ShaderProgram* aShader, Texture* aTexture);
	Material(ShaderProgram* aShader, MyColor aColor);
	virtual ~Material();

	ShaderProgram* GetShader() { return m_pShader; }
	Texture* GetTexture() { return m_pTexture ; }
	MyColor GetColor() { return m_pColor; }

	//Setter

	void SetColor(MyColor aColor) { m_pColor = aColor; }
	void SetTexture(Texture* newTexture) { m_pTexture = newTexture; }
};