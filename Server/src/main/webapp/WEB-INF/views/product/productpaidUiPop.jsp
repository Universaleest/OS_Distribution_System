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
      $('#viewLoading').hide();
      
      $("#list").jqGrid({
         url : "/img/Inqimg.do",
         loadtext : 'Loading..',
         datatype : "json",
         mtype : "post",
         width : "480",
         height : "430",
         jsonReader : {
            repeatitems : false
         },
         colNames : [ 'idx', 'imgName','versionType','deviceType','availableDeviceVolume','insertDate', 'flag'],
         colModel : [ //data mapping and row attribute
         {
            name : 'idx',
            width : "50%"
         }, {
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
         caption : "OS File List",
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

            $("#imgName", opener.document).val(rowdata.imgName);
            $("#deviceType", opener.document).val(rowdata.deviceType);
            $("#deviceVolume", opener.document).val(rowdata.availableDeviceVolume);
            window.close();
         }
      });
            
      jQuery("#list").jqGrid('navGrid', "#pager", {
         edit : false,
         add : false,
         del : false,
         search : false,
         refresh : false
      });
      
   });
</script>

</head>
<body>
<div class="list_area">
      <table id="list"></table>
      <div id="pager"></div>
   </div>
</body>
</html>