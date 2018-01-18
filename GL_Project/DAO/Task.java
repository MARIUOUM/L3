import java.util.Date;

class Task{
	private Date startTime;
	private Date endTime;
	private String description;

	Date getStartTime(){
		return startTime;

	}

	Date getEndTime(){
		return endTime;
	}

	String description(){
		return description;
	}

	Date setStartTime(int year,int month,int date,int hrs,int min){
		startTime=new date(year,month,date,hrs,min);
	}

	Date getEndTime(int year,int month,int date,int hrs,int min){
		endTime=new date(year,month,date,hrs,min);
	}

}
