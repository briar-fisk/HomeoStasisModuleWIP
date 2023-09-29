
//The DNA class holds a string of u_Data_3 vars. The function mutate() can be called to cause a mutation.
class c_DNA
{
	//The length of the DNA string.
     int Length;
	
	//The acutal DNA string.
	u_Data_3 * DNA;
	
	//The high setting for DNA mutation ranges.
	u_Data_3 * Mutation_Upper;
	
	//The low settings for DNA mutation ranges.
	u_Data_3 * Mutation_Lower;
};