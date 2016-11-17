#pragma once

#include <string>
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include <vector>
#include "common.h"
#include "platform.h"


class NavMesh
{
public:
	static const int MAX_POLYS = 256;
	static const int NAV_ERROR_NEARESTPOLY = -2;

	static const long RCN_NAVMESH_VERSION = 1;
	static const int INVALID_NAVMESH_POLYREF = 0;

	NavMesh();
	virtual ~NavMesh();
	
	bool create_core(uint8* data, size_t flen);
	bool create(std::string path);

	int findStraightPath(const float* start, const float* end, std::vector<Position3D>& paths);

	std::string resPath;

	struct NavmeshLayer
	{
		dtNavMesh* pNavmesh;
		dtNavMeshQuery* pNavmeshQuery;
	};

	NavmeshLayer navmeshLayer;
};

