#include "GamePCH.h"

#include "Mesh/Mesh.h"

void Mesh::CreateMeshFromOBJFile(const char* Filename)
{
	std::vector<vec3>VertPositions;
	std::vector<vec2>UVCoords;
	std::vector<vec3>NormalCoords;
	std::vector<unsigned int>Indicies;
	std::vector<VertexFormat>m_VertexFormats;

	long length = 0;

	char* buffer = LoadCompleteFile(Filename, &length);

	if (buffer == 0 || length == 0)
	{
		delete buffer;
		return;
	}
	char* next_token = 0;

	char* line = strtok_s(buffer, "\n", &next_token);
	while (line)
	{
		if (line[0] == 'v'  && line[1] == ' ')
		{
			float x;
			float y;
			float z;
			sscanf_s(line, "v %f %f %f", &x, &y, &z);

			VertPositions.push_back(vec3(x, y, z));
			if (VertPositions.size() % 3 == 0)
			{
				m_CollisionMeshPoints.push_back(vec3(x, y, z));
			}

		}

		if (line[0] == 'v' && line[1] == 't')
		{
			float x;
			float y;

			sscanf_s(line, "v t %f %f", &x, &y);

			UVCoords.push_back(vec2(x, y));
		}
		if (line[0] == 'v' && line[1] == 'n')
		{
			float Nx;
			float Ny;
			float Nz;

			sscanf_s(line, "v n %f %f %f", &Nx, &Ny, &Nz);

			NormalCoords.push_back(vec3(Nx, Ny, Nz));
		}
		if (line[0] == 'f')
		{
			int vertIndex[3];
			int UVIndex[3];
			int NormIndex[3];

			sscanf_s(line, "f %d / %d / %d %d / %d / %d %d / %d / %d", &vertIndex[0], &UVIndex[0], &NormIndex[0], &vertIndex[1],
				&UVIndex[1], &NormIndex[1], &vertIndex[2], &UVIndex[2], &NormIndex[2]);


			for (int i = 0; i < 3; i++)
			{
				m_VertexFormats.push_back(VertexFormat(VertPositions.at(vertIndex[i] - 1), MyColor(0, 0, 0, 255),
					UVCoords.at(UVIndex[i] - 1), NormalCoords.at(NormIndex[i] - 1)));

			}
		}

		line = strtok_s(0, "\n", &next_token);
	}
	float NewXSize = 0;
	float NewYSize = 0;
	float NewZSize = 0;

	int VertArraySize = VertPositions.size() - 1;

	for (int i = 0; i < VertArraySize; i++)
	{
		if (abs(VertPositions.at(i).x) + abs(VertPositions.at(VertArraySize - i).x) > NewXSize)
		{
			NewXSize = abs(VertPositions[i].x) + abs(VertPositions[VertArraySize - i].x);
		}

		if (abs(VertPositions[i].y) + abs(VertPositions[VertArraySize - i].y) > NewYSize)
		{
			NewYSize = abs(VertPositions[i].y) + abs(VertPositions[VertArraySize - i].y);
		}

		if (abs(VertPositions[i].z) + abs(VertPositions[VertArraySize - i].z) > NewZSize)
		{
			NewZSize = abs(VertPositions[i].z) + abs(VertPositions[VertArraySize - i].z);
		}
	}
	m_pMeshSize = vec3(NewXSize, NewYSize, NewZSize);

	Init(&m_VertexFormats[0], m_VertexFormats.size(), GL_TRIANGLES, GL_STATIC_DRAW);

	delete[]buffer;
}
