package com.example.jetty_jersey.ws;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import com.example.jetty_jersey.Dao.*;

@Path("/login")
public class LoginStub {

	@GET
	@Produces(MediaType.APPLICATION_JSON)
	@Path("/{username}/{id}")
	public MRO getUser(@PathParam("username") String username,
			@PathParam("id") int id){
		return new MRO();
	}
}
