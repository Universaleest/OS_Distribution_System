package com.wayneOS.dev.controller;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;
import org.springframework.web.multipart.commons.CommonsMultipartFile;
import org.springframework.web.servlet.ModelAndView;

import com.wayneOS.dev.Model.JqGrid.JqGridData;

/**
 * 이미지 파일을 저장관리 하는 컨트롤러
 */

@Controller
public class OSUploadController {
	/**
	 * 화면전송
	 */
	@RequestMapping(value = "/OSUploadUi.do")
	public ModelAndView OSUploadUi(Model model) {
				   
		ModelAndView mav = new ModelAndView("main/upload_Sample");

		return mav;
	}
	/**
	 * 선택한 파일을 저장
	 */
	@RequestMapping(value = "OSUpload.do", method = { RequestMethod.POST,RequestMethod.GET })
	public ModelAndView OSUpload(HttpServletRequest request,
			HttpServletResponse response,MultipartHttpServletRequest multirequest,ModelMap model) throws Exception {
		System.out.println("업로드 컨트롤로 접근");
		
	
		
		Map<String, MultipartFile> files = multirequest.getFileMap();
		CommonsMultipartFile cmf = (CommonsMultipartFile) files.get("osfile");
		String osName = cmf.getOriginalFilename(); //파일이름
		String path = request.getSession().getServletContext().getRealPath("/resources/File")+"\\"+osName; //저장경로 설정
		
		File file = new File(path);
		cmf.transferTo(file);
				try {
					// insert method
					model.addAttribute("resMessage", "업로드 성공"); 
					} catch (Exception e) {
					model.addAttribute("resMessage", "업로드 실패"); 
					}
				return null;
		
	}
	/**
	 * 서버에 업로드 된 OSFile들을 읽어오는 함수
	 */
	@RequestMapping(value = "/OSResourceRead.do", method = { RequestMethod.POST,
			RequestMethod.GET })
	@ResponseBody
	public String OSResourceRead(HttpServletRequest request, @RequestBody Map<String, Object> paramMap) throws Exception {
		List imgList = new ArrayList();

		String realPath = request.getSession().getServletContext().getRealPath("/");

		String osVersion = System.getProperty("os.name");
		
		String path ="";
		
		//local test path
		if(osVersion.contains("Windows")){
			path = realPath + "\\..\\OSFile\\";	
		}
		//linux server path
		else{
			path = realPath+"../OSFile/";
		}


		File dirFile = new File(path);
		File[] fileList = dirFile.listFiles();
		int i = 1;
		for (File tempFile : fileList) {
			Map<String, Object> imgName = new HashMap<String, Object>();
			if (tempFile.isFile()) {

				String tempPath = tempFile.getParent();
				String tempFileName = tempFile.getName();
				imgName.put("imgName", tempFileName);
				imgName.put("idx", i);
			}
			imgList.add(i - 1, imgName);
			i++;
		}
		JqGridData jqGridData = new JqGridData(paramMap, imgList);
		return jqGridData.getJsonString();
	}

}