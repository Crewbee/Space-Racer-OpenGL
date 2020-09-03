#pragma once
#include "GameObjects/LightCasters.h"

class Material;
class Texture;
class GameCamera;
class GameObject;
class Lamp;

using namespace std;

struct VertexFormat
{
    vec3 m_Pos;
    MyColor m_Color;
    vec2 m_UV;
	vec3 m_Norm;
	vec2 m_Position;

	VertexFormat() {};

    VertexFormat(vec3 pos, MyColor color, vec2 uv, vec3 norm)
    {
        m_Pos = pos;
        m_Color = color;
        m_UV = uv;
		m_Norm = norm;
    }

	VertexFormat(vec3 pos, MyColor color, vec2 uv)
	{
		m_Pos = pos;
		m_Color = color;
		m_UV = uv;
	}

	VertexFormat(vec2 pos, MyColor color, vec2 uv)
	{
		m_Position = pos;
		m_Color = color;
		m_UV = uv;
	}
};

class Mesh
{
protected:
    GLuint m_VBO;
    GLuint m_IBO;

	GLuint m_ParticlePosbuffer;
	GLuint m_ParticleColbuffer;

    unsigned int m_NumVerts;
    unsigned int m_NumIndices;

    GLenum m_PrimitiveType;

	vec3 m_pMeshSize;

	vector<vec3>m_CollisionMeshPoints;

public:
    Mesh();
    virtual ~Mesh();

	void ClearBuffers();

    void Init(VertexFormat* verts, int numVerts, unsigned int* indices, int numIndices, GLenum primitiveType, GLenum usage);
    void Init(const void* verts, int numVerts, GLenum primitiveType, GLenum usage);

    void SetupAttributes(Material* pMaterial);
	void SetupFrameAttributes(Material* pMaterial);
	void SetupParticleAttributes(Material* pMaterial);

	void SetupUniforms(Material* pMaterial , mat4 aWorldMatrix, GameCamera* aCamera);
	void SetupUniformsWithLight(Material* pMaterial, mat4 aWorldMatrix, mat4 NormalMatrix, GameCamera* aCamera, PointLight* PointLighting, SpotLight* SpotLighting);
	void SetupUniformsWithLightForDebris(Material* pMaterial, mat4 aWorldMatrix, mat4 NormalMatrix, GameCamera* aCamera, PointLight* PointLighting, SpotLight* SpotLighting, float pAlpha);
	static void SetupDebugUniforms(Material* pMaterial, mat4 aWorldMatrix, GameCamera* aCamera);
	void SetupLaserUniforms(Material* pMaterial, mat4 aWorldMatrix, GameCamera* aCamera, float pAlpha);
	void SetupUniformsRenderToTexture(Material* pMaterial, mat4 aWorldMatrix, GameCamera* aCamera, float pBlurStrength);
	void SetupUniformsUI(Material* pMaterial, mat4 aWorldPosition, float alpha, GameCamera* aCamera);

	void SetupRenderFrameUniforms(Material* pMaterial);

	void Draw(Material* pMaterial);

    // The following functions are defined in MeshShapes.cpp:
    void CreateBox(vec2 size, vec2 offset);

    void CreateLaserPlane(vec2 size, vec3 to, vec3 from);

	void CreateFrame();

	void CreateCube(vec3 size, vec2 offset);

	void CreatePlane(vec2 WoldSize, vec2 VertexCount);

	void CreateMeshFromOBJFile(const char* Filename);

	vec3 GetMeshSize() { return m_pMeshSize; }

	vec3* GetMeshPoints() { return &m_CollisionMeshPoints[0]; }

	int GetNumVerts() { return m_CollisionMeshPoints.size(); }
};
