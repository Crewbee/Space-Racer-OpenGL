#include "GamePCH.h"

#include "BulletDebugDraw.h"
#include "Helpers/ResourceManager.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"

BulletDebugDraw::BulletDebugDraw(ResourceManager* aResourceManager, GameCamera* aCamera)
{
	m_debugMode = 0;
	m_Resources = aResourceManager;
	m_Camera = aCamera;
	m_DebugMaterial = m_Resources->GetMaterial("DebugMaterial");
	m_DebugWorlMatrix.SetIdentity();
}

BulletDebugDraw::~BulletDebugDraw()
{
	m_Camera = nullptr;
	m_Resources = nullptr;
}
void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	if (m_debugMode > 0)
	{
		float DebugVerts[6] = { from.getX(), from.getY(), from.getZ(),
								to.getX(), to.getY(), to.getZ() };

		m_DebugMaterial->SetColor(MyColor(color.getX() * 255, color.getY() * 255, color.getZ() * 255, 255));

		GLuint shader = m_DebugMaterial->GetShader()->GetProgram();

		GLuint buffer = 0;

		glGenBuffers(1, &buffer);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 6, &DebugVerts[0], GL_STATIC_DRAW);

		GLuint loc = glGetAttribLocation(shader, "a_Position");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			glEnableVertexAttribArray(loc);
		}

		glDisable(GL_CULL_FACE);

		Mesh::SetupDebugUniforms(m_DebugMaterial, m_DebugWorlMatrix, m_Camera);

		glPointSize(5.0f);
		glLineWidth(2.0f);
		glDrawArrays(GL_POINTS, 0, 2);
		glDrawArrays(GL_LINES, 0, 2);

		glDisableVertexAttribArray(loc);

		glEnable(GL_CULL_FACE);
	}
}

void BulletDebugDraw::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void BulletDebugDraw::AddRayPoints(btVector3* aRayPoints, int numPoints)
{
	if (m_Raypoints.size() == 0)
	{
		for (int i = 0; i < numPoints; i++)
		{
			m_Raypoints.push_back(aRayPoints[i]);
		}
	}
}
void BulletDebugDraw::DrawRays()
{
	if (!m_Raypoints.empty())
	{
		drawLine(m_Raypoints[0], m_Raypoints[1], btVector3(1, 0, 0));
		m_Raypoints.clear();
	}
}

void BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
{

}

void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
	printf(warningString);
}

void BulletDebugDraw::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	{
		//btVector3 to = pointOnB + normalOnB * distance;

		//const btVector3&from = pointOnB;

		//drawLine(from, to, color);

	}
}