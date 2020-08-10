package com.wayneOS.dev.Model.JqGrid;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

import com.wayneOS.dev.Model.ObjectMapper.CustomObjectMapper;
/**
 * 웹 화면에 표시되는 표(Grid) 데이터 전체페이지, 페이지당 표시 데이터 개수, 현재 페이지 데이터를 담음
 */

public class JqGridData {

	/** Total number of pages */
	private int total;
	/** The current page number */
	private int page;
	/** Total number of records */
	private int records;
	/** The actual data */
	private List<Map<String,Object>> rows;
	public static final String STATUS_INSERT = "I";
	public static final String STATUS_UPDATE = "U";
	public static final String STATUS_DELETE = "D";
	
	public JqGridData()
	{
		
	}
	
	
	public JqGridData(Map<String,Object> paramMap,List<Map<String,Object>> rows){
		
		int page = Integer.parseInt(paramMap.get("page").toString());
		int pageRow = Integer.parseInt(paramMap.get("rows").toString());
		
		
		int total = rows.size() / pageRow;
		if(rows.size() - total * pageRow >0){
			total++;
		}
		System.out.println("total :" + total);
		System.out.println("page : " + page);
		System.out.println("pageRow : " + pageRow );
		System.out.println("records Size: " + rows.size());
		this.total = total;
		this.page = page;
		this.records = rows.size();
		
		if(records < pageRow){
			this.rows = rows;
		}
		else if( page *pageRow < records){
			this.rows =rows.subList( (page-1) * pageRow, page *pageRow);
		}
		else{
			this.rows =rows.subList( (page-1) * pageRow, records);
		}
		
		/*
		if(pageRow * pageRow + pageRow < records){
			this.rows = rows.subList( (page-1) * pageRow, page * pageRow + pageRow);
		}
		
		else{
			if(records == 1){
				this.rows = rows.subList( (page-1)* pageRow, records);
			}else{
				this.rows = rows.subList( (page-1)* pageRow, records -1);
				
			}
		}
		*/
		
	}

	

	public int getTotal() {
		return total;
	}
	public void setTotal(int total) {
		this.total = total;
	}
	public int getPage() {
		return page;
	}
	public void setPage(int page) {
		this.page = page;
	}
	public int getRecords() {
		return records;
	}
	public void setRecords(int records) {
		this.records = records;
	}
	public List<Map<String, Object>> getRows() {
		return rows;
	}
	public void setRows(List<Map<String, Object>> rows) {
		this.rows = rows;
	}
	public String getJsonString() {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("page", page);
		map.put("total", total);
		map.put("records", records);
		map.put("rows", rows);

		CustomObjectMapper mapper = new CustomObjectMapper();
		
		try {
			return mapper.writeValueAsString(map);
			
			
		} catch (JsonGenerationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (JsonMappingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return null;

	}
}