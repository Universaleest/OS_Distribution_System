package com.wayneOS.dev;

import java.sql.Connection;
import java.sql.DriverManager;

import org.junit.Test;

public class mysqlTest {
	private static final String DRIVER = "com.mysql.jdbc.Driver";
	private static final String URL = "jdbc:mysql://127.0.0.1:3306/wayneOS?useSSL=false&useUnicode=true&characterEncoding=utf8";
	private static final String USER = "root";
	private static final String PW = "0000";
	
	@Test
	public void testConnection() throws Exception{
		Class.forName(DRIVER);
		try(Connection con = DriverManager.getConnection(URL, USER, PW)){
			System.out.println("성공");
			System.out.println(con);
		}catch(Exception e)
		{
			System.out.println("에러발생");
			e.printStackTrace();
		}
	}
}
