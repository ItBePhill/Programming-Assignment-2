#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
TODO:	/  Doing
		// Done

- Create Utility functions /
	* Read Text 
	* Write Text 
	* Menu Template /
		- Potentially make title update automatically to centre with subtitle
	* Question Template /
- Create Main Functions
	* Welcome Screen
	* Calculate Pay / Income Tax



*/
class utility;
//Function Definitions ------------------------------------
void Hello(utility& util);
void Hi(utility& util);
void Welcome(utility& util);




//---------------------------------------------------------


//Classes -------------------------------------------------

//Utility functions e.g. Read, Write
class utility {
public:
	//The amount of nested menus the user is currently in;
	int nest = 0;
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
		std::string title = "What Would You Like To Do?";
		std::vector<std::string> options;
		//options will by default have a back option
		Question(std::string title = "What Would you Like to do?", std::vector<std::string> options = {"Back"}) : title(title), options(options) {
			this->title = title;
			this->options = options;
		}
		
		//Ask the question
		int ask(utility& util) {
			if (options.back() != "Back") {
				if (util.nest == 0) {
					options.push_back("Quit");
				}
				else {
					options.push_back("Back");
				}
			}
			//display title of question
			std::cout << std::endl << title << std::endl;
			//display options
			for (int i = 0; i <= options.size()-1; i++) {
				// 0 - {Answer}
				std::cout << i+1 << " - " << options[i] << std::endl;
			}

			//get user input
			std::string inputstr;
			int inputint = 0;
			//loop until we get an answer
			while (inputint == 0) {
				//get the input
				std::getline(std::cin, inputstr);
				//convert string to int and check if it is a number
				inputint = std::strtol(inputstr.c_str(), NULL, 0);
				//it isn't a number
				std::cout << inputint;
				if (inputint == 0 or inputint > options.size() or inputint < 1) {
					std::cout << "Entry is not a valid!";
					system("pause");
					break;
				}
				//it is a number
				else {
					//is the input the last element if so we want to quit or go back a menu
					if (inputint == options.size()) {
						//it is
						return 0;
					}
					else {
						return inputint;
					}
				
				}
			}
		}
	};

	/*
	Read a text file and return the result
	Options:
		filename (string) *Required* - The path to the file you want to read.
	*/
	std::string Read(std::string filename, utility& util) {
		return "";
	}

	/*
	Write to a text file and return whether it was successful
	Options:
		filename (string) *Required* - The path to the file you want to write to (will create the file if it doesn't exist).
	*/
	bool Write(std::string filename, utility& util) {
		return 0;

	}

	/*
	Create and Show the user a standardised menu, content and question are technically optional but you should probably use at least one
	Options:
		title (string) *Required* - The Title of the menu e.g. Welcome
		subtitle (string) *Optional* - a subtitle displayed under the title, could be useful for nested menus
		content (string) *Optional* - Content in the menu e.g. could be a paragraph or just a sentence
		question *Optional* - A Question to ask the user, e.g. what menu to move to

	There will always be a back option in the question so no need to add one
	*/
	int Menu(std::string title, std::string subtitle = "", std::string content = "", Question question = Question(), utility util = utility()) {
		std::cout << "============ " << title << " ============ " << nest << std::endl;
		std::cout << "-------- " << subtitle << "--------" << std::endl;
		std::cout << content;
		return question.ask(util);
	}

	
	

	
};
//---------------------------------------------------------
//Constants -----------------------------------------------


//---------------------------------------------------------


//Separate Functions -----------------------------------------------





//---------------------------------------------------------



void Hi(utility& util) {
	system("cls");
	int answer = util.Menu("Hi!", "Hi!", "Hi,Hi,Hi,Hi,Hi,Hi,Hi,Hi,Hi,Hi", { "Hi!", {"Hello"}}, util);
	switch (answer) {
	case 0:
		util.nest--;
		return;
	case 1:
		util.nest++;
		Hello(util);
	}
	Hi(util);
}
void Hello(utility& util) {
	system("cls");
	int answer = util.Menu("Hello!", "Hello!", "Hello,Hello,Hello,Hello,Hi,Hi,Hi,Hi,Hi,Hi");
	switch (answer) {
	case 0:
		util.nest--;
		return;
	}
	Hello(util);
}
void Welcome(utility& util) {
	system("cls");
	int answer = util.Menu("Welcome!", "Welcome to the payroll system", "The payroll system will contain all the information for a employee and their pay including income tax!", utility::Question("Hello World", { "Hi", "Hello"}));
	switch (answer) {
		//case 0 is back or quit
	case 0:
		util.nest--;
		return;
	case 1:
		util.nest++;
		Hi(util);
		break;
	case 2:
		util.nest++;
		Hello(util);
		break;

	}

	Welcome(util);
}
/*
The main function will call other helper functions
which will do the actual calculations and other stuff
*/
int main() {
	utility util = utility();
	Welcome(util);

}

