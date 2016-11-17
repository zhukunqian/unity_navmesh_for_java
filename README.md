# unity_navmesh_for_java
a java lib, for invoking navmesh using jni.

navmesh was exported by [https://github.com/kbengine/unity3d_nav_critterai](https://github.com/kbengine/unity3d_nav_critterai "kbengine/unity3d_nav_critterai")


# How to use
you can use build/recast.jar directly,it include dll lib for win64 and so lib for linux_64

# Code Demo

you can load navmesh data by absolutePath:

using recast.load(String absolutePath)

    RecastLib recast = new RecastLib();
	String path = new File(
				Thread.currentThread().getContextClassLoader().getResource("srv_CAIBakedNavmesh.navmesh").getFile())
						.getAbsolutePath();
	int meshId = recast.load(path);
	if (meshId <= 0) {
		System.out.println("load navmesh failed");
	}

or 

load navmesh data by byte[] data directly

using recast.load2(String meshName,byte[] meshData)

	RecastLib recast = new RecastLib();
	InputStream in = Thread.currentThread().getContextClassLoader().getResource("srv_CAIBakedNavmesh.navmesh")
					.openStream();
	byte[] data = IOUtils.toByteArray(in);
	int meshId = recast.load2("cainvamesh", data);
	if (meshId <= 0) {
		System.out.println("load navmesh failed");
	}

then use got meshId,use it to find paths:

	/*
	* x1,y1,z1 start position
	* x2,y2,z2 end position
	* 
	* success:return paths
	* failed:return null or empty list
	*/
	public List<float[]> find(int navmeshId, float x1, float y1, float z1, float x2, float y2, float z2);

coordate system is the same as unity's, Y axis is up.

if use want to release navmesh use release(int navmeshId)

	public int release(int navmeshId);

# Complete Example
	
you can view demo source in Demo folder

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


# How to export navmesh
view [https://github.com/kbengine/unity3d_nav_critterai](https://github.com/kbengine/unity3d_nav_critterai "kbengine/unity3d_nav_critterai")

# Build shared lib
if you want to build shared lib by yourself.

1、build x64 lib

-  use vs2015 open recast_vs_lib\Build\vs2015\recastdemo.sln
-  build release lib
-  then use can find recastDll.dll in recast_vs_lib\Build\vs2015\obj\Release
-  copy it to recast_java_lib\src folder and recast_java_lib\bin folder

2、build linux x64 lib

- in recast_vs_lib\Build\make, run make command
- use can find recastDll.so in recast_vs_lib\Build folder
- copy it to recast_java_lib\src folder and recast_java_lib\bin folder

3、build jar lib

- run recast_java_lib\build_jar.bat to build jar lib.
- if success,you can find recast.jar in recast_java_lib folder



# unity_navmesh_for_java
一个可以使用navmesh寻路的java库，通过jni调用c++写的底层库

unity中navmesh数据使用以下工具导出 [https://github.com/kbengine/unity3d_nav_critterai](https://github.com/kbengine/unity3d_nav_critterai "kbengine/unity3d_nav_critterai")


# How to use
可以直接将build/recast.jar复制在自己的工程中，jar包中已经包含了win64和linux 64的共享库，初始化时，会根据运行的平台自行选择加载的库。

# Code Demo

如何加载navmesh数据。
可以使用navmesh文件所在的绝对路径进行加载

使用 recast.load(String absolutePath) 方法

    RecastLib recast = new RecastLib();
	String path = new File(
				Thread.currentThread().getContextClassLoader().getResource("srv_CAIBakedNavmesh.navmesh").getFile())
						.getAbsolutePath();
	int meshId = recast.load(path);
	if (meshId <= 0) {
		System.out.println("load navmesh failed");
	}

或者 

直接加载navmesh的二进制数据

使用 recast.load2(String meshName,byte[] meshData) 方法

	RecastLib recast = new RecastLib();
	InputStream in = Thread.currentThread().getContextClassLoader().getResource("srv_CAIBakedNavmesh.navmesh")
					.openStream();
	byte[] data = IOUtils.toByteArray(in);
	int meshId = recast.load2("cainvamesh", data);
	if (meshId <= 0) {
		System.out.println("load navmesh failed");
	}

加载成功后返回meshId不为0，寻路时需要传入这个meshId

	/*
	* x1,y1,z1 开始坐标
	* x2,y2,z2 结束坐标
	* 
	* 成功:返回路径点
	* 失败:return null or empty list
	*/
	public List<float[]> find(int navmeshId, float x1, float y1, float z1, float x2, float y2, float z2);

注：坐标系和unity中相同，y轴向上。一定要注意。

如果navmesh不再被使用，想释放掉内存时可以主动调用 release(int navmeshId) 方法，否则，将一直在内存中直到进程终止。

	public int release(int navmeshId);

# Complete Example
	
可以在Demo文件夹中查看完整的工程

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


# How to export navmesh
导出导航网格数据，查看 [https://github.com/kbengine/unity3d_nav_critterai](https://github.com/kbengine/unity3d_nav_critterai "kbengine/unity3d_nav_critterai")

# Build shared lib
如果需要自己生成动态库

1、编译 win x64 下的库

-  使用 vs2015 打开 recast_vs_lib\Build\vs2015\recastdemo.sln
-  点击生成，注意一定要选择release发布
-  成功后在 recast_vs_lib\Build\vs2015\obj\Release 目录下找到 recastDll.dll 文件
-  复制到 recast_java_lib\src 目录和 recast_java_lib\bin 目录下

2、编译 linux x64 下的库

- 进入 recast_vs_lib\Build\make 目录，运行 make 命令
- 在 recast_vs_lib\Build 目录下找到 recastDll.so 文件
- 复制到 recast_java_lib\src 目录和 recast_java_lib\bin 目录下

3、打包生成 jar 文件

- 运行 recast_java_lib\build_jar.bat 生成 jar 文件.
- 生成后直接在 recast_java_lib目录下看到 recast.jar  

