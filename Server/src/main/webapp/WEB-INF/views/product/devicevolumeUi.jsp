<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%> 
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<link rel="stylesheet" type="text/css" media="screen" href="/resources/jquery/jquery-ui.min.css" />
<link rel="stylesheet" type="text/css" media="screen" href="/resources/jquery/ui.jqgrid.css" />
<link rel="stylesheet" type="text/css" media="screen" href="/resources/css/style.css" />
<link rel="stylesheet" type="text/css" media="screen" href="/resources/css/button.css" />
<!-- <script src="/resources/jquery/grid.locale-kr.js" type="text/javascript"></script> -->

<script src="/resources/jquery/jquery-1.11.2.min.js" type="text/javascript"></script>
<script type="text/javascript" src="/resources/jquery/jquery.blockUI.js"></script>
<script src="/resources/jquery/jquery-migrate-1.2.1.js" type="text/javascript"></script>
<script src="/resources/jquery/grid.locale-en.js" type="text/javascript"></script>
<script src="/resources/jquery/jquery.jqGrid.src.js" type="text/javascript"></script>
<script src="/resources/jquery/JQuery-lib.js" type="text/javascript"></script>

<script type="text/javascript">
	$(document).ready(function() {
		$('#viewLoading').hide();
		
		$("#list").jqGrid({
			url : "/devicevolume/Inqdevicevolume.do",
			loadtext : 'Loading..',
			datatype : "json",
			mtype : "post",
			width : "650",
			height : "700",
			jsonReader : {
				repeatitems : false
			},
			colNames : [ 'idx', 'volumeSize', 'flag'],
			colModel : [ //data mapping and row attribute
			{
				name : 'idx',
				width : "50%"
			}, {
				name : 'volumeSize',
				width : "50%"
			}, {name : 'flag',
				hidden : true 
				
			} ],

			rowList : [ 10, 20, 30 ],
			caption : "Device Management",
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
				jQuery("#volumeSize").val(rowdata.volumeSize);
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

			var total = $("#list").getGridParam("records") + 1;
			var volumeSize = jQuery("#volumeSize").val();
			

			var addData = {
				"idx" : total,
				"volumeSize" : volumeSize,
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
			jQuery("#volumeSize").val('');
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
			var volumeSize = jQuery("#volumeSize").val();
			var isInsert = false;
			var idx = $("#list").jqGrid('getRowData', rowid).idx;
			var deleteflag = $("#list").jqGrid('getRowData', rowid).flag;
			
			$("#list").setCell(rowid, 'volumeSize', volumeSize);
			var setData = {
					"idx" : idx,
					"flag" : "U"
				};
			if(!(deleteflag =="D")){
			$("#list").setRowData(rowid, setData,false);
			}
			var gridData = jQuery("#list").getRowData();
			
			$.blockUI();
			$('#viewLoading').fadeIn(500);
			
			jQuery.ajax({
		
				
				type : "POST",
				url : "/devicevolume/Save.do",

				dataType : "JSON", // �ɼ��̹Ƿ� JSON���� ������ �ƴϸ� �Ƚᵵ ��
				data : JSON.stringify(gridData),
				contentType : "application/json;charset=UTF-8", //�ѱ� �ȱ�����

				success : function(data) {
					$('#viewLoading').fadeOut(500);
					$.unblockUI();
					$("#list").trigger('reloadGrid');
				},
				error : function(xhr, status, error) {

					alert(xhr.reponseText);
				}

			});

		});
		//table height change
		$("#crud_area_box").height(200);
		$("#table_area").height(200);
		
		$("#main_Iframe", parent.document).width(1100);
	});
</script>

</head>
<body>
	<div class="list_area">
		<table id="list"></table>
		<div id="pager"></div>
	</div>

	<div class="crud_area">
		<div class="crud_area_box" id="crud_area_box">
		<table class="table_area" id="table_area">
				<tr class="center">
					<td><input type="button" value="Reload" id="btnSelect"
						class="button"></td>
					<td><input type="button" value="Reset" id="btnInit"
						class="button"></td>
					<td><input type="button" value="Add" id="btnInsert"
						class="button add"></td>
					<td><input type="button" value="Delete" style="width:60pt;" id="btnDelete"
						class="button delete"></td>
				</tr>
				<tr>
					<td colspan="2"><b>idx</b> <input type="text" id="idx" readonly="readonly"
						maxlength="5" class="input"></td>
					<td colspan="2"><b>volumeSize</b><input type="text" id="volumeSize" maxlength="15"
						class="input"></td>
				</tr>
			</table>
		</div>
		
		<div>
			
			<div class="save_area">
				<input type="button" value="Save" style="width:60pt;" id="btnSave" class="button save">
			</div>
		</div>
	</div>
	
	<div id="viewLoading">
		<img src="/resources/images/viewLoading.gif" />
	</div>
</body>
</html>