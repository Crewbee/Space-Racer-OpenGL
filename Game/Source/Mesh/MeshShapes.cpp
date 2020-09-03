#include "GamePCH.h"

#include "Mesh/Mesh.h"
#include "Mesh/Texture.h"
#include "Mesh.h"

void Mesh::CreateBox(vec2 size, vec2 offset)
{
	VertexFormat vertexAttributes[] =
	{
		VertexFormat(vec3(-size.x / 2, -size.y / 2, 0.0f) + offset, MyColor(0, 0, 0, 0), vec2(0.0f, 0.0f)),
		VertexFormat(vec3(-size.x / 2,  size.y / 2, 0.0f) + offset, MyColor(0, 0, 0, 0), vec2(0.0f, 1.0f)),
		VertexFormat(vec3(size.x / 2,  size.y / 2, 0.0f) + offset, MyColor(0, 0, 0, 0), vec2(1.0f, 1.0f)),
		VertexFormat(vec3(size.x / 2, -size.y / 2, 0.0f) + offset, MyColor(0, 0, 0, 0), vec2(1.0f, 0.0f)),
	};

	m_pMeshSize = vec3(size.x, size.y, 0);

	unsigned int indices[6] = { 0,1,2, 0,2,3 };

	Init(vertexAttributes, 4, indices, 6, GL_TRIANGLES, GL_STATIC_DRAW);
}
void Mesh::CreateLaserPlane(vec2 size, vec3 to, vec3 from)
{
	VertexFormat vertexAttributes[] =
	{
		VertexFormat(vec3(-size.x / 2, 0.0f, -size.y / 2) + from, MyColor(0, 0, 0, 0), vec2(0.0f, 0.0f)),
		VertexFormat(vec3(-size.x / 2, 0.0f, size.y / 2) + to, MyColor(0, 0, 0, 0), vec2(0.0f, 1.0f)),
		VertexFormat(vec3(size.x / 2, 0.0f, size.y / 2) + to, MyColor(0, 0, 0, 0), vec2(1.0f, 1.0f)),
		VertexFormat(vec3(size.x / 2, 0.0f, -size.y / 2) + from, MyColor(0, 0, 0, 0), vec2(1.0f, 0.0f)),
	};

	m_pMeshSize = vec3(size.x, size.y, 0);

	unsigned int indices[6] = { 0,1,2, 0,2,3 };

	Init(vertexAttributes, 4, indices, 6, GL_TRIANGLES, GL_STATIC_DRAW);
}
void Mesh::CreateFrame()
{
	VertexFormat vertexAttributes[] =
	{
		VertexFormat(vec2(-1.0f, -1.0f), MyColor(0, 0, 0, 0), vec2(0.0f, 0.0f)),
		VertexFormat(vec2(-1.0f,  1.0f), MyColor(0, 0, 0, 0), vec2(0.0f, 1.0f)),
		VertexFormat(vec2(1.0f,  1.0f), MyColor(0, 0, 0, 0), vec2(1.0f, 1.0f)),
		VertexFormat(vec2(1.0f, -1.0f), MyColor(0, 0, 0, 0), vec2(1.0f, 0.0f)),
	};

	unsigned int indices[6] = { 0,1,2, 0,2,3 };

	Init(vertexAttributes, 4, indices, 6, GL_TRIANGLES, GL_STATIC_DRAW);
}
void Mesh::CreateCube(vec3 size, vec2 offset)
{
	VertexFormat vertexAttributes[] =
	{
		//Front
		VertexFormat(vec3(-size.x / 2, -size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 0.0f)),  //bottom left
		VertexFormat(vec3(-size.x / 2,  size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 1.0f)), // top left
		VertexFormat(vec3(size.x / 2,  size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 1.0f)), //top right
		VertexFormat(vec3(size.x / 2, -size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 0.0f)), //bottom right
		//Rear
		VertexFormat(vec3(size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 0.0f)), //bottom left
		VertexFormat(vec3(size.x / 2,  size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 1.0f)),// top left
		VertexFormat(vec3(-size.x / 2,  size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 1.0f)), //top right
		VertexFormat(vec3(-size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 0.0f)),//bottom right
		//Top											    					 				    		 			 
		VertexFormat(vec3(-size.x / 2,  size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 0.0f)),  //bottom left
		VertexFormat(vec3(-size.x / 2,  size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 1.0f)), // top left
		VertexFormat(vec3(size.x / 2,  size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 1.0f)), //top right
		VertexFormat(vec3(size.x / 2,  size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 0.0f)), //bottom right
		//Bottom
		VertexFormat(vec3(-size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 0.0f)), //bottom left
		VertexFormat(vec3(-size.x / 2, -size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 1.0f)), // top left
		VertexFormat(vec3(size.x / 2, -size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 1.0f)),//top right
		VertexFormat(vec3(size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 0.0f)),//bottom right
		//Left		  	    							    					 				    		 		    
		VertexFormat(vec3(size.x / 2, -size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 0.0f)), //bottom left
		VertexFormat(vec3(size.x / 2,  size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 1.0f)), // top left
		VertexFormat(vec3(size.x / 2,  size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 1.0f)), //top right
		VertexFormat(vec3(size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 0.0f)), //bottom right
		//Right		  	    							    					 				    		 		    
		VertexFormat(vec3(-size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 0.0f)), //bottom left
		VertexFormat(vec3(-size.x / 2,  size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(0.0f, 1.0f)), // top left
		VertexFormat(vec3(-size.x / 2,  size.y / 2,  size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 1.0f)),//top right
		VertexFormat(vec3(-size.x / 2, -size.y / 2, -size.z / 2) + offset, MyColor(255, 255, 255, 255), vec2(1.0f, 0.0f)) //bottom right
	};

	unsigned int indices[36] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,9,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 };

	m_pMeshSize = size;

	Init(vertexAttributes, 24, indices, 36, GL_TRIANGLES, GL_STATIC_DRAW);
}

void Mesh::CreatePlane(vec2 WorldSize, vec2 VertexCount)
{
	int totalVertexCount = (int)(VertexCount.x * VertexCount.y);
	VertexFormat* vertexAttributes = new VertexFormat[totalVertexCount];

	vec2 Pivot = WorldSize / 2;

	int Num_Rows = (int)(VertexCount.x) - 1;
	int Num_Columns = (int)(VertexCount.y) - 1;

	int aVertexXCount = (int)VertexCount.x;

	float uvStepX = 4.0f / Num_Rows;
	float uvStepY = 4.0f / Num_Columns;

	int Num_Indices = Num_Rows * Num_Columns * 6;

	unsigned int* indices = new unsigned int[Num_Indices];

	for (int y = 0; y < VertexCount.y; y++)
	{
		for (int x = 0; x < VertexCount.x; x++)
		{
			int index = y * aVertexXCount + x;
			vertexAttributes[index] = VertexFormat();
			vertexAttributes[index].m_Pos = vec3(x*(WorldSize.x / (VertexCount.x - 1)), 0.0f, (y * (WorldSize.y / (VertexCount.y - 1)))) - Pivot;
			vertexAttributes[index].m_UV = vec2((x * uvStepX), (y * uvStepY));
			vertexAttributes[index].m_Color = MyColor(255, 255, 255, 255);
		}
	}

	for (int Points = 0; Points < (Num_Indices / 6); Points++)
	{
		indices[Points * 6] = Points + (Points / Num_Rows);
		indices[Points * 6 + 1] = (Points + aVertexXCount) + (Points / Num_Rows);
		indices[Points * 6 + 2] = (Points + (aVertexXCount + 1)) + (Points / Num_Rows);
		indices[Points * 6 + 3] = Points + (Points / Num_Rows);
		indices[Points * 6 + 4] = (Points + (aVertexXCount + 1)) + (Points / Num_Rows);
		indices[Points * 6 + 5] = (Points + 1) + (Points / Num_Rows);
	}

	m_pMeshSize = vec3(WorldSize.x, WorldSize.y, 0);

	Init(vertexAttributes, totalVertexCount, indices, Num_Indices, GL_TRIANGLES, GL_STATIC_DRAW);

	delete[] indices;
	delete[] vertexAttributes;
}
;
