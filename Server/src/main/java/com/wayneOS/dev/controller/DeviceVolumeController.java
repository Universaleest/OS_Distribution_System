package com.wayneOS.dev.controller;

import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.service.DeviceVolumeService;
/**
 * 디바이스 관리 컨트롤러
 */
@Controller
public class DeviceVolumeController {

	@Autowired
	DeviceVolumeService devicevolumeservice;
	/**
	 * 화면 전송
	 */
	@RequestMapping(value = "/devicevolumeUi.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	public ModelAndView devicevolumeUi(Locale locale, Model model) {

		ModelAndView mav = new ModelAndView("product/devicevolumeUi");

		return mav;
	}
	
	
	/**
	 * jqgrid에 값 전송
	 */
	@RequestMapping(value = "/devicevolume/Inqdevicevolume.do", method = RequestMethod.POST)
	@ResponseBody
	public String Inqdevicevolume(@RequestBody Map<String, Object> paramMap) {
		List<Map<String, Object>> rows = devicevolumeservice.InqDeviceVolume();
		JqGridData gridData = new JqGridData(paramMap, rows);
		// gridData.setRows(rows);
		System.out.println(gridData.getJsonString());
		return gridData.getJsonString();
	}
	/**
	 * 콤보박스 등에 값 전송
	 */
	@RequestMapping(value = "/devicevolume/volumeAll.do", method = RequestMethod.POST)
	@ResponseBody
	public List<Map> volumeAll() {
		List<Map> returnList =  devicevolumeservice.InqDeviceVolume();
		HashMap<String, Object> returnMap = new HashMap<String, Object>();
		return returnList;
	}
	
	@RequestMapping(value = "/devicevolume/volumeAllToClient.do", method = RequestMethod.POST)
	@ResponseBody
	public List<Map> volumeAllToClient() {
		List<Map> returnList =  devicevolumeservice.InqDeviceVolume();
		HashMap<String, Object> returnMap = new HashMap<String, Object>();
		returnMap.put("responseType", "deviceVolume");
		returnList.add(returnMap);
		
		return returnList;
		
	}
	
	
	/**
	 * 넘어온 데이터를 insert, update, delete
	 */
	@RequestMapping(value = "/devicevolume/Save.do", method = RequestMethod.POST)
	@ResponseBody
	public String Save(@RequestBody List<Map> paramMap) {
		devicevolumeservice.Save(paramMap);
		return "";
	}
	
}
