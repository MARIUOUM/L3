public interface MroDao{
	/**
	*@return the list of mro members with the given qualification
	**/
	public List<MRO> getMrobyQualification(String qualification);
}