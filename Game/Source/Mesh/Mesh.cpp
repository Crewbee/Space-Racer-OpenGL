#include "GamePCH.h"

#include "Mesh/Mesh.h"
#include "Mesh/Texture.h"
#include "Mesh/Material.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"
#include "GameObjects/Lamp.h"
#include "Mesh.h"


Mesh::Mesh()
{
	m_VBO = 0;
	m_IBO = 0;

	m_ParticlePosbuffer = 0;
	m_ParticleColbuffer = 0;

	m_NumVerts = 0;
	m_NumIndices = 0;

	m_PrimitiveType = -1;

	m_pMeshSize = vec3(0, 0, 0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void Mesh::ClearBuffers()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

	m_VBO = 0;
	m_IBO = 0;
}

void Mesh::Init(VertexFormat* verts, int numVerts, unsigned int* indices, int numIndices, GLenum primitiveType, GLenum usage)
{
	m_NumVerts = numVerts;
	m_NumIndices = numIndices;
	m_PrimitiveType = primitiveType;

	// Generate and fill buffer with our vertex attributes.
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat)*numVerts, verts, usage);

	// Generate and fill buffer with our indices.
	if (m_IBO == 0)
		glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numIndices, indices, usage);

	CheckForGLErrors();
}

void Mesh::Init(const void* verts, int numVerts, GLenum primitiveType, GLenum usage)
{
	m_NumVerts = numVerts;
	m_NumIndices = 0;
	m_PrimitiveType = primitiveType;

	// Generate and fill buffer with our vertex attributes.
	if (m_VBO == 0)
		glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * numVerts, verts, usage);

	CheckForGLErrors();
}

void SetUniform1f(GLuint shader, const char* uniformName, float value)
{
	GLint loc = glGetUniformLocation(shader, uniformName);
	if (loc != -1)
	{
		glUniform1f(loc, value);
	}
}

void SetUniform2f(GLuint shader, const char* uniformName, vec2 value)
{
	GLint loc = glGetUniformLocation(shader, uniformName);
	if (loc != -1)
	{
		glUniform2f(loc, value.x, value.y);
	}
}

void SetUniform3f(GLuint shader, const char* uniformName, vec3 value)
{
	GLint loc = glGetUniformLocation(shader, uniformName);
	if (loc != -1)
	{
		glUniform3f(loc, value.x, value.y, value.z);
	}
}

void Mesh::SetupAttributes(Material* pMaterial)
{
	assert(m_VBO != 0);
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Bind buffer and set up attributes.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	GLint loc = glGetAttribLocation(shader, "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(shader, "a_Color");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Color));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(shader, "a_UV");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UV));
		glEnableVertexAttribArray(loc);
	}
	loc = glGetAttribLocation(shader, "a_Normal");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Norm));
		glEnableVertexAttribArray(loc);
	}
}

void Mesh::SetupFrameAttributes(Material * pMaterial)
{
	assert(m_VBO != 0);
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Bind buffer and set up attributes.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	GLint loc = glGetAttribLocation(shader, "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Position));
		glEnableVertexAttribArray(loc);
	}
	loc = glGetAttribLocation(shader, "a_UV");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UV));
		glEnableVertexAttribArray(loc);
	}
}

void Mesh::SetupParticleAttributes(Material* pMaterial)
{
	assert(m_VBO != 0);
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Bind buffer and set up attributes.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	GLuint loc = glGetAttribLocation(shader, "a_Position");
	if (loc != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos));
	}
}


void Mesh::SetupUniforms(Material* pMaterial, mat4 aWorldMatrix, GameCamera* aCamera)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Set up uniforms.
	GLint loc;


	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetViewMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_Time");
	if (loc != -1)
		glUniform1f(loc, static_cast<float>(GetSystemTimeSinceGameStart()));

	loc = glGetUniformLocation(shader, "u_LightPos");
	if (loc != -1)
	{
		glUniform3f(loc, -10.0f, -30.0f, -10.0f);
	}

	loc = glGetUniformLocation(shader, "u_CameraPos");
	if (loc != -1)
	{
		vec3 camPos = vec3(aCamera->GetViewMatrix()->m11, aCamera->GetViewMatrix()->m22, aCamera->GetViewMatrix()->m33);
		glUniform3f(loc, camPos.x, camPos.y, camPos.z);
	}

	loc = glGetUniformLocation(shader, "u_AlphaChannel");
	if (loc != -1)
	{
		glUniform1f(loc, 1.0f);
	}

	loc = glGetUniformLocation(shader, "u_AlphaOverride");
	if (loc != -1)
	{
		glUniform1f(loc, 1.0f);
	}

	// Setup our texture.
	Texture* pTexture = pMaterial->GetTexture();
	if (pTexture != nullptr)
	{
		int textureUnitIndex = 0;
		glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
		glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
		GLint loc = glGetUniformLocation(shader, "u_TextureSampler");
		if (loc != -1)
		{
			glUniform1i(loc, textureUnitIndex);
		}
	}

	CheckForGLErrors();
}

void Mesh::SetupUniformsWithLight(Material* pMaterial, mat4 aWorldMatrix, mat4 NormalMatrix, GameCamera* aCamera, PointLight* PointLighting, SpotLight* SpotLighting)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Set up uniforms.
	GLint loc;

	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_NormalMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &NormalMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetViewMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}
	//Pass in Point Lighting
	{
		for (int i = 0; i < 3; i++)
		{
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].position").c_str());
			if (loc != -1)
			{
				glUniform3f(loc, PointLighting[i].Position.x, PointLighting[i].Position.y, PointLighting[i].Position.z);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].lightColor").c_str());
			if (loc != -1)
			{
				glUniform3f(loc, PointLighting[i].LightColor.x, PointLighting[i].LightColor.y, PointLighting[i].LightColor.z);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].ambientstr").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].Strength);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].specularstr").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].Strength);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].constant").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, 1.0f);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].linear").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].linear);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].quadratic").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].quadratic);
			}
		}
	}
	//Pass in Spot Lighting
	{
		loc = glGetUniformLocation(shader, "u_SpotLights.position");
		if (loc != -1)
		{
			glUniform3f(loc, SpotLighting[0].Position.x, SpotLighting[0].Position.y, SpotLighting[0].Position.z);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.direction");
		if (loc != -1)
		{
			glUniform3f(loc, SpotLighting[0].Direction.x, SpotLighting[0].Direction.y, SpotLighting[0].Direction.z);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.lightColor");
		if (loc != -1)
		{
			glUniform3f(loc, SpotLighting[0].LightColor.x, SpotLighting[0].LightColor.y, SpotLighting[0].LightColor.z);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.innercutOff");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].innerCutOff);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.outercutOff");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].outerCutOff);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.specularstr");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].Strength);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.constant");
		if (loc != -1)
		{
			glUniform1f(loc, 1.0f);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.linear");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].linear);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.quadratic");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].quadratic);
		}

	}
	//Pass in Directional Lighting
	{
		loc = glGetUniformLocation(shader, "u_DirectionalLight.Direction");
		if (loc != -1)
		{
			glUniform3f(loc, 200.0f, 100.0f, -100.0f);
		}
		loc = glGetUniformLocation(shader, "u_DirectionalLight.lightColor");
		if (loc != -1)
		{
			glUniform3f(loc, 1.0f, 1.0f, 0.8f);
		}
	}
	loc = glGetUniformLocation(shader, "u_CameraPos");
	if (loc != -1)
	{
		vec3 camPos = vec3(aCamera->GetViewMatrix()->m11, aCamera->GetViewMatrix()->m22, aCamera->GetViewMatrix()->m33);
		glUniform3f(loc, camPos.x, camPos.y, camPos.z);
	}

	loc = glGetUniformLocation(shader, "u_AlphaOverride");
	if (loc != -1)
	{
		glUniform1f(loc, 1.0f);
	}

	// Setup our texture.
	Texture* pTexture = pMaterial->GetTexture();
	if (pTexture != nullptr)
	{
		if (pTexture->GetIsCubeMap() == false)
		{
			int textureUnitIndex = 0;
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
			GLint loc = glGetUniformLocation(shader, "u_TextureSampler");
			if (loc != -1)
			{
				glUniform1i(loc, textureUnitIndex);
			}
		}
		if (pTexture->GetIsCubeMap() == true)
		{
			int textureUnitIndex = 0;
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
			GLint loc = glGetUniformLocation(shader, "u_TextureSampler");
			if (loc != -1)
			{
				glUniform1i(loc, textureUnitIndex);
			}

			textureUnitIndex = 1;
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pTexture->GetCubeTextureID());
			loc = glGetUniformLocation(shader, "u_TextureCubeMap");
			if (loc != -1)
			{
				glUniform1i(loc, textureUnitIndex);
			}
		}
	}

	CheckForGLErrors();
}

void Mesh::SetupUniformsWithLightForDebris(Material * pMaterial, mat4 aWorldMatrix, mat4 NormalMatrix, GameCamera * aCamera, PointLight * PointLighting, SpotLight * SpotLighting, float pAlpha)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Set up uniforms.
	GLint loc;

	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_NormalMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &NormalMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetViewMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}
	//Pass in Point Lighting
	{
		for (int i = 0; i < 3; i++)
		{
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].position").c_str());
			if (loc != -1)
			{
				glUniform3f(loc, PointLighting[i].Position.x, PointLighting[i].Position.y, PointLighting[i].Position.z);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].lightColor").c_str());
			if (loc != -1)
			{
				glUniform3f(loc, PointLighting[i].LightColor.x, PointLighting[i].LightColor.y, PointLighting[i].LightColor.z);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].ambientstr").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].Strength);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].specularstr").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].Strength);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].constant").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, 1.0f);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].linear").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].linear);
			}
			loc = glGetUniformLocation(shader, ("u_PointLights[" + to_string(i) + "].quadratic").c_str());
			if (loc != -1)
			{
				glUniform1f(loc, PointLighting[i].quadratic);
			}
		}
	}
	//Pass in Spot Lighting
	{
		loc = glGetUniformLocation(shader, "u_SpotLights.position");
		if (loc != -1)
		{
			glUniform3f(loc, SpotLighting[0].Position.x, SpotLighting[0].Position.y, SpotLighting[0].Position.z);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.direction");
		if (loc != -1)
		{
			glUniform3f(loc, SpotLighting[0].Direction.x, SpotLighting[0].Direction.y, SpotLighting[0].Direction.z);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.lightColor");
		if (loc != -1)
		{
			glUniform3f(loc, SpotLighting[0].LightColor.x, SpotLighting[0].LightColor.y, SpotLighting[0].LightColor.z);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.innercutOff");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].innerCutOff);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.outercutOff");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].outerCutOff);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.specularstr");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].Strength);
		}

		loc = glGetUniformLocation(shader, "u_SpotLights.constant");
		if (loc != -1)
		{
			glUniform1f(loc, 1.0f);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.linear");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].linear);
		}
		loc = glGetUniformLocation(shader, "u_SpotLights.quadratic");
		if (loc != -1)
		{
			glUniform1f(loc, SpotLighting[0].quadratic);
		}

	}
	//Pass in Directional Lighting
	{
		loc = glGetUniformLocation(shader, "u_DirectionalLight.Direction");
		if (loc != -1)
		{
			glUniform3f(loc, 200.0f, 100.0f, -100.0f);
		}
		loc = glGetUniformLocation(shader, "u_DirectionalLight.lightColor");
		if (loc != -1)
		{
			glUniform3f(loc, 1.0f, 1.0f, 0.8f);
		}
	}
	loc = glGetUniformLocation(shader, "u_CameraPos");
	if (loc != -1)
	{
		vec3 camPos = vec3(aCamera->GetViewMatrix()->m11, aCamera->GetViewMatrix()->m22, aCamera->GetViewMatrix()->m33);
		glUniform3f(loc, camPos.x, camPos.y, camPos.z);
	}

	loc = glGetUniformLocation(shader, "u_AlphaOverride");
	if (loc != -1)
	{
		glUniform1f(loc, pAlpha);
	}
	// Setup our texture.
	Texture* pTexture = pMaterial->GetTexture();
	if (pTexture != nullptr)
	{
		if (pTexture->GetIsCubeMap() == false)
		{
			int textureUnitIndex = 0;
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
			GLint loc = glGetUniformLocation(shader, "u_TextureSampler");
			if (loc != -1)
			{
				glUniform1i(loc, textureUnitIndex);
			}
		}
		if (pTexture->GetIsCubeMap() == true)
		{
			int textureUnitIndex = 0;
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
			GLint loc = glGetUniformLocation(shader, "u_TextureSampler");
			if (loc != -1)
			{
				glUniform1i(loc, textureUnitIndex);
			}

			textureUnitIndex = 1;
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pTexture->GetCubeTextureID());
			loc = glGetUniformLocation(shader, "u_TextureCubeMap");
			if (loc != -1)
			{
				glUniform1i(loc, textureUnitIndex);
			}
		}
	}

	CheckForGLErrors();
}


void Mesh::SetupDebugUniforms(Material * pMaterial, mat4 aWorldMatrix, GameCamera* aCamera)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Set up uniforms.
	GLint loc;

	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetViewMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_Color");
	if (loc != -1)
	{
		glUniform4f(loc, pMaterial->GetColor().r / 255.0f, pMaterial->GetColor().g / 255.0f, pMaterial->GetColor().b / 255.0f, pMaterial->GetColor().a / 255.0f);
	}

	loc = glGetUniformLocation(shader, "u_AlphaOverride");
	if(loc != -1)
	{
		glUniform1f(loc, 1.0f);
	}

	CheckForGLErrors();
}
void Mesh::SetupLaserUniforms(Material * pMaterial, mat4 aWorldMatrix, GameCamera * aCamera, float pAlpha)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Set up uniforms.
	GLint loc;

	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetViewMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_Color");
	if (loc != -1)
	{
		glUniform4f(loc, pMaterial->GetColor().r / 255.0f, pMaterial->GetColor().g / 255.0f, pMaterial->GetColor().b / 255.0f, pMaterial->GetColor().a / 255.0f);
	}
	loc = glGetUniformLocation(shader, "u_AlphaOverride");
	if (loc != -1)
	{
		glUniform1f(loc, pAlpha);
	}
	CheckForGLErrors();

}
void Mesh::SetupUniformsRenderToTexture(Material * pMaterial, mat4 aWorldMatrix, GameCamera * aCamera, float pBlurStrength)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Set up uniforms.
	GLint loc;

	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldMatrix.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetViewMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_BlurStrength");
	if (loc != -1)
	{
		glUniform1f(loc, pBlurStrength);
	}

	// Setup our texture.
	Texture* pTexture = pMaterial->GetTexture();
	if (pTexture != nullptr)
	{
		int textureUnitIndex = 0;
		glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
		glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
		GLint loc = glGetUniformLocation(shader, "u_Texture");
		if (loc != -1)
		{
			glUniform1i(loc, textureUnitIndex);
		}
	}
	CheckForGLErrors();

}
void Mesh::SetupUniformsUI(Material* pMaterial, mat4 aWorldPosition, float alpha, GameCamera* aCamera)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	mat4 Identity;
	Identity.SetIdentity();

	// Set up uniforms.
	GLint loc;

	loc = glGetUniformLocation(shader, "u_WorldPosition");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aWorldPosition.m11);
	}

	loc = glGetUniformLocation(shader, "u_ViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &Identity.m11);
	}

	loc = glGetUniformLocation(shader, "u_ProjectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, &aCamera->GetProjMatrix()->m11);
	}

	loc = glGetUniformLocation(shader, "u_AlphaChannel");
	if (loc != -1)
	{
		glUniform1f(loc, alpha);
	}
	// Setup our texture.
	Texture* pTexture = pMaterial->GetTexture();
	if (pTexture != nullptr)
	{
		int textureUnitIndex = 0;
		glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
		glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
		GLint loc = glGetUniformLocation(shader, "u_TextureSampler");
		if (loc != -1)
		{
			glUniform1i(loc, textureUnitIndex);
		}
	}

	CheckForGLErrors();
}

void Mesh::SetupRenderFrameUniforms(Material * pMaterial)
{
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	GLuint shader = pMaterial->GetShader()->GetProgram();

	// Set up shader.
	glUseProgram(shader);

	// Setup Texture for Frame.
	Texture* pTexture = pMaterial->GetTexture();
	if (pTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
		GLint loc = glGetUniformLocation(shader, "u_Texture");
		if (loc != -1)
		{
			glUniform1i(loc, 0);
			CheckForGLErrors();
		}
	}
	CheckForGLErrors();
}
void Mesh::Draw(Material* pMaterial)
{
	assert(m_PrimitiveType != -1);
	assert(m_NumVerts != 0);
	assert(pMaterial != nullptr);
	assert(pMaterial->GetShader() != nullptr);
	assert(pMaterial->GetShader()->GetProgram() != 0);
	CheckForGLErrors();

	// Draw.
	if (m_NumIndices > 0)
	{
		glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_NumVerts);
	}

	// Disable the attribute arrays used.
	{
		GLuint shader = pMaterial->GetShader()->GetProgram();

		GLint aPos = glGetAttribLocation(shader, "a_Position");
		GLint aColor = glGetAttribLocation(shader, "a_Color");
		GLint aUV = glGetAttribLocation(shader, "a_UV");

		glDisableVertexAttribArray(aPos);

		if (aColor != -1)
			glDisableVertexAttribArray(aColor);

		if (aUV != -1)
			glDisableVertexAttribArray(aUV);
	}

	CheckForGLErrors();
}


