package com.wayneOS.dev.dao;

import java.util.List;
import java.util.Map;
/**
 * 데이터베이스를 사용하기 위한 productpaid 인터페이스 생성
 */
public interface ProductpaidDAO {

	public void InsertProductpaid(Map map);
	public void DeleteProductpaid(Map map);
	public void SelectProductpaid(Map map);
	public void UpdateProductpaid(Map map);
	public List InqProductpaid();

	public String ValidationProduct(Map map);
	public void UpdateLocalData(Map map);	
	
	public Integer SelectReleaseCount(Map map);
}
