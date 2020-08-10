package com.wayneOS.dev.service.serviceImpl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.wayneOS.dev.Model.JqGrid.JqGridData;
import com.wayneOS.dev.dao.MemberDAO;
import com.wayneOS.dev.service.MemberService;

/**
 * memberDAO를 호출
 */
@Service("MemberService")
public class MemberServiceImpl implements MemberService {

	@Autowired
	MemberDAO memberDAO;

	private final Logger logger = LoggerFactory.getLogger(getClass());
	/**
	 * insert, delete, update 명령수행
	 */
	@Override
	public void Save(List<Map> listMap) {

		for (int i = 0; i < listMap.size(); i++) {
			// memberDAO.InsertMember();
			Map<String, Object> map = listMap.get(i);
			String status = map.get("flag").toString();
			if (status.equals(JqGridData.STATUS_INSERT)) {
				memberDAO.InsertMember(map);
			}
			else if(status.equals(JqGridData.STATUS_UPDATE)){
				memberDAO.UpdateMember(map);
			}
			else if(status.equals(JqGridData.STATUS_DELETE)){
				System.out.println("D "+map.get(i));
				memberDAO.DeleteMember(map);
			}
		}

	}
	/**
	 * select 명령 수행
	 */
	@Override
	public List Inq() {

		return memberDAO.InqMember();
	}

	@Override
	public boolean Login(Map map) {
		Map resultMap = null;

		resultMap = memberDAO.SelectMember(map);

		if (resultMap != null) {
			System.out.println("Succeed to log in");
			return true;

		} else {
			System.out.println("Fail to log in");
			return false;
		}

	}

}
