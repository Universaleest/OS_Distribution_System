package com.wayneOS.dev.service.serviceImpl;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.dao.DeviceVolumeDAO;
import com.wayneOS.dev.service.DeviceVolumeService;
/**
 * devicevolumeDAO를 호출
 */
@Service("DeviceVolumeService")
public class DeviceVolumeServiceImpl implements DeviceVolumeService {

	@Autowired
	DeviceVolumeDAO devicevolumeDAO;
	/**
	 * insert, delete update 명령 수행
	 */
	@Override
	public void Save(List<Map> listMap) {
		// TODO Auto-generated method stub
		for (int i = 0; i < listMap.size(); i++) {
			Map<String, Object> map = listMap.get(i);
			String status = map.get("flag").toString();
			if (status.equals(JqGridData.STATUS_INSERT)) {
				devicevolumeDAO.InsertDeviceVolume(map);
			}
			else if(status.equals(JqGridData.STATUS_UPDATE)){
				devicevolumeDAO.UpdateDeviceVolume(map);
			}
			else if(status.equals(JqGridData.STATUS_DELETE)){
				devicevolumeDAO.DeleteDeviceVolume(map);
			}
			
		}
	}
	/**
	 * select 명령 수행
	 */
	@Override
	public List InqDeviceVolume() {
		// TODO Auto-generated method stub
		return devicevolumeDAO.InqDeviceVolume();
	}
}
