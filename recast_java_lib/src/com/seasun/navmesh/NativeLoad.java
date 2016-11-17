package com.seasun.navmesh;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

public class NativeLoad {
	public static void loadNative(String name) {
		try {
			boolean isWindows = System.getProperty("os.name", "").toLowerCase().contains("windows");

			String libname = "recastDll";
			String suffix = "";
			if (isWindows) {
				suffix += ".dll";
			} else {
				suffix += ".so";
			}

			URL libUrl = Thread.currentThread().getContextClassLoader().getResource(libname + suffix);
			URLConnection resConn = libUrl.openConnection();
			resConn.setUseCaches(false);
			InputStream inputStream = resConn.getInputStream();
			if (inputStream == null) {
				System.out.println("无法加载 recast 库");
			}

			ByteArrayOutputStream out = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			int n = 0;
			while (-1 != (n = inputStream.read(buffer))) {
				out.write(buffer, 0, n);
			}

			File f = File.createTempFile(libname, suffix);
			FileOutputStream fileOutputStream = new FileOutputStream(f);
			fileOutputStream.write(out.toByteArray());
			fileOutputStream.close();

			System.load(f.getAbsolutePath());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
