#include "MeshData.h"

MeshData::MeshData()
{
	// Test triangle
	vertices = {
		{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f}, { 0.5f, 0.0f } },
		{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f}, { 1.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f}, { 0.0f, 1.0f } },
		{ { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f}, { 0.0f, 0.0f } }
	};
	indices = { 0, 1, 2, 0, 2, 3 };
}

MeshData::~MeshData()
{
}
