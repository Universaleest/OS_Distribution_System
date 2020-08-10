package com.wayneOS.dev.controller;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.util.FileCopyUtils;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.wayneOS.dev.service.ImgService;
import com.wayneOS.dev.service.ProductfreeService;
import com.wayneOS.dev.service.ProductpaidService;

/**
 * 이미지 파일을 관리하는 컨트롤러
 */

@Controller
public class OSFileController {
	
	private static final Logger logger = LoggerFactory.getLogger(OSFileController.class);
	
	@Autowired
	ProductpaidService productPaidService;
	@Autowired 
	ProductfreeService productFreeService;
	@Autowired
	ImgService imgService;
	
	private int sendFileSize = 1024 * 1024 * 30;
	/**
	 * 사용자 디바이스 정보를 받아온 후 해당 이미지가 있는지 검사
	 */
	@RequestMapping(value = "/OSData.do", method = RequestMethod.POST)
	@ResponseBody
	public Map OSData(@RequestBody Map<String,Object> paramMap, HttpServletRequest request)
	{
	logger.info("OsData paramMap : " + paramMap.toString());
		HashMap<String, Object> returnMap = new HashMap<String, Object>();
		returnMap.put("responseType", "OSData");
		
		String realPath =  request.getSession().getServletContext().getRealPath("/");
		logger.info("location : " +request.getSession().getServletContext().getRealPath("/"));
		
		String imgName="";
		//paramMap 6개 데이터 담김 -> 데이터별로 OSFile 경로 참조!
		
		String versionType =  paramMap.get("versionType").toString();
		
		//유료버전일 때
		if(versionType.equals("PAID")){
			imgName =  productPaidService.ValidationProduct(paramMap);
			returnMap.put("updateFileName", imgName);
			returnMap.put("versionType", "PAID");
			logger.info("Paid version imgName : " + imgName);
		}
		
		//무료버전일 때
		else if(versionType.equals("FREE")){
			imgName =  imgService.ValidationFreeProduct(paramMap);
			returnMap.put("updateFileName", imgName);
			returnMap.put("versionType", "FREE");
			logger.info(" Free version imgName : " + imgName);
		}
		else{
			logger.info("wrong versionType");
			
		}
		

		if(imgName != ""){
			
			/*//windows
			String osFilePath  =  realPath +"\\..\\OSFile\\" + imgName;
			
			//linux
			//String osFilePath = realPath+"../OSFile/" +imgName;
*/
			String osVersion = System.getProperty("os.name");
			
			String osFilePath ="";
			
			//local test path
			if(osVersion.contains("Windows")){
				osFilePath = realPath + "\\..\\OSFile\\" +imgName;	
			}
			//linux server path
			else{
				osFilePath = realPath+"../OSFile/" + imgName;
			}
			
			
			File osFile = new File(osFilePath); 
			logger.info("OS File Size : " + osFile.length());
			returnMap.put("fileSize : ", osFile.length());
			
			long slideSize = sendFileSize; 
			long slideCount = osFile.length() / slideSize;
			
			logger.info("slideCount : " + slideCount);
			
			if( (osFile.length()%slideSize) != 0) slideCount++;
			
			if(versionType.equals("PAID")){
				//realseCount,releaseDate,userIPAddress Update
				productPaidService.UpdateLocalData(paramMap);
			}
			else if(versionType.equals("FREE")){
				//데이터가 없을 시 
				
				
				paramMap.put("imgName", imgName);
				paramMap.put("releaseCount", 1);
				productFreeService.ValidationProduct(paramMap);
				//있을시 releaseCount ++
			}
			returnMap.put("slideCount", slideCount); 
		}
		
		
		
		
		else if(imgName.equals("")){
			
		}
		return returnMap;
		
	}
	
	/**
	 * 해당 이미지를 분할 전송 (30M씪)
	 */
	@RequestMapping(value ="/OSFileDownload.do" , method = {RequestMethod.POST, RequestMethod.GET })
	public void OSFileDownload(@RequestBody Map<String,Object> paramMap, HttpServletRequest request,HttpServletResponse response) throws Exception {
		
		 long slideCount = Long.parseLong(paramMap.get("slideCount").toString());
		 long slideIndex = Long.parseLong(paramMap.get("slideIndex").toString());
		
		
		//testCode
		String imgName = paramMap.get("updateFileName").toString(); 
		String realPath =  request.getSession().getServletContext().getRealPath("/");
		
		/*//local test path
		String osFilePath  =  realPath +"\\..\\OSFile\\" + imgName;
		
		//linux server path
		//String osFilePath = realPath+"../OSFile/" +imgName;
*/
		
		String osVersion = System.getProperty("os.name");
		
		String osFilePath ="";
		
		//local test path
		if(osVersion.contains("Windows")){
			osFilePath = realPath + "\\..\\OSFile\\" +imgName;	
		}
		//linux server path
		else{
			osFilePath = realPath+"../OSFile/" + imgName;
		}
		
		File osFile = new File(osFilePath); 
		
		response.setContentType("application/octet-stream");
		//response.setHeader("Content-Type", "application/octet-stream");
		response.setHeader("Content-Transfer-Encoding", "binary");
		response.setHeader("Content-Disposition", "attachment;filename=" + java.net.URLEncoder.encode("wayneOSForUpdateFile" + slideIndex, "utf-8") + ";");
	    response.setHeader("Connection", "keep-alive");
	    
	    response.addHeader("slideFile","true");
	    response.addHeader("slideIndex",String.valueOf(slideIndex+1));
	  	response.addHeader("slideCount", String.valueOf(slideCount));
	  		
	    OutputStream out  =  null;
		FileInputStream fis = null;
	
		
		byte[] sendByte = null;
		
	    
	    //first ~ before last send
	    if(slideIndex < slideCount-1 ){
	    	response.setContentLength((int)sendFileSize);
	    	sendByte= new byte[sendFileSize];
	    	
	    	logger.info("slideIndex : "+ slideIndex);
	    
	    }
	    //last send
	    else if(slideIndex == slideCount -1){
	    	long lastSize = (osFile.length() - (slideIndex * (sendFileSize)));
	    	
	    	response.setContentLength((int)lastSize); //마지막 용량
	    	
	    	sendByte= new byte[(int)lastSize];
	    	logger.info("Last Size : " + lastSize);
	    	logger.info("slideIndex : "+ slideIndex+ " last Sending..");
	    }
	    
	
		try{
			out = response.getOutputStream();
			fis = new FileInputStream(osFile);
		
			
			if(slideIndex < slideCount -1){
				
				long pos = sendByte.length * slideIndex ;
				int readSize = sendByte.length;
				fis.skip(pos);
				fis.read(sendByte , 0, readSize);
				
				logger.info("reading byte : " +pos +" ~ " + (pos+sendByte.length) ); 
			
				
			}
			
			else if(slideIndex == slideCount -1){
					
				//long lastFileSize  = osFile.length() - (sendByte.length * slideIndex);
				long lastSize = (osFile.length() - (slideIndex * (sendFileSize)));
				logger.info("lastFileSize : " + lastSize);

				long pos = sendFileSize * slideIndex ;
				
				fis.skip(pos);
				
				logger.info("last reading byte :" + +pos + " ~ " + (pos+lastSize));
				
				fis.read(sendByte,0,(int)lastSize);
				
				//logger.info("last reading byte : " +pos +" ~ " + pos+osFilePath.length());
			}
			else if(slideIndex> slideCount){
				logger.error("slide Count error!");
			}
			
			FileCopyUtils.copy(sendByte,out);
		
		}
		catch(IOException e){
			logger.error("File Response Fail : " + osFilePath);
			e.printStackTrace();
		}
		out.flush();
		fis.close();
		out.close();
		
		String mimeType = response.getContentType();
		System.out.println(mimeType);
		/*
		HashMap<String, Object> returnMap = new HashMap<String, Object>();
		returnMap.put("data", sendByte);
		return returnMap;
		*/
	}
	
}
