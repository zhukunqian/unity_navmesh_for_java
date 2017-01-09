package com.seasun.navmesh;

import java.util.List;

public class RecastLibDemo4 {
	public static void main(String[] args) {

		RecastLib recast = new RecastLib();

		recast.hello();
		int meshId = recast.load("srv_CAIBakedNavmesh.navmesh");
		if (meshId <= 0) {
			System.out.println("加载地图数据失败");
		}

		float[] start = new float[3];
		start[0] = 1.727072f;
		start[1] = 3.570761f;
		start[2] = 30.0069f;

		float[] end = new float[3];
		end[0] = 32.76521f;
		end[1] = 2.299392f;
		end[2] = -5.955514f;

		List<float[]> list = recast.raycast(meshId, start[0], start[1], start[2], end[0], end[1], end[2]);

		for (float[] f : list) {
			System.out.println(f[0] + " " + f[1] + " " + f[2]);
		}
		
		long time1 = 0;
		long time2 = 0;

		time1 = System.currentTimeMillis();
		for (int m = 0; m < 1000000; m++) {
			// recast.raycast(meshId, start[0], start[1], start[2], end[0], end[1], end[2]);
		}
		time2 = System.currentTimeMillis();
		System.out.println("recast.hello () 使用时间:" + (time2 - time1));
		
	}
}
