#include <stdio.h>
#include <stdlib.h>


#include "NavMesh.h"
#include <vector>

int main(int,char**)
{
	printf("demo start\n");
	
	// 
	NavMesh nav = NavMesh();
	bool suc=nav.create("srv_CAIBakedNavmesh.navmesh");
	if (!suc) {
		printf("create failed");
		return 0;
	}

	float start[3];
	start[0] = 1.727072;
	start[1] = 3.570761;
	start[2] = 30.0069;

	float end[3];
	end[0] = 32.76521;
	end[1] = 2.299392;
	end[2] = -5.955514;



	std::vector<Position3D> outPaths;
	int ret=nav.findStraightPath(start,end,outPaths);
	printf("return %d", ret);

	if (ret > 0) {
		std::vector<Position3D>::iterator iter = outPaths.begin();
		for (; iter != outPaths.end(); ++iter)
		{
			printf("%f %f %f\n", iter->x, iter->y, iter->z);
		}
	}

	return 0;
 
}