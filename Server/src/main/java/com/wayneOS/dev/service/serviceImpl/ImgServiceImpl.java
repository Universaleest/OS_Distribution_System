package com.wayneOS.dev.service.serviceImpl;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.dao.ImgDAO;
import com.wayneOS.dev.service.ImgService;
/**
 * imgDAO를 호출
 */
@Service("ImgService")
public class ImgServiceImpl implements ImgService{

	@Autowired
	ImgDAO ImgDAO;
	/**
	 *  insert, delete, update 명령 수행
	 */
	@Override
	public void Save(List<Map> listMap) {
		// TODO Auto-generated method stub
		for (int i = 0; i < listMap.size(); i++) {
			Map<String, Object> map = listMap.get(i);
			String status = map.get("flag").toString();
			if (status.equals(JqGridData.STATUS_INSERT)) {
				ImgDAO.InsertImg(map);
			}
			else if(status.equals(JqGridData.STATUS_UPDATE)){
				ImgDAO.UpdateImg(map);
			}
			else if(status.equals(JqGridData.STATUS_DELETE)){
				ImgDAO.DeleteImg(map);
			}
			
		}
	}
	/**
	 * select 명령 수행
	 */
	@Override
	public List InqImg() {
		// TODO Auto-generated method stub
		return ImgDAO.InqImg();
	}
	
	/**
	 * select 명령 수행
	 */
	@Override
	public String ValidationFreeProduct(Map paramMap){
		
		return ImgDAO.ValidationFreeProduct(paramMap);
	}
	
}
