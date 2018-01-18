package com.example.jetty_jersey.DaoInterface;

import java.util.List;

import com.example.jetty_jersey.Dao.*;
public interface TaskListDao{
	
	/**
	*@return the list of TaskTypes that need or not to be done in a hangar
	**/
	List<TaskList> getTaskTypesbyHangerNeed(Boolean hangarNeed);
	/**
	*@return the list of TaskTypes corresponding to given ataCategory
	**/
	List<TaskList> getTaskTypesbyAtaCategory(String ataCategory);
	/**
	*@return the list of TaskTypes done in the given amount of time months:days:hours:minutes
	**/
	List<TaskList> getTaskTypesbyPeriodicityEquals(String period);
	
	/**
	*@return the list of TaskTypes done in less amount of time that the given one months:days:hours:minutes
	**/
	List<TaskList> getTaskTypesbyPeriodicityLessThan(String period);
	/**
	*@return the list of TaskTypes done in more given amount of time that the given one months:days:hours:minutes
	**/
	List<TaskList> getTaskTypesbyPeriodicityGreaterThan(String period);
}