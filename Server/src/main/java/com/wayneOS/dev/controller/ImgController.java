package com.wayneOS.dev.controller;

import java.util.List;
import java.util.Locale;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.service.ImgService;
/**
 * 이미지 화면을 관리하는 컨트롤러
 */
@Controller
public class ImgController {
	
	@Autowired
	ImgService imgservice;
	/**
	 * 화면 전송
	 */
	@RequestMapping(value = "/imgUi.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	public ModelAndView imgUi(Locale locale, Model model) {

		ModelAndView mav = new ModelAndView("img/ImgUi");

		return mav;
	}
	/**
	 * jqgrid에 데이터를 전송
	 */
	@RequestMapping(value = "/img/Inqimg.do", method = RequestMethod.POST)
	@ResponseBody
	public String Inqimg(@RequestBody Map<String, Object> paramMap) {

		List<Map<String, Object>> rows = imgservice.InqImg();
		JqGridData gridData = new JqGridData(paramMap, rows);
		// gridData.setRows(rows);
		System.out.println(gridData.getJsonString());
		return gridData.getJsonString();
	}
	/**
	 * 화면에서 넘어온 데이터를 insert, update, delete
	 */
	@RequestMapping(value = "/img/Save.do", method = RequestMethod.POST)
	@ResponseBody
	public String Save(@RequestBody List<Map> paramMap) {
		imgservice.Save(paramMap);
		return "";
	}
	/**
	 * 팝업창 화면 전송
	 */
	@RequestMapping(value = "/imgUiPop.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	public ModelAndView imgUiPop(Locale locale, Model model) {

		ModelAndView mav = new ModelAndView("img/ImgUiPop");

		return mav;
	}
}
