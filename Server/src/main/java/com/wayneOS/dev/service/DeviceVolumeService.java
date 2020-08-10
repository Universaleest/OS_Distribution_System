package com.wayneOS.dev.service;

import java.util.List;
import java.util.Map;
/**
 * 용량을 관련된 인터페이스 생성
 */
public interface DeviceVolumeService {
	

	public void Save(List<Map> listMap);
	public List InqDeviceVolume();
	
}
