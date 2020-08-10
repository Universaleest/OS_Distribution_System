package com.wayneOS.dev.dao;

import java.util.List;
import java.util.Map;
/**
 * 데이터베이스를 사용하기 위한 img 인터페이스 생성
 */
public interface ImgDAO {

	public void InsertImg(Map map);
	public void DeleteImg(Map map);
	public void UpdateImg(Map map);
	public void SelectImg(Map map);
	public List InqImg();
	
	public String ValidationFreeProduct(Map paramMap);
	
	
}
