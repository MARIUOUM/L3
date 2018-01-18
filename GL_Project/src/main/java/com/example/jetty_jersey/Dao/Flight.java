package com.example.jetty_jersey.Dao;

import java.util.Date;

public class Flight{
	private String commercialId;
	private String departureAirport;
	private String arrivalAirport;
	private Date departureTime;
	private Date arrivalTime;
	public String getCommercialId() {
		return commercialId;
	}
	public void setCommercialId(String commercialId) {
		this.commercialId = commercialId;
	}
	public String getDepartureAirport() {
		return departureAirport;
	}
	public void setDepartureAirport(String departureAirport) {
		this.departureAirport = departureAirport;
	}
	public String getArrivalAirport() {
		return arrivalAirport;
	}
	public void setArrivalAirport(String arrivalAirport) {
		this.arrivalAirport = arrivalAirport;
	}
	public Date getDepartureTime() {
		return departureTime;
	}
	public void setDepartureTime(Date departureTime) {
		this.departureTime = departureTime;
	}
	public Date getArrivalTime() {
		return arrivalTime;
	}
	public void setArrivalTime(Date arrivalTime) {
		this.arrivalTime = arrivalTime;
	}

	

}