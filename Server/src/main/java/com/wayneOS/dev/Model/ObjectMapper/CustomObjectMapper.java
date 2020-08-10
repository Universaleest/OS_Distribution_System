package com.wayneOS.dev.Model.ObjectMapper;

import java.text.SimpleDateFormat;

import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.SerializationConfig.Feature;
/**
 * 현재 날짜와 시간을 구한다
 */
public class CustomObjectMapper extends ObjectMapper {
    public CustomObjectMapper() {
        super();
        configure(Feature.WRITE_DATES_AS_TIMESTAMPS, false);
        setDateFormat(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss"));
    }
}