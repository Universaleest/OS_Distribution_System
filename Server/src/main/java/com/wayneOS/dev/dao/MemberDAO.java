package com.wayneOS.dev.dao;

import java.util.List;
import java.util.Map;

/**
 * 데이터 베이스를 사용하기 위한 memver 인터페이스 생성
 */
public interface MemberDAO {

	public void InsertMember(Map map);

	public void DeleteMember(Map map);
	
	public void UpdateMember(Map map);
	
	public Map SelectMember(Map map);
	
	public List InqMember();
}
