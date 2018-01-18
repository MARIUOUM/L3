package com.example.jetty_jersey.ws;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import com.example.jetty_jersey.Dao.*;

@Path("/Flight")
public class FlightStub {
	
	@GET
	@Produces(MediaType.APPLICATION_JSON)
	@Path("/view/{id}")
	public Flight flightById(@PathParam("id") int id){
		return new Flight();
	}
	

}
