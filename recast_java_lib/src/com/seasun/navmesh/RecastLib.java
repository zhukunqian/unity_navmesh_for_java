package com.seasun.navmesh;

import java.util.List;

public class RecastLib {
	static {
		try {
			NativeLoad.loadNative("recastDll");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// ------------------------------------
	// start : RegisterNatives方式

	/*
	 * 加载地图方式1
	 * 
	 * 参数: path : 地图文件的路径例： c:/srv_CAIBakedNavmesh.navmesh
	 * 
	 * 返回值 int, navmeshId, 为0或负数表示加载失败，为正数表示加载成功，后续寻路时传入此id为参数
	 * 
	 */
	public native int load(String path);

	/*
	 * 加载地图方式2
	 * 
	 * 参数：meshName, 地图名：要求不重复 例：srv_CAIBakedNavmesh.navmesh
	 * 
	 * 参数: meshData, 地图数据
	 * 
	 * 返回值 int, navmeshId, 为0或负数表示加载失败，为正数表示加载成功，后续寻路时传入此id为参数
	 * 
	 */
	public native int load2(String meshName, byte[] meshData);

	// 寻路
	// 返回路径点列表，注意，查找不到时，会返回null，或list.size==0
	public native List<float[]> find(int navmeshId, float x1, float y1, float z1, float x2, float y2, float z2);

	// raycast
	// 返回路径点列表，注意，查找不到时，会返回null，或list.size==0
	public native List<float[]> raycast(int navmeshId, float x1, float y1, float z1, float x2, float y2, float z2);

	
	// 释放加载的地图数据
	public native int release(int navmeshId);

	// for test
	public native void hello();
	public native void hello2();

	// for test
	public native void testPermance(int navmeshId, float x1, float y1, float z1, float x2, float y2, float z2);

	// end
	// -------------------------------------

	// 注：写 c++代码时，可以使用以下命令查看函数签名：
	// javap -s com.seasun.navmesh.RecastLib
}
