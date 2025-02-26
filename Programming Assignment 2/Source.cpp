#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>
#include <random>
using std::string;
using std::vector;
/*
Programming Assignment 2 - Payroll System - Phillip Wood
Note: Windows Only
Description of Program:
Read and Display the contents of "Pay Files"
These Files will contain information like
Employee ID, pay rate and employee name
Calculate the pay and income tax on pay and display it
Allow a user to enter a pay file and recursively show pay
for each employee in the file

employees.txt:

{ID} {Name} {rate of pay}

{month}.txt:

{ID} {hours worked}



Calculating Income Tax:
	assuming £2,000 monthly income.
	£2,000 * 12 = £24,000 (annual income)
	£24,000 - £12,570 = £11,430 (taxable income)
	£11,430 * 0.20 = £2,286 (annual income tax)
	£2,286 / 12 = £190.50 (monthly tax deduction).
*/


/*
TODO:	/  Not Finished
		// Finished

- Create Utility functions /
	* Read Text 
	* Write Text 
	* Menu Template //
		- Potentially make title update automatically to centre with subtitle //
	* Question Template //
- Create Main Functions
	* Welcome Screen //
	* Find Employee information //
	* Find specified Employee Pay for each month or a specified month
	* Read from pay file by inputting file name
	* Write employees id number and monthly pay before tax deduction to a file called '{file}_output.txt'
	* Record error in a pay file by adding it to a file called errors.txt in format {name of file} {error description}



*/

/*
Code Taken from : https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows/12642749#12642749
Only works on windows but there is a Linux version available 
*/
int getWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return columns;
}




//Function/Class Declarations -----------------------------
class utility;
class employee;
void Payment(utility& util, string content);
void Info(utility& util, string content);
void Welcome(utility& util);




//---------------------------------------------------------


//Classes -------------------------------------------------

//Employee class hold info on an employee including name ID and pay
class employee {
public:
	string name;
	string ID;
	double pay;


	employee(string name, string ID, double pay) : name(name), ID(ID), pay(pay) {
		this->name = name;
		this->ID = ID;
		this->pay = pay;
	}
};



//Utility functions e.g. Read, Write
class utility {
public:
	//The amount of nested menus the user is currently in
	int nest = 0;
	//The size of the window
	int windowSize = 119;
	int windowSizeTemp = 0;
	string answerStr;
	int answer = 0;
	//Adjust window size
	void windowSizeConfig() {
		system("cls");
		//examples for good and bad window sizes
		string goodbadbad =
R"( 
Good:                   Too Low:                   Too High:       
 ----------------        ----------------         ----------------   
|=====title======|      |===title===     |       |=====title======|
|----subtitle----|      |--subtitle--    |       |=               |
|                |      |                |       |----subtitle----|
|                |      |                |       |-               |
 ----------------        ----------------         ----------------
)";
		int answer = this->Menu("Window Size", "Configure the Window Size, Current Window Size: " + std::to_string(this->windowSize), "Adjust Window Size\nExamples:\n"+goodbadbad, {"What Would you Like to do ? ", {"Edit", "Auto"}});
		switch (answer) {
		case 0:
			return;
			break;
		case 1:
			system("cls");
			std::cout << "Enter a Number above 0 (decimals will be rounded up), Current Window Size: " << std::to_string(this->windowSize) << "\n- ";
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
			std::cout << "Setting Window Size from: " << this->windowSize << "to: " << answer;
			windowSizeTemp = answer;
			break;
		case 2:
			std::cout << "Automatically finding the Window Size...\n";
			windowSizeTemp = getWidth();
			std::cout << "Found size {" << windowSizeTemp << "}\n";
			system("pause");
			break;
		} 
		this->windowSize = windowSizeTemp;
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
		string title = "What Would You Like To Do?";
		vector<string> options;
		//options will by default have a back option
		Question(string title = "What Would you Like to do?", vector<string> options = { "Back" }) : title(title), options(options) {
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
			string inputstr;
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
					//it's not
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
	Read one a text file and return the result, this function does no parsing and will just return what is exactly in the file
	Options:
		filename (string) *Required* - The path to the file you want to read.
	*/
	string Read(string filename) {
		// read a whole line, including the white space
		//create variable for the file we are reading
		std::fstream infile;
		//and for the string to write the result to
		string str;
		//write to this and then add to the actual return string
		string buffer;
		//open the file
		infile.open(filename);
		//get the first line in the file
		while (!infile.eof()) {
			std::getline(infile, buffer);
			str += "\n" + buffer;
		}
		//close the file
		infile.close();
		return str;
	}
	/*
	Write to a text file and return whether it was successful
	Options:
		filename (string) *Required* - The path to the file you want to write to (will create the file if it doesn't exist).
	*/
	bool Write(string filename) {
		return 0;

	}
	//Create a divider the size of the window with "-" as the filler
	string CreateDivider(int size = -1, char divider = '-') {
		utility util = *this;
		if (size == -1) {
			size = util.windowSize;
		}
		std::stringstream ss;
		ss << std::setfill(divider) << std::setw(size) << "-";
		return ss.str();
	}

	/*
	Create and Show the user a standardised menu
	Options:
		title (string) *Required* - The Title of the menu e.g. Welcome
		subtitle (string) *Optional* - a subtitle displayed under the title, could be useful for nested menus
		content (string) *Optional* - Content in the menu e.g. could be a paragraph or just a sentence, won't default but isn't required for menu to function as a back answer is added automatically
		question *Optional* - A Question to ask the user, e.g. what menu to move to, won't default but isn't required for menu to function as a back answer is added automatically
	There will always be a back option in the question so no need to add one
	*/

	
	int Menu(string title, string subtitle, string content, Question question) {
		utility util = *this;
		//The size of the window
		//the difference between the size of the window and the size of the title in characters
		const int windowSizeDiffT = util.windowSize - title.size();
		const int windowSizeDiffS = util.windowSize - subtitle.size();

		//Create Menu
		//Display Title / create menu headers
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffT - 2) / 2)) << " " << title << " ";
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffT - 1) / 2 )) << "=";
		std::cout << "\n";
		std::cout << std::setfill('-') << std::setw(ceil((windowSizeDiffS - 2) / 2)) << " " << subtitle << " ";
		std::cout << std::setfill('-') << std::setw(ceil((windowSizeDiffS - 1) / 2)) << "-";
		//show content
		if (content != "") {
			std::cout << "\n" << CreateDivider() << "\n";
			std::cout << content;
			std::cout << "\n" << CreateDivider() << "\n";
		}
		else {
			std::cout << "\n" << CreateDivider() << "\n";
		}
		//return the question
		return question.ask(util);
	}

	
	
	
};
//---------------------------------------------------------
//Constants -----------------------------------------------


//---------------------------------------------------------


//Separate Functions -----------------------------------------------





//---------------------------------------------------------
//Get a list of employees
vector<employee> getEmployees(utility& util) {
	/*
	format of original output:
	"
	ID Name Pay\n
	ID Name Pay
	"
	*/
	string output = util.Read("employees.txt");
	/*
	Format of middle output:
	{"ID Name Pay"},
	{"ID Name Pay"}
	*/
	vector<string> outputarrtemp;
	/*
	Format of final output:
	{ {ID}, {Name}, {Pay} },
	{ {ID}, {Name}, {Pay} }
	*/
	vector<employee> outputarr;
	
	//parse the output
	//split the output into an array of three indexes for ID, name and pay rate
	string token;
	vector<string> outputarrcomp;
	//split output string into each employee
	std::istringstream ss(output);
	while (std::getline(ss, token, '\n')) {
		outputarrtemp.push_back(token);
	}

	//split output strings in array into each component
	//for each string in the array split into into each component
	for (string i : outputarrtemp) {
		std::istringstream ss(i);
		while (std::getline(ss, token, ' ')) {
			//add each component
			outputarrcomp.push_back(token);
		}
		std::cout << outputarrcomp.size();
		if (outputarrcomp.size() >= 3) {
			outputarr.push_back(employee(outputarrcomp[1], outputarrcomp[0], strtod(outputarrcomp[2].c_str(), NULL)));
		}
		outputarrcomp.clear();
	}
	return outputarr;
}



//get a single employees payment info
string ViewSinglePayment(utility& util, string ID) {
	std::stringstream returnout;
	employee chosen("", "", 0.0);
	vector<employee> employees = getEmployees(util);
	returnout << "View Employee {" << ID << "}";
	return returnout.str();
}

//return a list of employees information including employee id, name and rate of pay
string ViewAllPayment(utility& util, string Month) {

	vector<employee> employees = getEmployees(util);
	std::stringstream returnout;
	returnout << "View Month {" << Month << "}";
	return returnout.str();
}
//return a list of employees information including employee id, name and rate of pay
string ViewPaymentFile(utility& util, string filename) {
	vector<employee> employees = getEmployees(util);
	std::stringstream returnout;
	returnout << "View File {" << filename << "}";
	return returnout.str();
}


//return a list of employees information including employee id, name and rate of pay
string ViewAllInfo(utility& util) {
	
	vector<employee> employees = getEmployees(util);
	std::stringstream returnout;
	//Create table
	returnout << "\n\n";
	//values were chosen semi-randomly so if they don't make any sense that's why
	returnout << std::left;
	returnout << std::setw(9) << "[ID]" << std::setw(15) << "[Name]" << std::setw(9) << "[Rate of Pay]\n";
	returnout << util.CreateDivider(util.windowSize/3) << "\n";
	for (auto i : employees) {
		returnout << std::setw(9) << i.ID << std::setw(15) << i.name << std::setw(9) << std::setprecision(4) << i.pay << "\n" << util.CreateDivider(util.windowSize/3) << "\n";
	}
	return returnout.str();
}


//get a single employees information
string ViewSingleInfo(utility &util, string ID) {
	std::stringstream returnout;
	employee chosen("","",0.0);
	//get all employees
	vector<employee> employees = getEmployees(util);
	for (auto i : employees) {
		if (i.ID == ID or i.name == ID) {
			chosen = i;
		}
	}
	//employee has no name so must not exist
	if (chosen.name == "") {
		return "Chosen employee doesn't exist!";
	}
	returnout << std::left;
	returnout << std::setw(9) << "[ID]" << std::setw(15) << "[Name]" << std::setw(9) << "[Rate of Pay]\n";
	returnout << util.CreateDivider(util.windowSize / 3) << "\n";
	returnout << std::setw(9) << chosen.ID << std::setw(15) << chosen.name << std::setw(9) << std::setprecision(4) << chosen.pay << "\n" << util.CreateDivider(util.windowSize / 3) << "\n";
	return returnout.str();
}


void Payment(utility& util, string content) {
	system("cls");
	int answer = util.Menu("Payment Information", "View Payment Information", content, { "What Would You Like to do?", {"View Payment Info for an Employee", "View all information for a month", "View information from a file"} });
	switch (answer) {
	case 0:
		util.nest--;
		return;
	case 1:
		std::cout << "Employee";
		content = ViewSinglePayment(util, "employee");
		break;
	case 2:
		std::cout << "Month";
		content = ViewAllPayment(util, "month");
		break;
	case 3:
		std::cout << "File";
		content = ViewPaymentFile(util, "file");
		break;
	
	}
	Payment(util, content);
}
void Info(utility& util, string content) {
	system("cls");
	int answer = util.Menu("Employee Information", "View employee information", content, { "What Would You Like To Do", {"View all", "View a specific Employee"} });
	switch(answer){
	case 0:
		util.nest--;
		return;
	//View All
	case 1:
		util.nest++;
		content = ViewAllInfo(util);
		break;
	case 2:
		util.nest++;
		string id;
		std::cout << "Please input the ID or name of the employee (case sensitive)\n- ";
		std::getline(std::cin, id);
		content = ViewSingleInfo(util, id);

	}
	Info(util, content);
}

void config(utility& util) {
	system("cls");
	int answer = util.Menu("Config", "Configure program settings", "", {"What Would you Like to do?", {"Window Size"}});
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
	std::vector <std::string> messages = {"Live Long and Prosper!", "Hey you you're finally awake!", "Keep the change, ya filthy animal!!", "Also try Minecraft!!", "Han didn't shoot first!!", "I guess you guys aren't ready for that yet. But your kids are gonna love it!", "When you get to Hell, Tell 'em Viper sent you!", "If my grandmother had wheels she would have been a bike!", "Well excuse me, princess!", "Also try Terraria!", "Not on Steam!", "As seen on TV!"};
	//Taken from: 
	//https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
	//https://www.geeksforgeeks.org/how-to-generate-random-number-in-range-in-cpp/
	//and my previous assignment
	//Generates a random number to be used to show a splash message
	
	int min = 0;
	int max = messages.size()-1;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);
	//---------------------------------------------------------
	string splash = "{splash}\n" + util.CreateDivider();
	string content = splash + "The payroll system will contain all the information for an employee and their pay including income tax";
	//replace {splash} with a random message chosen by random number generator above
	int randomint = distrib(gen);

	content.replace(content.find("{splash}"), sizeof("{splash}") - 1, messages[randomint]);

	
	
	int answer = util.Menu("Welcome!", "Welcome to the payroll system!", content, { "What Would You Like To Do?", { "View Payment Information", "View Employee Information", "Configure"}});
	switch (answer) {
		//case 0 is back or quit
	case 0:
		util.nest--;
		return;
	case 1:
		util.nest++;
		Payment(util, "Information will appear here");
		break;
	case 2:
		util.nest++;
		Info(util, "Information will appear here");
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
	util.windowSize = getWidth();
	Welcome(util);

}

