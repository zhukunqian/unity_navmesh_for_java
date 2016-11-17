package com.seasun.navmesh;

public class RecastLibDemo2 {
	public static void main(String[] args) {

		RecastLib recast = new RecastLib();

		recast.hello();

		for (int m = 0; m < 1000000; m++) {
			int id = recast.load("srv_CAIBakedNavmesh.navmesh");
			System.out.println("id:" + id);
			  recast.release(id);
		}

		try {
			Thread.sleep(100000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
