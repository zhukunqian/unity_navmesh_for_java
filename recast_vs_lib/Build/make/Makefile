PLATS = linux
.PHONY : linux all clean 

CC = g++

GSHARED = -fPIC --shared -lrt -Wl,-E
CFLAGS = -g -O2 -Wall -msse -msse2 -ffast-math  -mfpmath=sse

RECAST_INC ?= DemoSource

RECAST_SRC = ../../DemoSource/javalib.cpp \
	../../DemoSource/NavMesh.cpp \
	../../DemoSource/RecastLib.cpp \
	../../Source/DetourAlloc.cpp \
	../../Source/DetourCommon.cpp \
        ../../Source/DetourCrowd.cpp \
	../../Source/DetourLocalBoundary.cpp \
	../../Source/DetourNavMeshBuilder.cpp \
	../../Source/DetourNavMesh.cpp \
	../../Source/DetourNavMeshQuery.cpp \
	../../Source/DetourNode.cpp \
	../../Source/DetourObstacleAvoidance.cpp \
	../../Source/DetourPathCorridor.cpp \
	../../Source/DetourPathQueue.cpp \
	../../Source/DetourProximityGrid.cpp \
	../../Source/DetourTileCacheBuilder.cpp \
	../../Source/DetourTileCache.cpp \
	../../Source/RecastAlloc.cpp \
	../../Source/RecastArea.cpp \
	../../Source/RecastContour.cpp \
	../../Source/Recast.cpp \
	../../Source/RecastFilter.cpp \
	../../Source/RecastLayers.cpp \
	../../Source/RecastMesh.cpp \
	../../Source/RecastMeshDetail.cpp \
	../../Source/RecastRasterization.cpp \
	../../Source/RecastRegion.cpp

linux : recastDll.so
        $(MAKE) all PLAT=$@ GSHARED="$(GSHARED)"

all : recastDll.so

recastDll.so : $(RECAST_SRC)
	$(CC) $(CFLAGS) $(GSHARED) $^ -o $@ -I../../DemoSource/ -I../../Include -I/opt/jdk1.8.0_45/include -I/opt/jdk1.8.0_45/include/linux

