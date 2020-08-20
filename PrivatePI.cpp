//this was the public test version, source.cpp was always the development build.
//consult source.cpp for the most developed version of the software.
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdio.h>

#define clear() printf("\033[H\033[J")

using namespace std;

struct USERINFO {
	//each user will have 2 pieces of information
	//USERINFO objects can be stored into  a text file that can only be viewed by an administrator
	//this same information can be placed in either exiting user database or, accounts requesting access database
	string username;
	string password;
};


USERINFO createLogin() {
	//function will run once a user selects to apply for login
	USERINFO newUser;
	string newUsername;
	string newPassword;
	bool isUserRegistered;

	cout << "Type Your preffered Username: ";
	cin >> newUsername;

	cout << "Specify your password: ";
	cin >> newPassword;
	//check for existing USer accounts in a seperate function, the data returned must be compared with existing user information\
	//assign values to the new USERINFO variable
	newUser.username = newUsername;
	newUser.password = newPassword;
	// this USERINFO object will be used to test against other information stored within the user records 
	//use a conditional to write to the requested access  text file if user name is not found

	//if(isUserRegistered == false){
	//	//info is not registered we can write it to the request access database 
	//	//ask ethan for how he wrote to the file here
	//}
	return newUser;
}


bool registeredLogin(USERINFO returningUser, fstream& file) {
	//search throughout the entire registered user file for specific LOGIN information
	//assume the user is not registered until their information is found
	bool userFound;
	USERINFO registeredUser;
	//loop through the file to find specific User info
	while (!file.eof()) {
		file >> registeredUser.username;
		file >> registeredUser.password;

		if (returningUser.username == registeredUser.username) {
			if (returningUser.password == registeredUser.password) {
				cout << "Login Found. " << endl;
				userFound = true;
				return userFound;
				//here we should grant access to private Pi's functions , since teh user has been found 
			}
		}
		
	}
	//return wether or not the user exists in the database or not 
	cout << "Login not Found. " << endl;
	userFound = false;
	system("clear");
	return userFound;
}

bool checkUserInfo(USERINFO newinfo) {
	//check the registered user information so that we dont have more than 1 account with the exact same credentials
	//check requested new user information against all information located within the registered user database 
	bool Registered;	//confirmation that the new user info is not found within the text file 
	USERINFO retryInfo;
	fstream fileInput;
	fstream fileOutput;	//used to write all dredential requests for access to private pi
	fileInput.open("registeredUsers.txt");
	Registered = registeredLogin(newinfo, fileInput);	//if the newInfo remains true then 

													//if newinfo remains true, we should be able to append them after the check
	if (Registered == true) {
		cout << "User already Exists, Try a Different Login. "<< endl;
		//user must resubmit their credentials through createlogin()
		retryInfo = createLogin();
		//recursively check the nformation until we return false
		checkUserInfo(retryInfo);
	}

	//so long as we return false, to REGISTERED, we will be able to exit this function
		
	fileInput.close();
	system("clear");
	return Registered;	//if Registered == false, then we can register the current user name
}

void welcomeScreen(){
	cout << "*********************************************" << endl;
	cout << "                 PrivatePI                   " << endl;
	cout << "*********************************************" << endl;
	cout <<"\n\nSelect an option \n     1)New User\n     2)Returning User\n     3)Quit"<< endl;

	USERINFO returningCredentials;
	fstream registerData("/home/pi/Desktop/PrivatePI/registeredUsers.txt");
	char selection;
	bool returningUser;
	cin >> selection;

	if(selection == '1'){
		createLogin();
		system("clear");
		cout <<"Credentials Submitted for Administrative Approval " << endl;
		welcomeScreen();
	}

	if (selection == '2'){
		cout << "USERNAME: ";
		cin >> returningCredentials.username;

		cout << "PASSWORD: ";
		cin >> returningCredentials.password;

		returningUser = registeredLogin(returningCredentials, registerData);

		if(returningUser == false){
			system("clear");
			cout <<"\n\nUsername or Password not Found" <<endl;
			welcomeScreen();
		}
		else
			system("clear");
			cout << "Access Granted " <<endl;
			exit(0);
			
		//other stuff will be implemented here
	}

	if(selection == '3'){
		cout << "Exiting Program" << endl; 
		exit(0);
	}

	else{
		system("clear");
		welcomeScreen();
	}
}

int main() {
	//we need access files that will house all the information on respective user types 
	//we will have 2 respective texts files that will house the information
	fstream registeredInfo("registeredUsers.txt");	//registeredUsers.txt houses all of the info on users already using PrivatePi
	fstream requestInfo("requestedUsers.txt");	//This Text File will House Users with Pending Registration
	system("clear");

	welcomeScreen();

	system("clear");
	return 0;
}
