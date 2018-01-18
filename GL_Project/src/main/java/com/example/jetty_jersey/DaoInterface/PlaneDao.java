package com.example.jetty_jersey.DaoInterface;

import java.util.List;

import com.example.jetty_jersey.Dao.*;
public interface PlaneDao{
	
	/**
	*@return the list of planes the given type 
	**/
	List<Plane> getPlanesbyType(String type);

	/**
	*@return the plane corresponding to the given id
	**/
	Plane getPlanebyId(String id);
}