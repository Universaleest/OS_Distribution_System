package com.wayneOS.dev.service;

import java.util.List;
import java.util.Map;

/**
 * 사용자와 관련된 인터페이스 생성
 */

public interface MemberService {


	public void Save(List<Map> listMap);
	
	public List Inq();
	
	public boolean Login(Map map);
	
}


