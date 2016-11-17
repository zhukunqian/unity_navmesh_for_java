package com.seasun.navmesh;

import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;

public class RecastLibDemo3 {
	public static void main(String[] args) {

		RecastLib recast = new RecastLib();

		recast.hello();

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

			for (int m = 0; m < 1000000; m++) {
				int id = recast.load2("srv_CAIBakedNavmesh.navmesh", data);
				System.out.println("id:" + id);
				recast.release(id);
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		try {
			Thread.sleep(100000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
