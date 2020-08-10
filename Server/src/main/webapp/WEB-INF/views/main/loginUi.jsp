<!DOCTYPE html>
<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8"%>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<html>

<link rel="stylesheet" href="/resources/css/login.css">
<head>
<meta charset="UTF-8">
<script type="text/javascript"src="/resources/jquery/jquery-1.11.2.min.js"></script>
<script type="text/javascript" src="/resources/jquery/jquery.js"></script>
<script type="text/javascript"src="/resources/jquery/jquery.blockUI.js"></script>

<script type="text/javascript">

	$(function() {
		
		$("#btnLogin").click(function() {
			
			if(!($("#userID").val()=="")||(!($("#userPW").val()) == "")){
			var paramMap = {
				ID : $("#userID").val(),
				PW : $("#userPW").val()
			};
			jQuery.ajax({
				type : "POST",
				url : "/loginUi.do",
				// url:"/mainUi.do",
				dataType : "JSON", // 옵션이므로 JSON으로 받을게 아니면 안써도 됨
				data :  JSON.stringify(paramMap),
				contentType :"application/json;charset=UTF-8",  //한글 안깨지게
				success : function(data) {
					//alert(data);
					if (data == "true") {
						//window.location.href="<c:url value='mainUi.do'/>";
						window.location.href = "mainUi.do";
					}
					else{
						alert("Please check your ID and Password again.");
					}
				},
				error : function(xhr, status, error) {
					alert(error);
				}
			});
			}
			else{
				alert("Please input your ID and Password.");
			}
		});

		$(document).ready(function($) {
			// 페이지가 로딩될 때 'Loading 이미지'를 숨긴다.
			$('#viewLoading').hide();

			
			// ajax 실행 및 완료시 'Loading 이미지'의 동작을 컨트롤하자.
			$('#viewLoading').ajaxStart(function() {
				
				//$(this).show();
				$.blockUI();
				$(this).fadeIn(500);
				

			}).ajaxStop(function() {
				//$(this).hide();
				$(this).fadeOut(500);
				 $.unblockUI();
			});
		
		});

	});
</script>


</head>
<body>
	<div id="login">
	<div class="logo">
		<img src="/resources/images/logo.JPG"/>
	</div>
		<div id="triangle"></div>
		<h1>Log in</h1>
		<form>
			<input id="userID" name="userID" type="email" placeholder="ID" /> <input
				id="userPW" name="userPW" type="password" placeholder="Password" />

			<input id="btnLogin" name="btnLogin" type="button" value="Log in" />

		</form>
	</div>
	
	<div id="viewLoading">
		<img src="/resources/images/viewLoading.gif" />
	</div>
	
</body>


</html>