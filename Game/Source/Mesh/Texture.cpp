#include "GamePCH.h"

#include "Mesh/Texture.h"
#include "Texture.h"

Texture::Texture(const char* filename)
{
    m_TextureID = LoadTexture( filename );
	m_CubeMapID = 0;
	isCubeMap = false;

}

Texture::Texture(const char ** cubemapFilenames, const char* TextureFilename)
{
	m_TextureID = LoadTexture(TextureFilename);
	m_CubeMapID = LoadTextureCubemap(cubemapFilenames);
	isCubeMap = true;
}
Texture::Texture(GLuint aTexture)
{
	m_TextureID = aTexture;
	m_CubeMapID = 0;
	isCubeMap = false;
}

Texture::~Texture()
{
    glDeleteTextures( 1, &m_TextureID );
}
