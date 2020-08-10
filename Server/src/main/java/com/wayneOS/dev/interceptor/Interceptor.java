package com.wayneOS.dev.interceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;
/**
 * 사용자가 로그인 상태인지 확인
 */
public class Interceptor extends HandlerInterceptorAdapter{
	
	@Override
	public boolean preHandle(HttpServletRequest request,HttpServletResponse response, Object handler) throws Exception {
		// TODO Auto-generated method stub
		
		//System.out.println("URL : " + request.getRequestURI());
/*		
		try{
			
			if(request.getRequestURI().equals("loginUi.do")){
				return true;
			}
			if(request.getRequestURI().equals("")){
				return true;
			}
			//세션이  null이면 로그인 페이지로
			if(request.getSession().getAttribute("admin") == null ){
                 response.sendRedirect(""); 
                 return false;
			}
			
		}catch(Exception e){
			e.printStackTrace();
		}*/
		
		return true;
	}

}
