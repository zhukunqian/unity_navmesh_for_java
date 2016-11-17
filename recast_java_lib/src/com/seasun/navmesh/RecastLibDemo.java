package com.seasun.navmesh;

import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.util.List;

public class RecastLibDemo {
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

		List<float[]> list = recast.find(meshId, start[0], start[1], start[2], end[0], end[1], end[2]);

		for (float[] f : list) {
			System.out.println(f[0] + " " + f[1] + " " + f[2]);
		}

		long time1 = 0;
		long time2 = 0;

		time1 = System.currentTimeMillis();
		for (int m = 0; m < 1000000; m++) {
			recast.hello();
		}
		time2 = System.currentTimeMillis();
		System.out.println("recast.hello () 使用时间:" + (time2 - time1));
		
		time1 = System.currentTimeMillis();
		for (int m = 0; m < 1000000; m++) {
			recast.hello2();
		}
		time2 = System.currentTimeMillis();
		System.out.println("recast.hello2 () 使用时间:" + (time2 - time1));

		time1 = System.currentTimeMillis();
		for (int m = 0; m < 1000000; m++) {
			recast.find(meshId, start[0], start[1], start[2], end[0], end[1], end[2]);
		}
		time2 = System.currentTimeMillis();

		System.out.println("recast.find () 使用时间:" + (time2 - time1));

		time1 = System.currentTimeMillis();
		recast.testPermance(meshId, start[0], start[1], start[2], end[0], end[1], end[2]);
		time2 = System.currentTimeMillis();
		System.out.println("recast.testPerfor () 使用时间:" + (time2 - time1));

		for (int m = 0; m < 1; m++) {
			int id = recast.load("srv_CAIBakedNavmesh.navmesh");
			// System.out.println("id:" + id);
			recast.release(meshId);
		}

		try {
			FileInputStream fileInputStream = new FileInputStream("srv_CAIBakedNavmesh.navmesh");
			ByteArrayOutputStream out = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			int n = 0;
			while (-1 != (n = fileInputStream.read(buffer))) {
				out.write(buffer, 0, n);
			}
			fileInputStream.close();
			byte[] data = out.toByteArray();
			int id = recast.load2("caimesh", data);
			recast.find(id, start[0], start[1], start[2], end[0], end[1], end[2]);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
