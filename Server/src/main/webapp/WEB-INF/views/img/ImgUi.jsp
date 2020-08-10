<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%> 
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">

<link rel="stylesheet" type="text/css" media="screen" href="/resources/jquery/jquery-ui.min.css" />
<link rel="stylesheet" type="text/css" media="screen" href="/resources/jquery/ui.jqgrid.css" />
<link rel="stylesheet" type="text/css" media="screen" href="/resources/css/style.css" />
<link rel="stylesheet" type="text/css" media="screen" href="/resources/css/button.css" />

<script src="/resources/jquery/jquery-1.11.2.min.js" type="text/javascript"></script>
<script type="text/javascript" src="/resources/jquery/jquery.blockUI.js"></script>
<script src="/resources/jquery/jquery-migrate-1.2.1.js" type="text/javascript"></script>
<script src="/resources/jquery/grid.locale-en.js" type="text/javascript"></script>
<script src="/resources/jquery/jquery.jqGrid.src.js" type="text/javascript"></script>
<script src="/resources/jquery/JQuery-lib.js" type="text/javascript"></script>


<script type="text/javascript">

	$(document).ready(function() {
		//$("#viewLoading").fadeIn(500);
		//$('.wrap-loading').removeClass('display-none');
		$("#list").jqGrid({
			url : "/img/Inqimg.do",
			loadtext : 'Loading..',
			datatype : "json",
			mtype : "post",
			width : "650",
			height : "700",
			jsonReader : {
				repeatitems : false
			},
			colNames : [ 'idx', 'imgName','versionType','deviceType','availableDeviceVolume','insertDate', 'flag'],
			colModel : [ //data mapping and row attribute
			{
				name : 'idx',
				width : "50%"
			},{
				name : 'imgName',
				width : "50%"
			},{
				name : 'versionType',
				width : "50%"

			},{
				name : 'deviceType',
				width : "50%"

			},{
				name : 'availableDeviceVolume',
				width : "50%"

			},{
				name : 'insertDate',
				width : "50%"

			},{
				name : 'flag',
				hidden : true 
				
			} ],

			rowList : [ 10, 20, 30 ],
			caption : "OS file management",
			pager : '#pager',
			rowNum : '10',
			viewrecords : true,

			emptyrecords : "No data",
			loadonce : false,
			pager : $("#pager"),
			contentType : "application/json; charset=utf-8",

			serializeGridData : function(postData) {
				postData.formData = $("#list").serializeArray();
				return JSON.stringify(postData);
			},
			//korean language 
			ajaxGridOptions : {
				contentType : "application/json;charset=UTF-8"
			},

			//double click
			ondblClickRow : function(idx) {

				var rowdata = $("#list").jqGrid('getRowData', idx);
				
				jQuery("#idx").val(rowdata.idx);
				jQuery("#imgName").val(rowdata.imgName);
				jQuery("#versionType").val(rowdata.versionType);
				jQuery("#deviceType").val(rowdata.deviceType);
				jQuery("#availableDeviceVolume").val(rowdata.availableDeviceVolume);
				jQuery("#insertDate").val(rowdata.insertDate);

			}
			

		});
		 
		jQuery.ajax({
			type : "POST",
			url : "/devicevolume/volumeAll.do",
			dataType : "JSON", // 옵션이므로 JSON으로 받을게 아니면 안써도 됨
			contentType : "application/json;charset=UTF-8", //한글 안깨지게
			success : function(data) {
				$.each(data, function(){
					var volsize = this.volumeSize;
					//alert(volsize);
					//$('.wrap-loading').removeClass('display-none');
					//$("#viewLoading").fadeOut(500);
					$('.wrap-loading').addClass('display-none');
					$("#availableDeviceVolume").append("<option value='"+volsize+"'>"+volsize+"</option");
				});
			},beforeSend:function(){
		        //$('.wrap-loading').addClass('display-none');
				$('.wrap-loading').removeClass('display-none');
				
		    },
			error : function(xhr, status, error) {

				alert(xhr.reponseText);
			}

		});

		jQuery("#list").jqGrid('navGrid', "#pager", {
			edit : false,
			add : false,
			del : false,
			search : false,
			refresh : false
		});

		//flag
		var InsertArray = new Array();
		var count = 0;
		var gridNm = "list";
		//add button
		$("#btnInsert").click(function() {
			var rowid = $("#list").jqGrid('getGridParam', "selrow");
			var idx =$("#list").jqGrid('getRowData', rowid).idx;
			var total = $("#list").getGridParam("records") + 1;
			var imgName = jQuery("#imgName").val();
			var versionType = jQuery("#versionType").val();
			var deviceType = jQuery("#deviceType").val();
			var availableDeviceVolume = jQuery("#availableDeviceVolume").val();
			var insertDate = jQuery("#insertDate").val();
			

			var addData = {
				"idx" : idx,
				"imgName" : imgName,
				//"imgPath" : imgPath,
				"versionType" : versionType,
				"deviceType" : deviceType,
				"availableDeviceVolume" : availableDeviceVolume,
				"insertDate" : insertDate,
				"flag" : "I"
			};

			$("#list").jqGrid('addRowData', total, addData, "last");
			//color change
			BackgroundChange(gridNm, total, "#8AE634");
			InsertArray[count] = total;
			count++;
			
			
		});

		//select button
		$("#btnSelect").click(function() {

			$("#list").trigger('reloadGrid');

		});

		//Init button
		$("#btnInit").click(function() {
			jQuery("#idx").val('');
			jQuery("#imgName").val('');
			jQuery("#versionType").val('');
			jQuery("#deviceType").val('');
			jQuery("#availableDeviceVolume").val('');
			jQuery("#insertDate").val('');

			$("#list").resetSelection();
		});

		//delete button
		$("#btnDelete").click(function() {
			var rowid = $("#list").jqGrid('getGridParam', "selrow");
			var idx = $("#list").jqGrid('getRowData', rowid).idx;
			BackgroundChange(gridNm, rowid, "#FF0000");
			
			var setData = {
					"idx" : idx,
					"flag" : "D"
				};

			$("#list").setRowData(rowid, setData,false);
		});

		//save button
		$("#btnSave").click(function() {			
			var rowid = $("#list").jqGrid('getGridParam', "selrow");
			var rowid = $("#list").jqGrid('getGridParam', "selrow");
			var idx =$("#list").jqGrid('getRowData', rowid).idx;
			var imgName = jQuery("#imgName").val();
			var versionType = jQuery("#versionType").val();
			var deviceType = jQuery("#deviceType").val();
			var availableDeviceVolume = jQuery("#availableDeviceVolume").val();
			var deleteflag = $("#list").jqGrid('getRowData', rowid).flag;
			
			var isInsert = false;
			$("#list").setCell(rowid, 'imgName', imgName);
			$("#list").setCell(rowid, 'versionType', versionType);
			$("#list").setCell(rowid, 'deviceType', deviceType);
			$("#list").setCell(rowid, 'availableDeviceVolume', availableDeviceVolume);
			var setData = {
					"idx" : idx,
					"flag" : "U"
				};
			if(!(deleteflag =="D")){
			$("#list").setRowData(rowid, setData,false);
			}
			var gridData = jQuery("#list").getRowData();
			

			jQuery.ajax({
				type : "POST",
				url : "/img/Save.do",

				dataType : "JSON", // 옵션이므로 JSON으로 받을게 아니면 안써도 됨
				data : JSON.stringify(gridData),
				contentType : "application/json;charset=UTF-8", //한글 안깨지게

				success : function(data) {
					
					
					$("#list").trigger('reloadGrid');
				},
				error : function(xhr, status, error) {

					alert(xhr.reponseText);
				}

			});
		});
		
		
		//iframe 길이
		$("#main_Iframe", parent.document).width(1350);
		
		//center popup position
		function popWin(url,wname,w,h,ptype,t,l) {
		  var pWin, tVal, lVal;
		  if (t == "c") {
		    lVal = (screen.availWidth - w) / 2;
		    tVal = (screen.availHeight - h) / 2;
		  }
		  else {
		    tVal = (t!=null)?t:0;
		    lVal = (l!=null)?l:0;
		  }
		
		  var popOpt = "width="+w+"px,height="+h+"px, top="+tVal+", left="+lVal;
		  if(ptype == 0)
		    popOpt += ",scrollbars=no";
		
		  if(ptype == 1)
		    popOpt += ",scrollbars=yes";
		
		  if(ptype == 2)
		    popOpt += ",scrollbars=yes,resizable=yes";
		
		  pWin = window.open(url,wname,popOpt);
		//  pWin.focus();
		
		  return pWin;
		}
		
		$("#btnImgSelect").click(function() {
			popWin('imgUiPop.do', 'name', 500, 500, 0, 'c');
			
		});
		
		
		
		
		
		
	});
</script>


</head>

<div>

	 <div class="wrap-loading display-none">
    <div><img src="/resources/images/viewLoading.gif" /></div>
	</div>

	<div class="list_area">
		<table id="list"></table>
		<div id="pager"></div>
	</div>
	<div class="crud_area">
		<div class="crud_area_box">

			<table class="table_area">

				<tr>
					<td colspan="4">
					<input type="button" value="Reload" id="btnSelect" class="button">
						 <input type="button" value="Reset" id="btnInit" class="button">
						 <input type="button" value="Add" id="btnInsert" class="button add">
						 <input type="button" value="Delete" style="width:60pt;" id="btnDelete" class="button delete"></td>
				</tr>
				<tr>
					<td>idx</td>
					<td><input type="text" id="idx" readonly="readonly" class="input"></td>
					<td>imgName</td>
					<td><input type="text" id="imgName" maxlength="100" class="input">
						<input type="button" id="btnImgSelect" maxlength="100" class="button" value="Select">
					</td>
				</tr>
				
				<tr>
					<td>versionType</td>
					<td><select id="versionType">
							<option value="PAID">PAID</option>
							<option value="FREE">FREE</option>
					</select></td>
					
					<td>deviceType</td>
					<td><select id="deviceType">
							<option value="USB">USB</option>
							<option value="SSD">SSD</option>
					</select></td>
					
				</tr>
				<tr>
					<td>availableDeviceVolume</td>
					<td><select id="availableDeviceVolume" /></select></td>
				</tr>
			</table>
		</div>
		<div class="save_area">
			<input type="button" value="Save" style="width:60pt;" id="btnSave" class="button save">
		</div>

	</div>

</div>

</html>