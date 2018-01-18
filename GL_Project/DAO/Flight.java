import java.util.Date;

class Flight{
	private String commercialId;
	private String departureAirport;
	private String arrivalAirport;
	private Date departureTime;
	private Date arrivalTime;

	String getCommercialId(){
		return commercialId;
	}
	String getDepartureAirport(){
		return departureAirport;
	}
	String getArrivalAirport(){
		return arrivalAirport;
	}
	Date getDepartureTime(){
		return departureTime;
	}
	Date getArrivalTime(){
		return arrivalTime;
	}

	private Void setArrivalAirport(String airport){
		arrivalAirport=airport;

	}
	private Void setDepartureTime(Date time){
		departureTime=time;

	}
	private Void setArrivalTime(Date time){
		arrivalTime=time;

	}


}