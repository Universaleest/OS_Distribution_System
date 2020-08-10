package com.wayneOS.dev.dao;

import java.util.List;
import java.util.Map;
/**
 * 데이터베이스를 사용하기 위한 productfree 인터페이스 생성
 */
public interface ProductfreeDAO {

	public void InsertProductfree(Map map);
	public void DeleteProductfree(Map map);
	public void SelectProductfree(Map map);
	public void UpdateProductfree(Map map);
	public List InqProductfree();
	
	public String ValidationProduct(Map map);
	public void UpdateLocalData(Map map);	
	
	public Integer SelectReleaseCount(Map map);
	
	public Map SelectOne(Map map);
	
}
