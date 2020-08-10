package com.wayneOS.dev.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import com.wayneOS.dev.service.MemberService;

/**
 * 프로젝트 실행시 초기에 뜨는 화면을 관리하는 컨트롤러
 */
@Controller
public class MainController {

	@Autowired
	MemberService memberService;

	private static final Logger logger = LoggerFactory.getLogger(MainController.class);

	/**
	 * 화면전송
	 */
	@RequestMapping(value = "/")
	public ModelAndView main(Model model) {
				   
		ModelAndView mav = new ModelAndView("main/loginUi");

		return mav;
	}
	
}
