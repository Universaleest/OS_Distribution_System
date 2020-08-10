package com.wayneOS.dev;

import javax.inject.Inject;

import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations= {"file:src/main/webapp/WEB-INF/spring/**/root-context.xml"})

public class MyBatisMySQLtest {
	@Inject
	private SqlSessionFactory sqlFactory;
	
	@Test
	public void testFactory() {
		System.out.println("===== sql session factory check start =====");
		System.out.println(sqlFactory);
		System.out.println("===== sql session factory check end =====");
	}
	
	@Test
	public void testSession() throws Exception{
		try(SqlSession session1 = sqlFactory.openSession();
			SqlSession session2 = sqlFactory.openSession();){
			System.out.println("===== mybatis sql session test start =====");
			System.out.println(session1);
			System.out.println(session2);
			System.out.println("===== mybatis sql session test end =====");
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
