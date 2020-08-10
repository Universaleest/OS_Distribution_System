package com.wayneOS.dev.service;

import java.util.List;
import java.util.Map;
/**
 * 유료배포에 관련된 인터페이스 생성
 */
public interface ProductpaidService {

	public void Save(List<Map> listMap);
	public List InqProductpaid();
	
	public String ValidationProduct(Map<String,Object> paramMap);
	
	public void UpdateLocalData(Map<String,Object> paramMap);
}
