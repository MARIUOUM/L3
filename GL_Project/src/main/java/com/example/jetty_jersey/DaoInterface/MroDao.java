package com.example.jetty_jersey.DaoInterface;

import java.util.List;

import com.example.jetty_jersey.Dao.*;
public interface MroDao{
	/**
	*@return the list of mro members with the given qualification
	**/
	public List<MRO> getMrobyQualification(String qualification);
}