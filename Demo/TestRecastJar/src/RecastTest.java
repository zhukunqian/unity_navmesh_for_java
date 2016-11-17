import java.io.File;
import java.util.List;

import com.seasun.navmesh.RecastLib;

public class RecastTest {
	public static void main(String[] args) {
		RecastLib recast = new RecastLib();

		int meshId = recast.load(new File("srv_CAIBakedNavmesh.navmesh").getAbsolutePath());
		if (meshId <= 0) {
			System.out.println("load navmesh failed");
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
		if (list != null) {
			for (float[] l : list) {
				System.out.println("paths:" + l[0] + " " + l[1] + " " + l[2]);
			}
		}
	}
}
