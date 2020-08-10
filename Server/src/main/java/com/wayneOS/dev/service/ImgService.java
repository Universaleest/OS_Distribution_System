package com.wayneOS.dev.service;

import java.util.List;
import java.util.Map;
/**
 * 이미지에 관련된 인터페이스 생성
 */
public interface ImgService {

	public void Save(List<Map> listMap);
	public List InqImg();
	
	public String ValidationFreeProduct(Map paramMap);
	
}
