<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="ko">
<head>
<link href='http://fonts.googleapis.com/css?family=Droid+Sans' rel='stylesheet' type='text/css'>
<meta charset="utf-8">
<title>WayneOS Distribution System</title>
<link href="/resources/css/style.css" media="screen" rel="stylesheet" type="text/css" />
<link href="/resources/css/iconic.css" media="screen" rel="stylesheet" type="text/css" />
<script src="/resources/js/prefix-free.js"></script>
<script type="text/javascript">
	function changeIframeUrl(url) {
		document.getElementById("main_Iframe").src = url;
	}
</script>
</head>
<body>
	<div class="wrap">
		<nav>
			<ul class="menu">
				<li><img src="/resources/images/logo.JPG" class="logo" /></li>
				<li><a href="#"><span class="iconic home"></span> Home</a></li>
				<li><a href="#"><span class="iconic plus-alt"></span>
						Client Management</a>
					<ul>
						<li><a href="javascript:changeIframeUrl('HTMLEditUi.do')">Edit Introduction</a></li>
						<li><a href="javascript:changeIframeUrl('LicenseEditUi.do')">Edit License</a></li>
					</ul></li>

				<li><a href="#"><span class="iconic magnifying-glass"></span>
						DB Management</a>
					<ul>
						<li><a href="javascript:changeIframeUrl('userUi.do')">Admin Management</a></li>
						<li><a href="javascript:changeIframeUrl('devicevolumeUi.do')">Device Management</a></li>
						<li><a href="javascript:changeIframeUrl('imgUi.do')">OS File Management</a></li>
						<li><a href="javascript:changeIframeUrl('productpaidUi.do')">Paid User Records</a></li>
						<li><a href="javascript:changeIframeUrl('productfreeUi.do')">Free User Records</a></li>
					</ul></li>
			</ul>
		</nav>
		<div class="clearfix">
			<iframe src="" class="iframe-userUi" id="main_Iframe"></iframe>
		</div>
	</div>
</body>
</html>