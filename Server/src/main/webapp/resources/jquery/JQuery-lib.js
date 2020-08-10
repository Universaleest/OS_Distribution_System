/**
 * 
 */

function BackgroundChange(gridNm, rowid, color) {
	$("#" + gridNm).setRowData(rowid, false, {
		background : color
	});
}
