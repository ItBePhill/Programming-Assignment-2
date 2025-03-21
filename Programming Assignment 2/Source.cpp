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
	* Write Text //
	* Menu Template //
		- Potentially make title update automatically to centre with subtitle //
	* Question Template //
- Create Main Functions
	* Welcome Screen //
	* Find Employee information //
	* Find specified Employee Pay for each month //
	* Read from pay file by inputting file name //
	* Write employees id number and monthly pay before tax deduction to a file called '{month_file}_output.txt' //
	* Record error in a pay file by adding it to a file called errors.txt in format {name of file} {error description} //



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
void Payment(Helper&, string );
void Info(Helper&, string );
void Welcome(Helper&);
vector<Employee> GetEmployees(Helper&);
string ViewSingleInfo(Helper&, Employee);
void writeErrors(Helper&, string, vector<string>);
//---------------------------------------------------------


//Classes -------------------------------------------------

/// <summary>
/// holds info on an Employee including name, ID and pay
/// </summary>
struct Employee {
	public:
		//name of the Employee
		string name;
		//ID of the Employee
		string ID;
		//pay rate of the Employee
		double pay;
};


/// <summary>
/// Utility functions e.g. Read, Write
/// </summary>
class Helper {
private:
	//size of the console window, used to create dividers and menus
	size_t mWindowSize = 120;
	//the current amount of menus the user is in
	int mNest = 0;
	char mMoneySign = char(156);
public:
	char getMoneySign() {
		return mMoneySign;
	}
	size_t getWindowSize() {
		return this->mWindowSize;
	}
	void setWindowSizeAuto() {
		this->mWindowSize = getWidth();
	}
	void setWindowSize(size_t Size) {
		if (Size < 1) {
			throw std::invalid_argument("Size too small");
		}
		else {
			this->mWindowSize = Size;
		}
	}
	void IncrementNest() {
		this->mNest++;
	}
	void DecrementNest() {
		this->mNest--;
	}


	/// <summary>
	/// Adjust window Size Menu
	/// </summary>
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

	class Question {
	private:
		string mTitle;
		vector<string> mOptions;
	public:
		/// <summary>
		/// A Question to be used in a menu
		/// </summary>
		/// <param name="Title">The title opf the question</param>
		/// <param name="Options">The options for the question</param>
		Question(string Title = "What Would you Like to do?", vector<string> Options = { "Back" }) : mTitle(Title), mOptions(Options) {
		}
		/// <summary>
		/// Ask the question
		/// </summary>
		/// <param name="helper">Helper class</param>
		/// <returns></returns>
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

	
	/// <summary>
	/// Read a text file and return the result, this function does no parsing and will just return what is exactly in the file
	/// </summary>
	/// <param name="filename">The path to the file you want to read.</param>
	/// <returns>Returns of the data in the file</returns>
	string Read(string filename) {
		if (!std::filesystem::exists(filename)) {
			return "Something went wrong and we couldnt find the file";
		}
		else {
			//read a whole line, including the white space
			//create variable for the file we are reading
			std::ifstream infile;
			//and for the string to write the result to a buffer
			std::stringstream str;
			//write to this and then add to the actual return string
			string buffer;
			//open the file
			infile.open(filename);
			//get the first line in the file
			while (!infile.eof()) {
				if (std::getline(infile, buffer)) {
					str << buffer << "\n";
				}
			}
			//close the file
			infile.close();
			return str.str();
		}
	}


	/// <summary>
	/// Write to a text file
	/// </summary>
	/// <param name="filename">The path to the file you want to write to (will create the file if it doesn't exist).</param>
	/// <param name="data">The data in string format to write to the file</param>
	void Write(string filename, string data) {
		//Open and if it doesnt exist create the file
		std::ofstream outfile;
		outfile.open(filename);

		//write data to file and close it
		outfile << data;
		outfile.close();
	}



	/// <summary>
	/// Create a Divider using the '-' character
	/// </summary>
	/// <param name="size">the size of the divider if set to -1 or left unset will create based on windowSize</param>
	/// <param name="divider">the divider to use defaults to '-' </param>
	/// <returns>returns a string containing the divider</returns>
	const string CreateDivider(int size = -1, char divider = '-') {
		if (size == -1) {
			size = this->mWindowSize;
		}
		std::stringstream ss;
		ss << std::setfill(divider) << std::setw(size) << "-";
		return ss.str();
	}


	/// <summary>
	/// 	Create and Show the user a standardised menu
	/// </summary>
	/// <param name="mTitle">The Title of the menu e.g. Welcome</param>
	/// <param name="subtitle">A subtitle displayed under the Title, could be useful for nested menus</param>
	/// <param name="content">Content in the menu e.g. could be a paragraph or just a sentence, won't default but isn't required for menu to function as a back answer is added automatically</param>
	/// <param name="question">A Question to ask the user, e.g. what menu to move to, won't default but isn't required for menu to function as a back answer is added automatically</param>
	/// <returns></returns>
	int Menu(string title, string subtitle, string content, Question question) {
		//The size of the window
		//the difference between the size of the window and the size of the mTitle in characters
		const size_t windowSizeDiffT = this->mWindowSize - title.size();
		const size_t windowSizeDiffS = this->mWindowSize - subtitle.size();

		//Create Menu
		//Display Title / create menu headers
		std::cout << std::setfill('=') << std::setw(ceil((windowSizeDiffT - 1) / 2)) << " " << title << " " << std::setfill('=') << std::setw(ceil((windowSizeDiffT) / 2 )) << "=";
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
		return question.ask(*this);
	}	


	/// <summary>
	/// replace text in a string
	/// </summary>
	/// <param name="original">The string containing the substring to replace</param>
	/// <param name="oldsubstr">The substring to replace</param>
	/// <param name="newsubstr">The new string to insert</param>
	/// <returns></returns>
	string replaceText(string original, string oldsubstr, string newsubstr) {
		return original.replace(original.find(oldsubstr), sizeof(oldsubstr) - 1, newsubstr);
	}
};
//---------------------------------------------------------







/// <summary>
/// Get a list of employees
/// </summary>
/// <param name="helper">Helper class</param>
/// <returns>returns a vector of employees</returns>
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

/// <summary>
///  get a singular Employee will return an employee with the name "NULL" if no employee was found
/// </summary>
/// <param name="helper">Helper class</param>
/// <param name="name">the name or ID of the employee</param>
/// <returns></returns>
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




/// <summary>
/// GetPayments function will get the payment info from a file and return it
/// </summary>
/// <param name="helper">Helper class</param>
/// <param name="file">The file we want to read from</param>
/// <returns>returns a vector of monthlies, e.g. {monthly(before), monthly(after)} in string format</returns>
vector<vector<string>> GetAllPayments(Helper& helper, string file, vector<string> &errors) {
	vector<string> outputLine = {};
	vector<string> outputCompTemp = {};
	vector<vector<string>> output = { {} };
	std::stringstream errorBuffer;
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
		//Code Taken from: https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/#1-using-stringstream
		//and edited slightly

		// Create a stringstream object
		std::stringstream ss(line);

		// Variable to hold each word
		string word;

		// Extract words from the sentence
		int count = 0;
		while (ss >> word) {
			outputCompTemp.push_back(word);
			count++;
		}
		//the output vector is smaller than it should be and is therefore missing a component
		//TODO: add a way to check which counter we are at to determine whether it is a missing ID or hours.
		if (outputCompTemp.size() < 2) {
			if (count == 0) {
				//we have an ID but not hours
				errorBuffer << "We are missing the ID! for hours: " << word;
				errors.push_back(errorBuffer.str());
				errorBuffer = std::stringstream();
			}
			else {
				//we have hours but not an ID
				errorBuffer << "We are missing hours! for ID: " << word;
				errors.push_back(errorBuffer.str());
				errorBuffer = std::stringstream();
			}
			outputCompTemp.clear();
			continue;
		}
		// we have looped twice and gotten the ID and hours worked and therefore Need to add it to the final list
		output.push_back(outputCompTemp);
		outputCompTemp.clear();
	}
	return output;
}

//

/// <summary>
/// write an error to the error file
/// </summary>
/// <param name="file">The file the errors was found in</param>
/// <param name="desc">A vector of descriptions of the errors</param>
void writeErrors(Helper &helper, string file, vector<string> desc) {
	//file format
	//[Month]
	//description of error
	//description of error
	//description of error
	//[/Month]
	//[Month]
	//description of error
	//description of error
	//description of error
	//[/Month]


	//string that will be written


	std::string oldata;
	std::stringstream returnout;
	string filebase = file.substr(file.find_last_of("/\\") + 1);

	//create the string
	//get the basename of the file
	returnout << "[" << filebase << "]";
	for (string error : desc) {
		returnout << endl << error;
	}
	returnout << endl << "[/" << filebase << "]";
	//check if the file exists
	//if not skip straight to writing the file as we dont need to check anything
	if (!std::filesystem::exists(".\\errors.txt")) {
		helper.Write(".\\errors.txt", returnout.str());
		return;
	}
	else {
		//read the file and loop through until we find [month].
		//if we find it then keep looping until we find [/month]
		//count the amount of loops from [month] to [/month] and delete these lines


		//add the new data to the end of the file

		//get the current data in the file
		oldata = helper.Read(".\\errors.txt");
		string dataBuffer;
		bool found = false;
		int countFirst = 0;
		int count = 0;
		vector<string> tempVector;
		std::stringstream ss(oldata);
		//create a vector of each line
		while (bool(getline(ss, dataBuffer, '\n')) && not dataBuffer.empty()) {
			tempVector.push_back(dataBuffer);
		}
		//check if the file contains the month
		for(string i: tempVector) {
			count++;
			if (found) {
				if (i == "[/" + filebase + "]") {
					//we found the end of this month!
					//delete from the first to last index of the month
					tempVector.erase(tempVector.begin()+(countFirst-1), tempVector.begin() + count);
					break;
				}
			}
			else if (i == "[" + filebase + "]") {
				//we have found the month!
				//keep looping until we find [/month]
				found = true;
				countFirst = count;
				continue;
			}
		}


		
		std::stringstream newdata;
		//rebuild the string
		for (string str : tempVector) {
			newdata << str << "\n";
		}
		newdata << returnout.str();


		helper.Write(".\\errors.txt", newdata.str());
	}







}

/// <summary>
/// write the monthlies to a file
/// </summary>
/// <param name="helper">Helper class</param>
/// <param name="monthlies">Vector of the before and after tax monthly pay</param>
/// <param name="hours">Vector of hours worked</param>
/// <param name="employees">vector of employee info</param>
/// <param name="filename">the name of the month file</param>
void writeMonthlies(Helper &helper, vector<vector<double>> monthlies, vector<string> hours, vector<Employee> employees, string filename) {
	//strings that will be written to the file
	std::stringstream outputString;
	std::stringstream ssTemp;
	//add _output to filename
	filename = helper.replaceText(filename, ".txt", "_output.txt");
	//Taken from: https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
	string base_filename = filename.substr(filename.find_last_of("/\\") + 1);
	filename = ".\\output\\" + base_filename;
	//for each employee then construct a string which will be written on a line of the file
	//format of each line:
	//ID hours monthly(before) monthly(after)
	for (int i = 0; i < employees.size() - 1; i++) {
		ssTemp = std::stringstream();
		ssTemp << employees[i].ID << " " << monthlies[i][0] << " " << monthlies[i][1];
		outputString << ssTemp.str() << "\n";
	}

	//write to the file
	helper.Write(filename, outputString.str());
}
/// <summary>
///	 Calculate the Monthly Pay for the specified employee
/// </summary>
/// <param name="hours">The hours worked</param>
/// <param name="employee">The employees information</param>
/// <returns></returns>
vector<double> CalculateMonthly(double hours, Employee employee) {
	//You must multiply your monthly income by twelve to establish the annual income.If this is less than £12570, then tax should not be deducted,
	//Calculating Income Tax :
	//assuming £2, 000 monthly income.
	//£2, 000 * 12 = £24, 000 (annual income)
	//£24, 000 - £12, 570 = £11, 430 (taxable income)
	//£11, 430 * 0.20 = £2, 286 (annual income tax)
	//£2, 286 / 12 = £190.50 (monthly tax deduction).
	//subtract from monthly
	double monthly;
	double monthlytaxed;
	double annual;
	double annualtaxed;
	double threshold = 12570;
	double percentage = 0.20;
	//calculate their monthly income:
	//pay rate * hours to get the amount of pay over the month
	monthly = employee.pay * hours;
	annual = monthly * 12;
	if (annual >= threshold) {
		//this person can be taxed
		annualtaxed = (annual - threshold) * percentage;
		monthlytaxed = annualtaxed / 12;
		monthlytaxed = monthly - monthlytaxed;
	}
	else {
		//Not applicable to be taxed
		monthlytaxed = -1;
	}
	//{monthly before, monthly after}
	return { monthly, monthlytaxed};
}



/// <summary>
/// get a single employees payment info
/// get the Employee that has the specified ID, display it and then get the monthly pay and hours worked for the ID
///	then calculate the monthly pay with/without tax
/// </summary>
/// <param name="helper">Helper class</param>
/// <param name="ID">The ID of the employee we want to view pay info for</param>
/// <returns>returns a table containing the info</returns>
string ViewSinglePayment(Helper& helper, string ID) {
	//the stringstream that will be outputted
	std::stringstream returnout;
	//vector of each months components;
	vector<vector<string>> outputMonths;
	tabulate::Table monthlyTable;
	//get the Employee details
	Employee info = GetEmployee(helper, ID);
	if (info.name == "NULL") {
		return "Employee not found!";
	}
	std::string employeeTable = ViewSingleInfo(helper, info);
	//for each month we get the entire file and find the ID passed in
	std::string path = ".\\months";
	vector<string> errors;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		//get all the payments in this file
		vector<vector<string>> data = GetAllPayments(helper, entry.path().string(), errors);
		for (vector<string> employee : data) {
			if (!employee.empty()) {
				if (employee[0] == info.ID && employee.size() > 1) {
					//add this to the list for each month
					//add the stuff from the month plus the file name
					outputMonths.push_back({ employee[0], employee[1], entry.path().string() });
				}
			}
		}
	}
	//create and display table
	monthlyTable.add_row({"Month", "Hours Worked", "Monthly(Before Tax)", "Monthly (After Tax)"});
	for (auto i : outputMonths) {
		//get the monthly income
		vector<double> monthlies = CalculateMonthly(strtod(i[1].c_str(), NULL), info);
		//create stringstreams for the monthlies to set precision
		std::stringstream monthlyAfter;
		std::stringstream monthlyBefore;
		if(monthlies[1] != -1){
			monthlyAfter << helper.getMoneySign() << std::fixed << std::setprecision(2) << monthlies[1];
		}
		else {
			monthlyAfter << "N/A";
		}
		monthlyBefore << helper.getMoneySign() << std::fixed << std::setprecision(2) << monthlies[0];
		
		//add the values to the table
		monthlyTable.add_row({ i[2], i[1]+"hrs", monthlyBefore.str(), monthlyAfter.str()});

	}
	//return the tables as strings
	return employeeTable + "\n" + monthlyTable.str();
 }




/// <summary>
/// return a list of payment information for a specified month including Employee id, name, rate of pay, hours worked, monthly pay before and after tax;
/// </summary>
/// <param name="helper">Helper Class</param>
/// <param name="filename">The file that we are viewing from</param>
/// <returns>a table containing all the relevant info</returns>
string ViewPaymentFile(Helper& helper, string filename) { 
	//get all the payments in this file

	vector<string> errors;
	std::stringstream errorBuffer;
	vector<vector<string>> data = GetAllPayments(helper, filename, errors);
	//Create table
	tabulate::Table table;

	vector<vector<double>> monthliesVector;
	vector<string> hours;
	vector<Employee> employees;

	table.add_row({ "Name", "ID", "Rate of Pay", "Monthly (Before Tax)", "Monthly (After Tax)"});
	for (vector<string> payment : data) {
		if (!payment.empty()) {
			Employee info = GetEmployee(helper, payment[0]);

			if (info.name != "NULL") {
				//get the details for this ID
				vector<double> monthlies = CalculateMonthly(strtod(payment[1].c_str(), NULL), info);

				monthliesVector.push_back(monthlies);
				hours.push_back(payment[1]);
				employees.push_back(info);
				
				std::stringstream monthlyAfter;
				std::stringstream monthlyBefore;
				if (monthlies[1] != -1) {
					monthlyAfter << helper.getMoneySign() << std::fixed << std::setprecision(2) << monthlies[1];
				}
				else {
					monthlyAfter << "N/A";
				}
				monthlyBefore << helper.getMoneySign() << std::fixed << std::setprecision(2) << monthlies[0];
				std::stringstream pay;
				pay << helper.getMoneySign() << std::fixed << std::setprecision(2) << info.pay;
				table.add_row({ info.name, info.ID, pay.str(), monthlyBefore.str(), monthlyAfter.str() });
			}
			else {
				errorBuffer = std::stringstream();
				errorBuffer << "Employee with ID: " << payment[0] << " doesn't exist";
				if (not errors.empty()) {
					if (errors.back() != errorBuffer.str() ) {
						errors.push_back(errorBuffer.str());
					}
				}
				else {
					errors.push_back(errorBuffer.str());
				}
				continue;
			}
		}
	}
	if (not errors.empty()) {
		writeErrors(helper, filename, errors);
	}
	writeMonthlies(helper, monthliesVector, hours, employees, filename);
	return table.str();
}


/// <summary>
/// View All info about an employee
/// </summary>
/// <param name="helper">Helper class</param>
/// <returns>table containing the info</returns>
string ViewAllInfo(Helper& helper) {
	vector<Employee> employees = GetEmployees(helper);
	//Create table
	tabulate::Table table;
	table.add_row({ "Name", "ID", "Rate of Pay"});
	for (Employee i : employees) {
		std::stringstream pay;
		pay << helper.getMoneySign() << std::fixed << std::setprecision(2) << i.pay;
		table.add_row({ i.name, i.ID, pay.str() });
	}
	return table.str();
}


/// <summary>
/// get a single employees information
/// </summary>
/// <param name="helper">Helper Class </param>
/// <param name="employee">the employee we are viewing the info for</param>
/// <returns>a table containing the info</returns>
string ViewSingleInfo(Helper &helper, Employee employee) {
	tabulate::Table table;
	std::stringstream payFormat;
	table.add_row({ "Name", "ID", "Rate of Pay" });
	std::stringstream pay;
	pay << helper.getMoneySign() << std::fixed << std::setprecision(2) << employee.pay;
	table.add_row({ employee.name, employee.ID, pay.str()});
	return table.str();
}


/// <summary>
/// get and show payment information
/// </summary>
/// <param name="helper">Helper class </param>
/// <param name="content">The content of the menu</param>
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
		std::cout << "Please input filename of the month (e.g. Mar25.txt) (case sensitive)\n- ";
		std::getline(std::cin, file);
		if (file.size() < 2) {
			content = "That File Doesn't Exist";
		}
		else if (std::filesystem::exists(".\\months\\" + file)) {
			content = ViewPaymentFile(helper, ".\\months\\" + file);
		}
		else {
			content = "That File Doesn't Exist!";
		}
		break;
	
	}
	Payment(helper, content);
}

/// <summary>
/// get and show Employee information
/// </summary>
/// <param name="helper">Helper class</param>
/// <param name="content">The content shown in the menu</param>
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

/// <summary>
/// configure the programs settings.
/// </summary>
/// <param name="helper">Helper class</param>
void config(Helper& helper) {
	system("cls");
	int answer = helper.Menu("Config", "Configure program settings", "", { "What Would you Like to do?", {"Window Size", "View Error List"}});
	switch (answer) {
	case 0:
		helper.DecrementNest();
		return;

	case 1:
		helper.IncrementNest();
		helper.windowSizeConfig();
		break;
	case 2:
		break;
	}

	config(helper);
}

/// <summary>
/// Welcome Screen
/// </summary>
/// <param name="helper">Helper class</param>
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
	string splash = "{splash}" + helper.CreateDivider() + "\n";
	string content = splash + "The payroll system will contain all the information for an Employee and their pay including income tax";
	//replace {splash} with a random message chosen by random number generator above
	int randomint = distrib(gen);
	content = helper.replaceText(content, "{splash}", messages[randomint]+"\n");
	

	
	
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


/// <summary>
/// The main function just starts the program could also be useful for restarting the program as everything is done outside this function.
/// </summary>
/// <returns></returns>
int main() {
	std::cout << "- WARNING -\nResizing Window smaller than default may cause menus to break\n";
	system("pause");
	std::cout << endl;
	//Check if required folders exist:

	//months folder
	if (!std::filesystem::exists(".\\months")) {
		system("mkdir .\\months");
	}
	//output folder
	else if (!std::filesystem::exists(".\\months\\output")) {
		system("mkdir .\\output");
	}

	//create the helper object which contains helper functions and set the window size
	Helper helper = Helper();
	helper.setWindowSizeAuto();
	//then call welcome to start the program
	Welcome(helper);

}

