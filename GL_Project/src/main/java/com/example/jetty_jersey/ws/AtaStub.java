package com.example.jetty_jersey.ws;


import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

@Path("/ata")
public class AtaStub {
	
	@GET
	@Produces(MediaType.APPLICATION_JSON)
	@Path("/view/{id}")
	public String getAtaById(@PathParam("id") int id){
		return new String();
	}

}
