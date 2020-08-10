package com.wayneOS.dev.service.serviceImpl;

import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.dao.ProductpaidDAO;
import com.wayneOS.dev.service.ProductpaidService;
/**
 * productpaidDAO 호출
 */
@Service("ProductpaidService")
public class ProductpaidServiceImpl implements ProductpaidService {

	@Autowired
	ProductpaidDAO productpaidDAO;
	/**
	 * insert, delete, update 명령 수행
	 */
	@Override
	public void Save(List<Map> listMap) {
		// TODO Auto-generated method stub

		for (int i = 0; i < listMap.size(); i++) {

			Map<String, Object> map = listMap.get(i);
			String status = map.get("flag").toString();
			if (status.equals(JqGridData.STATUS_INSERT)) {
				productpaidDAO.InsertProductpaid(map);
			} else if (status.equals(JqGridData.STATUS_UPDATE)) {
				productpaidDAO.UpdateProductpaid(map);
				System.out.println(map.get("imgName"));
			} else if (status.equals(JqGridData.STATUS_DELETE)) {
				System.out.println("D " + map.get(i));
				productpaidDAO.DeleteProductpaid(map);
			}
		}

	}
	/**
	 * select 명령수행
	 */
	@Override
	public List InqProductpaid() {
		// TODO Auto-generated method stub
		return productpaidDAO.InqProductpaid();
	}
	/**
	 * 사용자가 전송한 디바이스 정보가 유료,무료버전일 때 제품이 존재하는지를 검사
	 */
	
	@Override
	public String ValidationProduct(Map<String, Object> paramMap)
	{
		return productpaidDAO.ValidationProduct(paramMap);
		
		
	}
	/**
	 * 제품 정보가 존재 할 시 IP,RealseCount,lastReleaseDate 수정
	 */
	@Override
	public void UpdateLocalData(Map<String,Object> paramMap){
		
		//paramMap has a userIPAddress
		//paramMap.put("userIPAddress", "testIP");

		Integer releaseCount = productpaidDAO.SelectReleaseCount(paramMap);
		
		if(releaseCount == null)
			{
				releaseCount = new Integer(0);
				
			}
		paramMap.put("releaseCount", ++releaseCount);
		
		System.out.println("UpdateLocalData : " +paramMap );

		String now = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date(System.currentTimeMillis()));
		paramMap.put("lastReleaseDate", now);
		
		
		
		productpaidDAO.UpdateLocalData(paramMap);
	}
}
