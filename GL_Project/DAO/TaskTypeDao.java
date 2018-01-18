public interface TaskTypeDao{
	
	/**
	*@return the list of TaskTypes that need or not to be done in a hangar
	**/
	List<TaskType> getTaskTypesbyHangerNeed(Boolean hangarNeed);
	/**
	*@return the list of TaskTypes corresponding to given ataCategory
	**/
	List<TaskType> getTaskTypesbyAtaCategory(String ataCategory);
	/**
	*@return the list of TaskTypes done in the given amount of time months:days:hours:minutes
	**/
	List<TaskType> getTaskTypesbyPeriodicityEquals(String period);
	
	/**
	*@return the list of TaskTypes done in less amount of time that the given one months:days:hours:minutes
	**/
	List<TaskType> getTaskTypesbyPeriodicityLessThan(String period);
	/**
	*@return the list of TaskTypes done in more given amount of time that the given one months:days:hours:minutes
	**/
	List<TaskType> getTaskTypesbyPeriodicityGreaterThan(String period);
}