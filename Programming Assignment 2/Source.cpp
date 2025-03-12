#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>
#include <random>
#include <filesystem>

//tabulate library: https://github.com/p-ranav/tabulate/releases/tag/v1.5
//created by: Pranav
#include "tabulate.hpp"


using std::string;
using std::vector;
using std::to_string;
using std::endl;
/*
Programming Assignment 2 - Payroll System - Phillip Wood
Note: Windows Only
Description of Program:
Read and Display the contents of "Pay Files"
These Files will contain information like
Employee ID, pay rate and Employee name
Calculate the pay and income tax on pay and display it
Allow a user to enter a pay file and recursively show pay
for each Employee in the file

employees.txt:

{ID} {Name} {rate of pay}

{month}.txt:

{ID} {hours worked}


You must multiply your monthly income by twelve to establish the annual income. If this is less than £12570, then tax should not be deducted,
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

- Bugs
	* Edit Window Size doesn't clear console //

- Create Utility functions /
	* Read Text //
	* Write Text 
	* Menu Template //
		- Potentially make mTitle update automatically to centre with subtitle //
	* Question Template //
- Create Main Functions
	* Welcome Screen //
	* Find Employee information //
	* Find specified Employee Pay for each month /
	* Read from pay file by inputting file name /
	* Write employees id number and monthly pay before tax deduction to a file called '{month_file}_output.txt' /
	* Record error in a pay file by adding it to a file called errors.txt in format {name of file} {error description}



*/

/*
Code Taken from : https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows/12642749#12642749
Only works on windows but there is a Linux version available 
*/
static int getWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return columns;
}




//Function/Class Declarations -----------------------------
class Helper;
struct Employee;
void Payment(Helper& helper, string content);
void Info(Helper& helper, string content);
void Welcome(Helper& helper);
vector<Employee> GetEmployees(Helper& helper);
string ViewSingleInfo(Helper& helper, Employee employee);




//---------------------------------------------------------


//Classes -------------------------------------------------

//Employee struct holds info on an Employee including name, ID and pay
struct Employee {
	public:
		//name of the Employee
		string name;
		//ID of the Employee
		string ID;
		//pay rate of the Employee
		double pay;
};



//Utility functions e.g. Read, Write
class Helper {
private:
	//size of the console window, used to create dividers and menus
	size_t mWindowSize = 120;
	//the current amount of menus the user is in
	int mNest = 0;
public:
	size_t getWindowSize() {
		return this->mWindowSize;
	}
	void setWindowSizeAuto() {
		this->mWindowSize = getWidth();
	}
	void IncrementNest() {
		this->mNest++;
	}
	void DecrementNest() {
		this->mNest--;
	}
	//Adjust window size
	void windowSizeConfig() {
		system("cls");
		size_t windowSizeTemp = 0;
		string answerStr;
		int answer = 0;
		//examples for good and bad window sizes
		string goodbadbad =
R"( 
Good:                   Too Low:                   Too High:       
 ----------------        ----------------         ----------------   
|=====Title======|      |===Title===     |       |=====Title======|
|----subtitle----|      |--subtitle--    |       |=               |
|                |      |                |       |----subtitle----|
|                |      |                |       |-               |
 ----------------        ----------------         ----------------
)";
		answer = this->Menu("Window Size", "Configure the Window Size, Current Window Size: " + to_string(this->mWindowSize), "Adjust Window Size\nExamples:\n" + goodbadbad, { "What Would you Like to do ? ", {"Edit", "Auto"} });
		switch (answer) {
		case 0:
			return;
			system("cls");
			break;
		case 1:
			system("cls");
			std::cout << "Enter a Whole Number above 0, Current Window Size: " << to_string(this->mWindowSize) << "\n- ";
			while (windowSizeTemp == 0) {
				std::getline(std::cin, answerStr);
				windowSizeTemp = strtol(answerStr.c_str(), NULL, 0);
				//user either inputted an incorrect number or a letter
				if (windowSizeTemp <= 0) {
					std::cout << "Invalid input: Try Again";
					windowSizeTemp = 0;
					continue;
				}

			}
			std::cout << "Setting Window Size from: " << this->mWindowSize << "to: " << answer;
			break;
		case 2:
			system("cls");
			std::cout << "Automatically finding the Window Size...\n";
			windowSizeTemp = getWidth();
			std::cout << "Found size {" << windowSizeTemp << "}\n";
			system("pause");
			break;
		} 
		this->mWindowSize = windowSizeTemp;
		windowSizeConfig();
	}
	/*
	little weird having a class nested in a class but it's cleaner than having it separate so oh well
	Will store a question e.g.
	1 - Add Credits
	2 - Create Order 


	Options:
		Title (string) *Optional* Default: "What would you like to do?" - The Question to ask the user.
		Options (Vector of strings) *Required* - The mOptions for the user to choose from.
	*/
	class Question {
	private:
		string mTitle;
		vector<string> mOptions;
	public:
		
		Question(string mTitle = "What Would you Like to do?", vector<string> mOptions = { "Back" }) : mTitle(mTitle), mOptions(mOptions) {
		}
		//Ask the question
		int ask(Helper& helper) {
			if (mOptions.back() != "Back") {
				if (helper.mNest == 0) {
					mOptions.push_back("Quit");
				}
				else {
					mOptions.push_back("Back");
				}
			}
			//display mTitle of question
			std::cout << endl << mTitle << endl;
			//display mOptions
			for (int i = 0; i <= mOptions.size() - 1; i++) {
				// 0 - {Answer}
				std::cout << i + 1 << " - " << mOptions[i] << endl;
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
				if (inputint == 0 or inputint > mOptions.size() or inputint < 1) {
					std::cout << endl << "Entry is not a valid!\n";
					system("pause");
					break;
				}
				//it is a number
				else {
					//is the input the last element if so we want to quit or go back a menu
					if (inputint == mOptions.size()) {
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
	Read a text file and return the result, this function does no parsing and will just return what is exactly in the file
	Options:
		filename (string) *Required* - The path to the file you want to read.
	*/
	string Read(string filename) {
		//read a whole line, including the white space
		//create variable for the file we are reading
		std::ifstream infile;
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
	Write to a text file
	Options:
		filename (string) *Required* - The path to the file you want to write to (will create the file if it doesn't exist).
		data (string) *Required* -  The data in string format to write to the file
	*/
	void Write(string filename, string data) {
		//Open and if it doesnt exist create the file
		std::ofstream outfile;
		outfile.open(filename);

		//write data to file and close it
		outfile << data;
		outfile.close();
	}
	//Create a divider the size of the window with "-" as the filler
	const string CreateDivider(int size = -1, char divider = '-') {
		Helper helper = *this;
		if (size == -1) {
			size = helper.mWindowSize;
		}
		std::stringstream ss;
		ss << std::setfill(divider) << std::setw(size) << "-";
		return ss.str();
	}

	/*
	Create and Show the user a standardised menu
	Options:
		mTitle (string) *Required* - The Title of the menu e.g. Welcome
		subtitle (string) *Optional* - a subtitle displayed under the mTitle, could be useful for nested menus
		content (string) *Optional* - Content in the menu e.g. could be a paragraph or just a sentence, won't default but isn't required for menu to function as a back answer is added automatically
		question *Optional* - A Question to ask the user, e.g. what menu to move to, won't default but isn't required for menu to function as a back answer is added automatically
	There will always be a back option in the question so no need to add one
	*/

	
	int Menu(string mTitle, string subtitle, string content, Question question) {
		Helper helper = *this;
		//The size of the window
		//the difference between the size of the window and the size of the mTitle in characters
		const size_t windowSizeDiffT = helper.mWindowSize - mTitle.size();
		const size_t windowSizeDiffS = helper.mWindowSize - subtitle.size();

		//Create Menu
		//Display Title / create menu headers
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffT - 1) / 2)) << " " << mTitle << " " << std::setfill('=') << std::setw(ceil((windowSizeDiffT) / 2 )) << "=";
		std::cout << endl;
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffS - 1) / 2)) << " " << subtitle << " " << std::setfill('=') << std::setw(ceil((windowSizeDiffS) / 2)) << "=";
		//show content
		if (content != "") {
			std::cout << endl << CreateDivider() << endl;
			std::cout << content;
			std::cout << endl << CreateDivider() << endl;
		}
		else {
			std::cout << endl << CreateDivider() << endl;
		}
		//return the question
		return question.ask(helper);
	}	
};
//---------------------------------------------------------





//Get a list of employees
vector<Employee> GetEmployees(Helper& helper) {
	/*
	format of original output:
	"
	ID Name Pay\n
	ID Name Pay
	"
	*/
	string output = helper.Read("employees.txt");
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
	vector<Employee> outputarr;
	
	//parse the output
	//split the output into an array of three indexes for ID, name and pay rate
	string token;
	vector<string> outputarrcomp;
	//split output string into each Employee
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
		if (outputarrcomp.size() >= 3) {
			outputarr.push_back(Employee(outputarrcomp[1], outputarrcomp[0], strtod(outputarrcomp[2].c_str(), NULL)));
		}
		outputarrcomp.clear();
	}
	return outputarr;
}

// get a singular Employee nm\
//will return an Employee with the name "NULL" if no Employee was found
Employee GetEmployee(Helper& helper, string name) {
	//Get all employees and check f\or the specific ID or username passed in
	vector<Employee> employees = GetEmployees(helper);
	for (Employee i : employees) {
		if (i.ID == name or i.name == name) {
			return i;
		}
	}
	return { "NULL", "", 0.0 };

}
//GetPayments function will get the payment info from a file and return it;
//output format:
//		[0]				 [1]			  [2]
// {hours worked, {monthly after}, {monthly before}};
vector<vector<string>> GetAllPayments(Helper& helper, string file) {
	vector<string> outputLine;
	vector<string> outputCompTemp;
	vector<vector<string>> output;

	//read the month file passed in;
	string data = helper.Read(file);
	//Parse the File:
	//First split the output into each line aka employees
	std::stringstream ss(data);
	std::string token;
	while (std::getline(ss, token, '\n')) {
		outputLine.push_back(token);
	}
	//split each line into each component and add it to output
	for (string line : outputLine) {
		//split the line by spaces
		//since there are only two components if we go over 2 loops then something must be wrong
		while (std::getline(std::stringstream(line), token, ' ')) {
			//add the component to the back of the temporary vector
			outputCompTemp.push_back(token);
		}
		if (!outputCompTemp.empty()) {
			//add the temporary vector to the final output:
			output.push_back(outputCompTemp);
			//clear the temp vector for next loop
			outputCompTemp.clear();
		}
	}
	for (vector<string> emp : output) {
		std::cout << emp[0] << " - " << emp[1] << endl;
	} 
	system("pause");
	return output;
}




//get a single employees payment info

//get the Employee that has the specified ID, display it and then get the monthly pay and hours worked for the ID
//First get the
string ViewSinglePayment(Helper& helper, string ID) {
	//the stringstream that will be outputted
	std::stringstream returnout;
	//vector of each months components;
	vector<vector<string>> outputMonths;
	//get the Employee details
	Employee info = GetEmployee(helper, ID);
	if (info.name == "NULL") {
		return "Employee not found!";
	}
	std::string employeeTable = ViewSingleInfo(helper, info);
	//create a table to add the payment information to.
	tabulate::Table table;
	//for each month we get the entire file and find the ID passed in
	std::string path = ".\\months";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		//get all employees in this file
		for (vector<string> employee : GetAllPayments(helper, entry.path().string())) {
			if (employee[0] == info.ID) {
				returnout << "We Found: " << employee[1] << " For ID: " << ID << " In Month: " << entry.path().string();
				break;
			}
		}
		break;
	}

	return returnout.str();
}

//return a list of payment information for a specified month including Employee id, name, rate of pay, hours worked, monthly pay before and after tax;
//TODO: Get Hours worked for each Employee;
string ViewPaymentFile(Helper& helper, string filename) { 
	return "";
}


//return a list of employees information including Employee id, name and rate of pay
string ViewAllInfo(Helper& helper) {
	vector<Employee> employees = GetEmployees(helper);
	//Create table
	tabulate::Table table;
	std::stringstream payFormat;
	table.add_row({ "Name", "ID", "Rate of Pay"});
	for (Employee i : employees) {
		payFormat = std::stringstream();
		payFormat << std::setprecision(4) << i.pay;
		table.add_row({i.name, i.ID, payFormat.str()});
	}
	return table.str();
}


//get a single employees information
string ViewSingleInfo(Helper &helper, Employee employee) {
	tabulate::Table table;
	std::stringstream payFormat;
	table.add_row({ "Name", "ID", "Rate of Pay" });
	payFormat << std::setprecision(4) << employee.pay;
	table.add_row({ employee.name, employee.ID, payFormat.str() });
	return table.str();
}

//get and show payment information
void Payment(Helper& helper, string content) {
	system("cls");
	int answer = helper.Menu("Payment Information", "View Payment Information", content, { "What Would You Like to do?", {"View Payment Info for an Employee", "View information from a month file"}});
	switch (answer) {
	case 0:
		helper.IncrementNest();
		return;
	case 1: {
			string id;
			std::cout << "Please input the ID or name of the Employee (case sensitive)\n- ";
			std::getline(std::cin, id);
			content = ViewSinglePayment(helper, id);
			break;
		}
	case 2:
		string file;
		std::cout << "Please input filename of the month (case sensitive)\n- ";
		std::getline(std::cin, file);
		content = ViewPaymentFile(helper, file);
		break;
	
	}
	Payment(helper, content);
}
//get and show Employee information
void Info(Helper& helper, string content) {
	system("cls");
	int answer = helper.Menu("Employee Information", "View Employee information", content, { "What Would You Like To Do", {"View all", "View a specific Employee"}});
	switch(answer){
	case 0:
		helper.DecrementNest();
		return;
	//View All
	case 1:
		content = ViewAllInfo(helper);
		break;
	//View Single
	case 2: {
		string id;
		std::cout << "Please input the ID or name of the Employee (case sensitive)\n- ";
		std::getline(std::cin, id);
		Employee chosen = GetEmployee(helper, id);
		if (chosen.name != "NULL") {
			content = ViewSingleInfo(helper, chosen);
		}
		else {
			content = "Employee could not be found";
		}
		
		break;
	}
	}
	Info(helper, content);
}
//configure the programs settings.
void config(Helper& helper) {
	system("cls");
	int answer = helper.Menu("Config", "Configure program settings", "", { "What Would you Like to do?", {"Window Size"} });
	switch (answer) {
	case 0:
		helper.DecrementNest();
		return;

	case 1:
		helper.IncrementNest();
		helper.windowSizeConfig();

	}
	config(helper);
}
//Welcome Screen
void Welcome(Helper& helper) {
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
	string splash = "{splash}\n" + helper.CreateDivider();
	string content = splash + "The payroll system will contain all the information for an Employee and their pay including income tax";
	//replace {splash} with a random message chosen by random number generator above
	int randomint = distrib(gen);

	content.replace(content.find("{splash}"), sizeof("{splash}") - 1, messages[randomint]);

	
	
	int answer = helper.Menu("Welcome!", "Welcome to the payroll system!", content, { "What Would You Like To Do?", { "View Payment Information", "View Employee Information", "Configure"}});
	switch (answer) {
		//case 0 is back or quit
	case 0:
		helper.DecrementNest();
		return;
	case 1:
		helper.IncrementNest();
		Payment(helper, "*Information will appear here*");
		break;
	case 2:
		helper.IncrementNest();
		Info(helper, "*Information will appear here*");
		break;
	case 3:
		helper.IncrementNest();
		config(helper);
		break;

	}

	Welcome(helper);
}
/*
The main function just starts the program could also be useful for restarting the program as everything is done outside this function.
*/
int main() {
	std::cout << "- WARNING -\nResizing Window smaller than default may cause menus to break\n";
	system("pause");
	std::cout << endl;
	//create the helper object which contains helper functions and set the window size
	Helper helper = Helper();
	helper.setWindowSizeAuto();
	//then call welcome to start the program
	Welcome(helper);

}

