package com.wayneOS.dev.controller;

import java.util.List;
import java.util.Locale;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.service.DeviceVolumeService;
import com.wayneOS.dev.service.ProductfreeService;
/**
 * 무료배포 관리 컨트롤러
 */
@Controller
public class ProductfreeController {

	@Autowired
	ProductfreeService productfreeService;
	/**
	 * 화면전송
	 */
	@RequestMapping(value = "/productfreeUi.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	public ModelAndView productfreeUi(Locale locale, Model model) {

		ModelAndView mav = new ModelAndView("product/productfreeUi");

		return mav;
	}
	/**
	 * jqgrid에 값 전송
	 */
	@RequestMapping(value = "/Product/InqProductFree.do", method = RequestMethod.POST)
	@ResponseBody
	public String InqProductFree(@RequestBody Map<String, Object> paramMap,
			HttpServletRequest request) {

		List<Map<String, Object>> rows = productfreeService.InqProductfree();

//		for (int i = 1; i <= rows.size(); i++) {
//			rows.get(i - 1).put("order", i);
//		}
		JqGridData gridData = new JqGridData(paramMap, rows);
		String returnStr = gridData.getJsonString();
		return returnStr;
	}
	/**
	 * 화면에서 넘어온 값을 insert, update, delete
	 */
	@RequestMapping(value = "/Productfree/Save.do", method = RequestMethod.POST)
	@ResponseBody
	public String Save(@RequestBody List<Map> paramMap) {
		productfreeService.Save(paramMap);
		return "";

	}
}
