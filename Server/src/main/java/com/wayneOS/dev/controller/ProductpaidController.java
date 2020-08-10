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
import com.wayneOS.dev.service.ProductpaidService;
/**
 * 유료배포를 관리하는 컨트롤러
 */
@Controller
public class ProductpaidController {

	@Autowired
	ProductpaidService productpaidService;

	/*
	 * @RequestMapping(value = "/productpaidInsert.do") public ModelAndView
	 * productpaidInsert(Model model) { Map<String, Object> map = new
	 * HashMap<String, Object>();
	 * 
	 * 
	 * map.put("vendorID", "LG_electronic"); map.put("productID",
	 * "Good_Ssd-4jf3200:0"); map.put("deviceType", "1");
	 * map.put("deviceVolume", "64"); map.put("userIPAddress", "211.119.223.2");
	 * map.put("lastReleaseDate", "2015/02/15/16:54:01");
	 * map.put("releaseCount", "2"); map.put("customerInfo0", "MS,미국");
	 * map.put("customerInfo1", "구매담당:빌게이츠,010-4444-4444");
	 * 
	 * productpaidService.Save(map); ModelAndView mav = new ModelAndView(""); }
	 */
	/**
	 * 화면전송
	 */
	@RequestMapping(value = "/productpaidUi.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	public ModelAndView productpaidUi(Locale locale, Model model) {

		ModelAndView mav = new ModelAndView("product/productpaidUi");

		return mav;
	}
	/**
	 * jqgrid에 값 전송
	 */
	@RequestMapping(value = "/Product/InqProductPaid.do", method = RequestMethod.POST)
	@ResponseBody
	public String InqProductPaid(@RequestBody Map<String, Object> paramMap,
			HttpServletRequest request) {

		List<Map<String, Object>> rows = productpaidService.InqProductpaid();
		
		JqGridData gridData = new JqGridData(paramMap, rows);
		String returnStr = gridData.getJsonString();
		return returnStr;

	}
	/**
	 * 화면에서 넘어온 값을 insert, update, delete
	 */
	@RequestMapping(value = "/Productpaid/Save.do", method = RequestMethod.POST)
	@ResponseBody
	public String Save(@RequestBody List<Map> paramMap) {
		System.out.println(paramMap.toString());
		
		productpaidService.Save(paramMap);
		
		return "";

	}
	/**
	 * 파업화면을 전송
	 */
	@RequestMapping(value = "/productpaidUiPop.do", method = RequestMethod.GET)
	public ModelAndView imgUiPop(Locale locale, Model model) {
		ModelAndView mav = new ModelAndView("product/productpaidUiPop");
		return mav;

	}
}
