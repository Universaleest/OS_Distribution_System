package com.wayneOS.dev.service.serviceImpl;

import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.dao.ProductfreeDAO;
import com.wayneOS.dev.service.ProductfreeService;
/**
 * productfreeDAO 호출
 */
@Service("ProductfreeService")
public class ProductfreeServiceImpl implements ProductfreeService{

	@Autowired
	ProductfreeDAO productfreeDAO;
	/**
	 * insert, delete, update 명령수행
	 */
	@Override
	public void Save(List<Map> listMap) {

		for (int i = 0; i < listMap.size(); i++) {
			// memberDAO.InsertMember();
			Map<String, Object> map = listMap.get(i);
			String status = map.get("flag").toString();
			if (status.equals(JqGridData.STATUS_INSERT)) {
				productfreeDAO.InsertProductfree(map);
			}
			else if(status.equals(JqGridData.STATUS_UPDATE)){
				productfreeDAO.UpdateProductfree(map);
			}
			else if(status.equals(JqGridData.STATUS_DELETE)){
				System.out.println("D "+map.get(i));
				productfreeDAO.DeleteProductfree(map);				
			}
		}

	}
	/**
	 * select 명령 수행
	 */
	@Override
	public List InqProductfree() {
		// TODO Auto-generated method stub
		System.out.println("call"); 
		return productfreeDAO.InqProductfree();
	}
	
	
	
	
	@Override
	public void ValidationProduct(Map<String, Object> paramMap)
	{
		//return productfreeDAO.ValidationProduct(paramMap);
		
		Map<String,Object>  map = productfreeDAO.SelectOne(paramMap);
		if(map == null){
			productfreeDAO.InsertProductfree(paramMap);
		}
		else{
			System.out.println("map size : " + map.size());
			this.UpdateLocalData(paramMap);
		}
		
	}
	/**
	 * 제품 정보가 존재 할 시 IP,RealseCount,lastReleaseDate 수정
	 */
	@Override
	public void UpdateLocalData(Map<String,Object> paramMap){
		
		//paramMap has a userIPAddress
		//paramMap.put("userIPAddress", "testIP");

		Integer releaseCount = productfreeDAO.SelectReleaseCount(paramMap);
		System.out.println("Free releaseCount : " + releaseCount);
		if(releaseCount == null){
			releaseCount = new Integer(0);
			
		}
		paramMap.put("releaseCount", ++releaseCount);
		
		System.out.println("UpdateLocalData : " +paramMap );

		String now = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date(System.currentTimeMillis()));
		paramMap.put("lastReleaseDate", now);
		
		
		
		productfreeDAO.UpdateLocalData(paramMap);
	}
	
	
}
