<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<!DOCTYPE html>
<html lang="ko">
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
<!-- <script type="text/javascript"
	src="/resources/smarteditor/js/HuskyEZCreator.js" charset="utf-8"></script> -->
<script type="text/javascript"
	src="/resources/smarteditor/js/service/HuskyEZCreator.js" charset="utf-8"></script>
<script type="text/javascript" src="/resources/jquery/jquery.js"></script>
<script type="text/javascript"src="/resources/jquery/jquery-1.11.2.min.js"></script>

	<script type="text/javascript">
	
		$(function() {
			  
			var lag = "${language}";
		
			if(lag == "kr"){
				$("#language").val("kr");
			}
			else if(lag =="eng"){
				$("#language").val("eng");
			}
		});
		function selectEvent(selectobj){
					
			if($("#language option:selected").val() == "kr"){
				
				window.location.href = "HTMLEditUi.do";
			}
			else{
				window.location.href = "EngHTMLEditUi.do";
			
			}
		}
		//iframe 길이
		$("#main_Iframe", parent.document).width(900);
		
	
	</script>
	
</head>
<body>
<div style="margin:0 auto;">
	<form id="html-edit-form" action="HTML-edit-success.do" method="post">
	
		<select name="language" id = "language" onchange="javascript:selectEvent(this)">
			<%-- <option value="${language}">language</option> --%>
    		<option value="kr">Korean</option>
	    	<option value="eng">English</option>
		</select>
		
		<table style="width: 100%">

		
			<tr>
				<td><textarea rows="10" cols="87" id="ir1" name="content"><c:out
							value="${content }"></c:out></textarea></td>
				<script type="text/javascript">
					var oEditors = [];
					nhn.husky.EZCreator
							.createInIFrame({
								oAppRef : oEditors,
								elPlaceHolder : "ir1",
								sSkinURI : "/resources/smarteditor/SmartEditor2Skin_en_US.html",
								htParams : {
									bUseToolbar : true, // 툴바 사용 여부
									bUseVerticalResizer : true, // 입력창 크기 조절바 사용여부
									bUseModeChanger : true, //모드 탭 사용 여부

									fOnBeforeUnload : function() {

									}
								},
								fOnAppLoad : function() {
								},
								fCreator : "createSEditor2"
							});
				</script>
			</tr>
			<tr>
				<td><input type="button" id="save" value="Save"> <script
						type="text/javascript">
					$("#save").click(
							function() {
								oEditors.getById["ir1"].exec("UPDATE_CONTENTS_FIELD", []);
								$("#html-edit-form").submit();
							});
				</script></td>
			</tr>
		</table>
	</form>
	</div>
</body>
</html>