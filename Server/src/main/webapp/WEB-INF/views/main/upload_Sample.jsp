<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%> 
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Insert title here</title>
</head>
<body>
<form method="post" enctype="multipart/form-data" action="OSUpload.do">

	<input type="file" name="osfile" id = "osfile">
	<input type=submit value="Register">
	
</form>
</body>
</html>