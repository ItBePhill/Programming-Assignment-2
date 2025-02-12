#include <iostream>
#include <fstream>
#include <iomanip>
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
	* Menu Template //
		- Potentially make title update automatically to centre with subtitle
	* Question Template //
- Create Main Functions
	* Welcome Screen
	* Calculate Pay / Income Tax



*/
class utility;
//Function Definitions ------------------------------------
void Payment(utility& util);
void Info(utility& util);
void Welcome(utility& util);




//---------------------------------------------------------


//Classes -------------------------------------------------

//Utility functions e.g. Read, Write
class utility {
public:
	//The amount of nested menus the user is currently in;
	int nest = 0;
	//The size of the window
	int windowSize = 119;
	//Adjust window size
	void windowSizeConfig() {
		system("cls");
		int answer = Menu("Window Size", "Configure the Window Size, Current Window Size: " + std::to_string(this->windowSize), "Adjust Window Size until the title reaches the corner of the screen", {"What Would you Like to do?", {"Edit"}}, *this);
		switch (answer) {
		case 0:
			return;

		case 1:
			system("cls");
			std::cout << "Enter a Number above 0 (decimals will be rounded up), Current Window Size: " << std::to_string(this->windowSize) << "\n- ";
			std::string answerStr;
			int answer = 0;
			while (answer == 0) {
				std::getline(std::cin, answerStr);
				answer = strtol(answerStr.c_str(), NULL, 0);
				//user either inputted an incorrect number or a letter
				if (answer <= 0) {
					std::cout << "Invalid input: Try Again";
					answer = 0;
					continue;
				}
				
			}
			std::cout << "Window Size was changed from: " << this->windowSize << "to: " << answer;
			this->windowSize = answer;
			break;
		} 
		windowSizeConfig();
	}
	/*
	little weird having a class nested in a class but it's cleaner than having it separate so oh well
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
		Question(std::string title = "What Would you Like to do?", std::vector<std::string> options = { "Back" }) : title(title), options(options) {
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
			for (int i = 0; i <= options.size() - 1; i++) {
				// 0 - {Answer}
				std::cout << i + 1 << " - " << options[i] << std::endl;
			}

			//get user input
			std::string inputstr;
			int inputint = 0;
			//loop until we get an answer
			while (inputint == 0) {
				//get the input
				std::cout << "- ";
				std::getline(std::cin, inputstr);
				//convert string to int and check if it is a number
				inputint = std::strtol(inputstr.c_str(), NULL, 0);
				//it isn't a number
				if (inputint == 0 or inputint > options.size() or inputint < 1) {
					std::cout << std::endl << "Entry is not a valid!\n";
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
				return -1;
			}
			return -1;
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
	//Create a divider with "-" as the filler
	void CreateDivider(utility& util) {
		std::cout << std::endl << std::setfill('-') << std::setw(util.windowSize - 1) << "-" << std::endl;
	}

	/*
	Create and Show the user a standardised menu, content and question are technically optional but you should probably use at least one
	Options:
		title (string) *Required* - The Title of the menu e.g. Welcome
		subtitle (string) *Optional* - a subtitle displayed under the title, could be useful for nested menus
		content (string) *Optional* - Content in the menu e.g. could be a paragraph or just a sentence
		question *Optional* - A Question to ask the user, e.g. what menu to move to
		utility is technically optional, but in reality it is not and you need to pass utility in.
	There will always be a back option in the question so no need to add one
	*/

	
	int Menu(std::string title, std::string subtitle = "", std::string content = "", Question question = Question(), utility util = utility()) {
		//The size of the window (there is a way to get this automatically but it's complex and platform dependent)
		//the difference between the size of the window and the size of the title in characters
		const int windowSizeDiffT = util.windowSize - title.size();
		const int windowSizeDiffS = util.windowSize - subtitle.size();
	/*	std::cout << title.size();
		std::cout << subtitle.size();*/

		//Create Menu
		//Display Title
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffT - 2) / 2)) << " " << title << " ";
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffT - 1) / 2 )) << "=";
		std::cout << "\n";
		std::cout << std::setfill('-') << std::setw(ceil((windowSizeDiffS - 2) / 2)) << " " << subtitle << " ";
		std::cout << std::setfill('-') << std::setw(ceil((windowSizeDiffS - 1) / 2)) << "-";
		if (content != "") {
			CreateDivider(util);
			std::cout << content;
			CreateDivider(util);
		}
		else {
			CreateDivider(util);
		}
		return question.ask(util);
	}

	
	
	
};
//---------------------------------------------------------
//Constants -----------------------------------------------


//---------------------------------------------------------


//Separate Functions -----------------------------------------------





//---------------------------------------------------------



void Payment(utility& util) {
	system("cls");
	int answer = util.Menu("Payment Information", "View Payment Information", "", utility::Question(), util);
	switch (answer) {
	case 0:
		util.nest--;
		return;
	case 1:
		util.nest++;
		Payment(util);
	}
	Info(util);
}
void Info(utility& util) {
	system("cls");
	int answer = util.Menu("Employee Information", "View employee information", "", utility::Question(), util);
	switch (answer) {
	case 0:
		util.nest--;
		return;
	}
	Payment(util);
}

void config(utility& util) {
	system("cls");
	int answer = util.Menu("Config", "Configure program settings", "", {"What Would you Like to do?", {"Window Size"}}, util);
	switch (answer) {
	case 0:
		util.nest--;
		return;

	case 1:
		util.nest++;
		util.windowSizeConfig();

	}
	config(util);
}
void Welcome(utility& util) {
	system("cls");
	int answer = util.Menu("Welcome!", "Welcome to the payroll system!", "The payroll system will contain all the information for an employee and their pay including income tax", { "What Would You Like To Do?", { "View Payment Information", "View Employee Information", "Configure"}}, util);
	switch (answer) {
		//case 0 is back or quit
	case 0:
		util.nest--;
		return;
	case 1:
		util.nest++;
		Payment(util);
		break;
	case 2:
		util.nest++;
		Info(util);
		break;
	case 3:
		util.nest++;
		config(util);
		break;

	}

	Welcome(util);
}
/*
The main function will call other helper functions
which will do the actual calculations and other stuff
*/
int main() {
	std::cout << "- WARNING -\nResizing Window smaller than default may cause menus to break\n";
	system("pause");
	std::cout << "\n";
	utility util = utility();
	Welcome(util);

}

