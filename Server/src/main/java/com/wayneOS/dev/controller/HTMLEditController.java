package com.wayneOS.dev.controller;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.ModelAndView;

import com.wayneOS.dev.service.MemberService;
/**
 * 소개 변경에 관한 HTML 관리 컨트롤러
 */
@Controller
public class HTMLEditController {
	@Autowired
	MemberService memberService;

	private static final Logger logger = LoggerFactory.getLogger(MainController.class);
	/**
	 * 한글 html 파일을 읽어와 화면에 전송
	 */
	@RequestMapping(value = "/HTMLEditUi.do", method = { RequestMethod.POST,RequestMethod.GET })
	public ModelAndView htmledit(Model model, HttpServletRequest request, Map<String, Object> paramMap) {	//read
		ModelAndView mav = new ModelAndView("info/htmlEditUi");
		String content = "";
		
		try {
			
			String path = request.getSession().getServletContext().getRealPath("/resources/Info/mainInfo-Kr.html");
			
			// 읽기
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(new File(path)), "utf-8"));
			String temp = "";
			while ((temp = br.readLine()) != null) {
				content += temp + "\n";
			}
			//System.out.println(content);
		} catch (Exception e) {
			e.printStackTrace();
		}
		mav.addObject("content", content);
		mav.addObject("language", "kr");
		return mav;
	}
	/**
	 * 영문 html 파일을 읽어와 화면에 전송
	 */
	@RequestMapping(value = "/EngHTMLEditUi.do", method = { RequestMethod.POST,RequestMethod.GET })
	public ModelAndView EngHTMLEditUi(Model model, HttpServletRequest request, Map<String, Object> paramMap) {	//read
		ModelAndView mav = new ModelAndView("info/htmlEditUi");
		String content = "";
		
		try {
			
			String path = request.getSession().getServletContext().getRealPath("/resources/Info/mainInfo-Eng.html");
			
			// 읽기
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(new File(path)), "UTF-8"));
			String temp = "";
			while ((temp = br.readLine()) != null) {
				content += temp + "\n";
			}
			//System.out.println(content);
		} catch (Exception e) {
			e.printStackTrace(); 
		}
		mav.addObject("content", content);
		mav.addObject("language", "eng");
		return mav;
	}
	/**
	 * 화면에서 넘어온 데이터를 저장
	 */
	@RequestMapping(value = "/HTML-edit-success.do", method = {RequestMethod.POST, RequestMethod.GET })
	public ModelAndView HTMLeditsuccess(@RequestParam Map<String, Object> paramMap, HttpServletRequest request) {						//write

		ModelAndView mav = new ModelAndView("info/htmlEditUi");
		
		try {
			String content = paramMap.get("content").toString(); // html 코드
			mav.addObject("content", content);
			String path = "";
			if(paramMap.get("language").toString().equals("kr")){
				path = request.getSession().getServletContext().getRealPath("/resources/Info/mainInfo-Kr.html");
				mav.addObject("language", "kr");
			}
			else{
				path = request.getSession().getServletContext().getRealPath("/resources/Info/mainInfo-Eng.html");
				mav.addObject("language", "eng");
			}
		
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path), "UTF-8"));
			String utf8encoding = "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n";
			writer.write(utf8encoding);
			writer.write(content);
			writer.close();
		} catch (Exception e) {
			e.printStackTrace();
			
		}
		
		return mav;
	}
}
