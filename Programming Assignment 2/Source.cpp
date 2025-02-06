#include <iostream>
#include <fstream>

/*
Programming Assignment 2 - Payroll System - Phillip Wood
Description of Program:
Read and Display the contents of "Pay Files"
These Files will contain information like
Employee ID, pay rate and employee name
Calculate the pay and income tax on pay and display it
Allow a user to enter a pay file and recursively show pay
for each employee in the file

Calculating Income Tax:
	assuming £2,000 monthly income.
	£2,000 * 12 = £24,000 (annual income)
	£24,000 - £12,570 = £11,430 (taxable income)
	£11,430 * 0.20 = £2,286 (annual income tax)
	£2,286 / 12 = £190.50 (monthly tax deduction).
*/


/*
TODO:	/  Done
		// Doing

- Create Utility functions /
	* Read Text /
	* Write Text /
	* Menu Template /
	* Question Template /
- Create Main Functions
	* Welcome Screen
	* Calculate Pay / Income Tax
*/


//Function Definitions ------------------------------------





//---------------------------------------------------------

//Separate Functions -----------------------------------------------





//---------------------------------------------------------

//Classes -------------------------------------------------

//Utility functions e.g. Read, Write
class util {
public:

	/*
	little weird having a class nested in a class but it's cleaner than having it separate so o well
	Will store a question e.g.
	1 - Add Credits
	2 - Create Order 


	Options:
		Title (string) *Optional* Default: "What would you like to do?" - The Question to ask the user.
		Options (Vector of strings) *Required* - The options for the user to choose from.
	*/
	class Question {
	public:
		//Ask the question
		void ask() {
			return;
		}
	};

	/*
	Read a text file and return the result
	Options:
		filename (string) *Required* - The path to the file you want to read.
	*/
	std::string Read(std::string filename) {
		return "";
	}

	/*
	Write to a text file and return whether it was successful
	Options:
		filename (string) *Required* - The path to the file you want to write to (will create the file if it doesn't exist).
	*/
	bool Write(std::string filename) {
		return 0;

	}

	/*
	Create and Show the user a standardised menu
	Options:
		title (string) *Required* - The Title of the menu e.g. Welcome
		subtitle (string) *Optional* - a subtitle displayed under the title, could be useful for nested menus
		description (string) *Optional* - a description displayed in the menu, could tell information about the menu.
	*/
	void Menu(std::string title, std::string subtitle = "", std::string description = "", Question question = Question()) {
		return;
	}
	
	

	
};




//---------------------------------------------------------

//Constants -----------------------------------------------





//---------------------------------------------------------

/*
The main function will call other helper functions
which will do the actual calculations and other stuff
*/
int main() {
	
}