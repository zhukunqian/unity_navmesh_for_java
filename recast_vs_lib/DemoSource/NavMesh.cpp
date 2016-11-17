#include "NavMesh.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "platform.h"
#include "common.h"
#include <vector>


struct NavMeshSetHeader
{
	int version;
	int tileCount;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

NavMesh::NavMesh()
{
	navmeshLayer.pNavmesh = NULL;
	navmeshLayer.pNavmeshQuery = NULL;

	printf("create new navMesh");
}


NavMesh::~NavMesh()
{
	if (navmeshLayer.pNavmesh) {
		dtFreeNavMesh(navmeshLayer.pNavmesh);
	}
	if (navmeshLayer.pNavmeshQuery) {
		dtFreeNavMeshQuery(navmeshLayer.pNavmeshQuery);
	}
	printf("release navmesh");
}


bool NavMesh::create(std::string path)
{
	FILE* fp = fopen(path.c_str(), "rb");
	if (!fp) 
	{
		printf("can't find navmesh file");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size_t flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	uint8* data = new uint8[flen];
	if (data == NULL)
	{
		printf("memory error\n");
		fclose(fp);
		SAFE_RELEASE_ARRAY(data);
		return false;
	}

	size_t readsize = fread(data, 1, flen, fp);
	if (readsize != flen)
	{ 
		printf("read data error\n");
		fclose(fp);
		SAFE_RELEASE_ARRAY(data);
		return false;
	}

	if (readsize < sizeof(NavMeshSetHeader))
	{
		printf("readsize error\n");
		fclose(fp);
		SAFE_RELEASE_ARRAY(data);
		return false;
	}

	

	bool suc= create_core(data, flen);
	fclose(fp);
	SAFE_RELEASE_ARRAY(data);
	return suc;
}

bool NavMesh::create_core(uint8* data, size_t flen) {
	bool safeStorage = true;
	int pos = 0;
	int size = sizeof(NavMeshSetHeader);

	NavMeshSetHeader header;
	memcpy(&header, data, size);

	pos += size;

	if (header.version != NavMesh::RCN_NAVMESH_VERSION)
	{
		printf("navmesh version error\n");
		 
		return false;
	}

	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		printf("create navmesh error\n");
		 
		return false;
	}

	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		printf("mesh init error\n");
		 
		return false;
	}

	// Read tiles.
	bool success = true;
	for (int i = 0; i < header.tileCount; ++i)
	{
		NavMeshTileHeader tileHeader;
		size = sizeof(NavMeshTileHeader);
		memcpy(&tileHeader, &data[pos], size);
		pos += size;

		size = tileHeader.dataSize;
		if (!tileHeader.tileRef || !tileHeader.dataSize)
		{
			success = false;
			status = DT_FAILURE + DT_INVALID_PARAM;
			break;
		}

		unsigned char* tileData =
			(unsigned char*)dtAlloc(size, DT_ALLOC_PERM);
		if (!tileData)
		{
			success = false;
			status = DT_FAILURE + DT_OUT_OF_MEMORY;
			break;
		}
		memcpy(tileData, &data[pos], size);
		pos += size;

		status = mesh->addTile(tileData
			, size
			, (safeStorage ? DT_TILE_FREE_DATA : 0)
			, tileHeader.tileRef
			, 0);

		if (dtStatusFailed(status))
		{
			success = false;
			break;
		}
	}

	if (!success)
	{
		printf("NavMeshHandle::create:  %d\n", status);
		dtFreeNavMesh(mesh);
		return false;
	}

	dtNavMeshQuery* pMavmeshQuery = new dtNavMeshQuery();

	pMavmeshQuery->init(mesh, 1024);

	navmeshLayer.pNavmeshQuery = pMavmeshQuery;
	navmeshLayer.pNavmesh = mesh;

	uint32 tileCount = 0;
	uint32 nodeCount = 0;
	uint32 polyCount = 0;
	uint32 vertCount = 0;
	uint32 triCount = 0;
	uint32 triVertCount = 0;
	uint32 dataSize = 0;

	const dtNavMesh* navmesh = mesh;
	for (int32 i = 0; i < navmesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = navmesh->getTile(i);
		if (!tile || !tile->header)
			continue;

		tileCount++;
		nodeCount += tile->header->bvNodeCount;
		polyCount += tile->header->polyCount;
		vertCount += tile->header->vertCount;
		triCount += tile->header->detailTriCount;
		triVertCount += tile->header->detailVertCount;
		dataSize += tile->dataSize;



		printf("%f %f %f\n", tile->verts[0], tile->verts[1], tile->verts[2]);

		for (int m = 0; m < tile->header->detailVertCount; m++) {
			printf("%f %f %f\n", tile->detailVerts[m * 3 + 0], tile->detailVerts[m * 3 + 1], tile->detailVerts[m * 3 + 2]);
		}

		// DEBUG_MSG(fmt::format("NavMeshHandle::create: verts({}, {}, {})\n", tile->verts[0], tile->verts[1], tile->verts[2]));
	}

	printf("\t==> tiles loaded: %d\n", tileCount);
	printf("\t==> BVTree nodes: %d\n", nodeCount);
	printf("\t==> %d polygons (%d vertices)\n", polyCount, vertCount);
	printf("\t==> %d triangles (%d vertices)\n", triCount, triVertCount);
	printf("\t==> %.2f MB of data (not including pointers)\n", (((float)dataSize / sizeof(unsigned char)) / 1048576));
	return true;
 
}

int NavMesh::findStraightPath(const float* start, const float* end, std::vector<Position3D>& paths)
{
 
	// recast navmesh的查询接口。
	dtNavMeshQuery* navmeshQuery = navmeshLayer.pNavmeshQuery;
 
	// 这里好像是设置寻路相关的参数
	dtQueryFilter filter;
	filter.setIncludeFlags(0xffff);
	filter.setExcludeFlags(0);

	// 这也是相关参数，是做什么用的
	const float extents[3] = { 2.f, 4.f, 2.f };

	dtPolyRef startRef = INVALID_NAVMESH_POLYREF;
	dtPolyRef endRef = INVALID_NAVMESH_POLYREF;

	// 离起始点最近的点
	float startNearestPt[3];
	// 离终点最近的点
	float endNearestPt[3];
	// 查找最近的多边形，放在startRef中。
	navmeshQuery->findNearestPoly(start, extents, &filter, &startRef, startNearestPt);
	// 查找最近的多边形，放在endRef中。
	navmeshQuery->findNearestPoly(end, extents, &filter, &endRef, endNearestPt);

	// 如果能找到开始和结点的多边形
	if (!startRef || !endRef)
	{
		printf("NavMeshHandle::findStraightPath\n");
		return NAV_ERROR_NEARESTPOLY;
	}

	// 准备查找寻路的多边形，最多存放MAX_POLYS个网格。超出数量的就不放进去了。
	dtPolyRef polys[MAX_POLYS];
	// 查找到的数量
	int npolys;
	// 存放路径点
	float straightPath[MAX_POLYS * 3];
	// pathFlags是什么？
	unsigned char straightPathFlags[MAX_POLYS];
	dtPolyRef straightPathPolys[MAX_POLYS];
	int nstraightPath;
	int pos = 0;

	// 寻路
	navmeshQuery->findPath(startRef, endRef, startNearestPt, endNearestPt, &filter, polys, &npolys, MAX_POLYS);
	nstraightPath = 0;

	if (npolys)
	{
		// 如果有查找到的
		float epos1[3];
		// 复制点
		dtVcopy(epos1, endNearestPt);

		if (polys[npolys - 1] != endRef)
			navmeshQuery->closestPointOnPoly(polys[npolys - 1], endNearestPt, epos1, 0);
		// 这里是优化路径，将弯路改为直路
		navmeshQuery->findStraightPath(startNearestPt, endNearestPt, polys, npolys, straightPath, straightPathFlags, straightPathPolys, &nstraightPath, MAX_POLYS);

		Position3D currpos;
		for (int i = 0; i < nstraightPath * 3; )
		{
			currpos.x = straightPath[i++];
			currpos.y = straightPath[i++];
			currpos.z = straightPath[i++];
			paths.push_back(currpos);
			pos++;

			//DEBUG_MSG(fmt::format("NavMeshHandle::findStraightPath: {}->{}, {}, {}\n", pos, currpos.x, currpos.y, currpos.z));
		}
	}


	return pos;
}