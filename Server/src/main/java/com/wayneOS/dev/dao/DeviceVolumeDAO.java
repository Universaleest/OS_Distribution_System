package com.wayneOS.dev.dao;

import java.util.List;
import java.util.Map;
/**
 * 데이터베이스를 사용하기 위한 deviceVolume 인터페이스 생성
 */
public interface DeviceVolumeDAO {

	public void InsertDeviceVolume(Map map);
	public void DeleteDeviceVolume(Map map);
	public void UpdateDeviceVolume(Map map);
	public void SelectDeviceVolume(Map map);
	public List InqDeviceVolume();
	
}
