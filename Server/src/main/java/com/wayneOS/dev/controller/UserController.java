package com.wayneOS.dev.controller;

import java.util.List;

import java.util.Locale;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.codehaus.jackson.annotate.JsonUnwrapped;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
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
import com.wayneOS.dev.service.MemberService;
/**
 * 사용자 관리 컨트롤러
 */
@Controller
public class UserController {

	private static final Logger logger = LoggerFactory
			.getLogger(UserController.class);

	@Autowired
	MemberService memberService;
	/**
	 * 화면전송
	 */
	@RequestMapping(value = "/userUi.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	public ModelAndView userUi(Locale locale, Model model) {
		logger.info("Welcome home! The client locale is {}.", locale);

		ModelAndView mav = new ModelAndView("user/userUi");

		return mav;
	}
	/**
	 * 전송된 값을 확인 후 세션처리
	 */
	@RequestMapping(value = "/loginUi.do", method = RequestMethod.POST)
	@ResponseBody
	public boolean LoginUi(@RequestBody Map<String, Object> paramMap,
			HttpServletRequest request) {

		System.out.println(paramMap.size());

		System.out.println(paramMap.get("PW"));
		System.out.println(paramMap.get("ID"));

		if (memberService.Login(paramMap)) {
			if (paramMap.get("ID") != null || paramMap.get("PW") != null) {
				request.getSession().setAttribute("admin", paramMap);
				return true;
			}
		}

		return false;

	}
	/**
	 * 화면전송
	 */
	@RequestMapping(value = "/mainUi.do", method = RequestMethod.GET)
	public ModelAndView main(@RequestParam Map<String, Object> paramMap,
			HttpServletRequest request) {

		ModelAndView mav = new ModelAndView("main/mainUi");
		return mav;

	}
	/**
	 * jqgrid에 데이터 전송
	 */
	@RequestMapping(value = "/User/InqMember.do", method = RequestMethod.POST)
	@ResponseBody
	// public String InqMember(@RequestBody JqGridData gridData) {
	public String InqMember(@RequestBody Map<String, Object> paramMap) {

		System.out.println("Admin list");
		List<Map<String, Object>> rows = memberService.Inq();

		JqGridData gridData = new JqGridData(paramMap, rows);
		// gridData.setRows(rows);
		System.out.println(gridData.getJsonString());
		return gridData.getJsonString();
	}
	/**
	 * 넘어온 값을 insert, update, delete
	 */
	@RequestMapping(value = "/User/Save.do", method = RequestMethod.POST)
	@ResponseBody
	public String Save(@RequestBody List<Map> paramMap) {


		memberService.Save(paramMap);
		return "";

	}

}