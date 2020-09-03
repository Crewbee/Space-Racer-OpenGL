#pragma once
class Texture
{
protected:
    GLuint m_TextureID;
	GLuint m_CubeMapID;
	bool isCubeMap;
public:
    Texture(const char* filename);
	Texture(const char** cubemapFilenames, const char* TextureFilename);
	Texture(GLuint aTexture);
    virtual ~Texture();

    GLuint GetTextureID() { return m_TextureID; }
	GLuint GetCubeTextureID() { return m_CubeMapID; }
	void SetTexture(GLuint aTexture) { m_TextureID = aTexture; }
	bool GetIsCubeMap() { return isCubeMap; }
};

