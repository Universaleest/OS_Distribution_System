package com.wayneOS.dev.service;

import java.util.List;
import java.util.Map;
/**
 * 무료배포에 관련된 인터페이스 생성
 */
public interface ProductfreeService {
	
	public void Save(List<Map> listMap);
	public List InqProductfree();
	
	public void ValidationProduct(Map<String,Object> paramMap);
	public void UpdateLocalData(Map<String,Object> paramMap);
	
	
	
}
