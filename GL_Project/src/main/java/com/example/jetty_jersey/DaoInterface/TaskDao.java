package com.example.jetty_jersey.DaoInterface;

import java.util.List;

import com.example.jetty_jersey.Dao.*;
public interface TaskDao{
	

	List<Task> getTaskbyDay(String dayMonthYear);
	List<Task> getTaskbyMonth(String monthYear);
	List<Task> getTaskbyHour(String hourDaymonth);
	List<Task> getTaskbyYear(String year);
	/**
	*@return the list of Tasks that start the given day
	**/
	List<Task> getTaskbyStartDay(String dayMonthYear);
	/**
	*@return the list of Tasks that start the given month
	**/
	List<Task> getTaskbyStartMonth(String monthYear);
	/**
	*@return the list of Tasks that start the given day at the corresponding hour
	**/
	List<Task> getTaskbyStartHour(String hourDaymonth);
	/**
	*@return the list of Tasks that start the given year
	**/
	List<Task> getTaskbyStartYear(String year);
	/**
	*@return the list of Tasks that end the given day
	**/
	List<Task> getTaskbyEndDay(String dayMonthYear);
	/**
	*@return the list of Tasks that end the given month
	**/
	List<Task> getTaskbyEndMonth(String monthYear);
	/**
	*@return the list of Tasks that end the given day at the corresponding hour
	**/
	List<Task> getTaskbyEndHour(String hourDaymonth);
	/**
	*@return the list of Tasks that end the given year
	**/
	List<Task> getTaskbyEndYear(String year);


}