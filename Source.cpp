//NOTE SOME OF THESE BUILDS ARE COMPLETELY DEPENDENT ON THE SYSTEM YOU ARE BUILDING ON, THIS IS NOT GOOD PRACTICE BUT AT THE TIME REALLY HELPED WITH TESTING.
//THIS SOFTWARE WAS DESIGNED AROUND USING THE RASPBERRY PI AS THE CENTRALIZED SERVER FOR USE OF THE VPN AND MANAGING TRAFFIC THROUGH SSH TUNNELING INTO THE RASPBERRY PI
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ios>

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

	cout << "Type Your preffered USERNAME: ";
	cin >> newUsername;

	cout << "Specify your PASSWORD: ";
	cin >> newPassword;
	//check for existing USer accounts in a seperate function, the data returned must be compared with existing user information\
	//assign values to the new USERINFO variable
	system("clear");
	newUser.username = newUsername;
	newUser.password = newPassword;
	// this USERINFO object will be used to test against other information stored within the user records 
	//use a conditional to write to the requested access  text file if user name is not found
	return newUser;
};

bool registeredLogin(USERINFO returningUser, fstream& file) {
	//search throughout the entire registered user file for specific LOGIN information
	//assume the user is not registered until their information is found

	bool userFound;
	USERINFO registeredUser;
	//loop through the file to find specific User info
	while (!file.eof()) {
		file >> registeredUser.username;
		file >> registeredUser.password;

		if (returningUser.username == registeredUser.username || returningUser.username == "MAJORZERO") {
			if (returningUser.password == registeredUser.password || returningUser.password == "CIPHER") {

				cout << "Login Found. " << endl;
				userFound = true;
				return userFound;
				//here we should grant access to private Pi's functions , since teh user has been found 
			}
			//WHO ARE THE PATRIOTS, WHAT IS THE PHILOSOPHER'S LEGACY
		}		
	}
	//return wether or not the user exists in the database or not 
	
	cout << "Login not Found. " << endl;
	userFound = false;
	
	file.close();
	file.clear();
	file.seekg(0, ios::beg);
	return userFound;

};

void DisplayRequests(){
	USERINFO request;
	fstream displayrequestfile("/home/pi/Desktop/PrivatePI/requestedUsers.txt");
	
	cout << "-----Requests For Access to Private PI-----" << endl;
	cout << "===========================================" << endl;
	while(!displayrequestfile.eof()){
		displayrequestfile >> request.username;
		displayrequestfile >> request.password;	
		cout << "Username: " << request.username << " Password: " << request.password << endl;
	}
};

void DisplayRegistered(string Path){
	USERINFO request;
	fstream displayregisterfile(Path);
	
	cout << "-----Registrations For Private PI-----" << endl;
	cout << "======================================" << endl;
	while(!displayregisterfile.eof()){
		displayregisterfile >> request.username;
		displayregisterfile >> request.password;	
		cout << "Username: " << request.username << " Password: " << request.password << endl;
	}

};

void checkUserInfo(USERINFO newinfo) {
	//check the registered user information so that we dont have more than 1 account with the exact same credentials
	//check requested new user information against all information located within the registered user database 
	
	bool Registered;	//confirmation that the new user info is not found within the text file 
	USERINFO retryInfo;
	fstream fileInput;
	fstream fileOutput;	//used to write all dredential requests for access to private pi
	fileInput.open("/home/pi/Desktop/PrivatePI/registeredUsers.txt");
	fileOutput.open("/home/pi/Desktop/PrivatePI/requestedUsers.txt", std::ios::out | std::ios::app);
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
	if(Registered == false){
		if(newinfo.username == "MAJORZERO"){
			if(newinfo.password == "CIPHER"){
				cout << "ACCESS DENIED" << endl;
				fileInput.close();
				fileOutput.close();

				exit(0);
			}
		}

		fileOutput << newinfo.username << " " << newinfo.password << '\n';
	}

	fileInput.close();
	fileOutput.close();

	fileInput.clear();
	fileInput.seekg(0, ios::beg);

	fileOutput.clear();
	fileOutput.seekg(0, ios::beg);
		//if Registered == false, then we can register the current user name
};


void administratorRegister(){
	//register a requested user into the REGISTERED USER database
	//display all users in the requests
	system("clear");
	string requestInfo;
	bool comparisonFound = false;
	USERINFO requestApproval;
	USERINFO compareRequest;
	fstream requestList("/home/pi/Desktop/PrivatePI/requestedUsers.txt");
	fstream registerList("/home/pi/Desktop/PrivatePI/registeredUsers.txt", std::ios::out | std::ios::app);
	
	//cout <<"******************************************" << endl;
	//while(!requestList.eof()){
	//	getline(requestList, requestInfo);
	//	cout << requestInfo << endl;
	//}

	//requestList.close();

	//cout <<"******************************************" << endl;
	DisplayRequests();

	cout <<"										     " << endl;
	cout <<"=============================================" << endl;
	cout <<"      Grant Access to a specific User	     " << endl;
	cout <<"=============================================" << endl;
	cout <<"1)Register requested User "<< endl;
	
		cout <<" Specify the User Credentials to Grant Access" << endl;
		cout <<" Username: ";
		cin >> requestApproval.username;
		cout <<" Password: ";
		cin >> requestApproval.password;
	
		//search for credentials, if the person is found, append them to the registered USers list
		//make the comparison to see if this USer Exists within the requested Userlist
		cout <<"we will now read from file" << endl;
		
	    while(!requestList.eof()){
			
			requestList >> compareRequest.username;
			requestList >> compareRequest.password;
			cout << compareRequest.username << " " << compareRequest.password << endl;

			if(requestApproval.username == compareRequest.username){
				if(requestApproval.password == compareRequest.password){
					cout << endl;
					cout <<"User found, Granting ACCESS" <<endl;
					comparisonFound = true;
				}
			}
		}
		if(comparisonFound == true){
			system("clear");
			cout << endl;
			cout <<" Appending to Registered User List " << endl;
			registerList <<"\n"<< requestApproval.username << " " << requestApproval.password;
		}

		else if(comparisonFound == false){
			system("clear");
			cout << endl;
			cout << " ERROR: Requested USERNAME and PASSWORD not found in request list. " << endl;
			administratorRegister();
		}

};

void administratorRevoke(){
	system("clear");	
	string requestInfo;
	string removedCredential;
	bool comparisonFound;
	USERINFO requestApproval;
	USERINFO compareRequest;
	
	string databaseFile;
	string databaseDir = "/home/pi/Desktop/PrivatePI/";
	cout << "Specify The Database of Users\n in the Database Directory\n" << databaseDir <<": ";
	cin >> databaseFile;
	
	string FullPath = databaseDir.append(databaseFile);

	cout << "Source: " << FullPath << endl;
	
	string DestinationFile;
	cout << "Specify Output File Database of Users\n in the Database Directory\n" << databaseDir <<": ";
	cin >> DestinationFile;
	string FinaldatabaseDir = "/home/pi/Desktop/PrivatePI/";
	string FinalPath = FinaldatabaseDir.append(DestinationFile);

	cout <<"Destination: " << FinalPath << endl;

	ifstream requestList(FullPath);
	ofstream registerList(FinalPath);

	DisplayRegistered(FullPath);
	
	cout <<"=================================================" << endl;
	cout <<"         Revoke Access to a specific User	     " << endl;
	cout <<"=================================================" << endl;
	cout <<"1)Revoke Registered-User Access" << endl;
	
	cout << "USERNAME to revoke: ";
	cin >> requestApproval.username;
	cout << "PASSWORD of account: "; 
	cin >> requestApproval.password;

	while(!requestList.eof()){
		requestList >> compareRequest.username;
		requestList >> compareRequest.password;
		
		if(compareRequest.username != requestApproval.username){
			if(compareRequest.password != requestApproval.password){
				
				registerList << compareRequest.username << " " << compareRequest.password << " \n";
			}
		}
	}
	system("clear");
	cout << "Revoking Access for "<< requestApproval.username << " ";
	cout << "Exiting Program..." <<endl;
	requestList.close();
	registerList.close();

	rename(FinalPath.c_str(),FullPath.c_str());
	exit(0);
	//remove(FullPath.c_str());
};

void administratorScreen(){
	//the administrator screen will have access to managing registered users and confirming new user registrations,
	cout << "=================================================" << endl;
	cout << "                  Administrator	          " << endl;
	cout << "=================================================" << endl;

	cout <<"What actions do you wish to complete:\n	1)Register requested Users\n	2)Manage Registered Users\n	3)Quit the Program " << endl;
	char adminSelection;
	cout << "Selection:  ";
	cin >> adminSelection;
	//create an admin menu for its funcrions
	if(adminSelection == '1'){
		administratorRegister();
	}

	else if(adminSelection == '2'){
		administratorRevoke();
	}

	else if(adminSelection == '3'){
		exit(0);
	}

	else
		administratorScreen();
	//certain actions will be completed based on administrators selection;
};  //administrator functions 


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
		
		returningCredentials = createLogin();
		checkUserInfo(returningCredentials);
		system("clear");
		cout <<"Credentials Submitted for Administrative Approval " << endl;
		cout <<"Exiting PrivatePi..." << endl;
		exit(0);
		//implement system logoutfunction here if possible

	}

	if (selection == '2'){
		cout <<"==========================================" << endl;
		cout <<"             Returning User               " << endl;
		cout <<"==========================================" << endl; 
		cout << "USERNAME: ";
		cin >> returningCredentials.username;

		cout << "PASSWORD: ";
		cin >> returningCredentials.password;
		system("clear");

		returningUser = registeredLogin(returningCredentials, registerData);

		if(returningCredentials.username == "MAJORZERO"){
			if(returningCredentials.password == "CIPHER"){
				administratorScreen();
			}
		}

		if(returningUser == false && returningCredentials.username != "MAJORZERO" && returningCredentials.password != "CIPHER"){
			cout << endl;
			cout <<"Username or Password not Found" <<endl;
			welcomeScreen();
		}

		
		if(returningUser == true){
			cout << endl;
			cout << "Access Granted " <<endl;
		}
		//other stuff will be implemented here
		registerData.close();
	}

	if(selection == '3'){
		cout << "Exiting Program" << endl; 
		exit(0);
	
	}

	else{
		system("clear");
		welcomeScreen();
	}
};

int main() {
	//we need access files that will house all the information on respective user types 
	//we will have 2 respective texts files that will house the information
	fstream registeredInfo("registeredUsers.txt");	//registeredUsers.txt houses all of the info on users already using PrivatePi
	fstream requestInfo("requestedUsers.txt");	//This Text File will House Users with Pending Registration
	char selection;
	bool setupcompleted;
	system("clear");
	cout << "Have You Completed First time Set up for tunnelng?\n	Enter(Y/N): " << endl;
	cin >> selection;

	if (selection == 'N' || selection == 'n') {
		string s[] = {
		 "This is a tutorial for directing outgoing web traffic through your proxy using PuTTY.\n",
		 "Open PuTTy to the session menu.\n",
		 "Add IP address of proxy server under \"Host Name (or IP address\", then SSH port (usually 22).\n",
		 "Go to the Tunnels menu in Connection->SSH->Tunnels\n",
		 "input any source port, then switch Destination to Dynamic. Press the \"Add\" button.\n",
		 "This will add an item to the \"Forwarded Ports\" list. You don't need to do anything with this item.\n",
		 "Back in the Session menu and under \"Saved Sessions\", name your session and hit \"Save\".\n",
		 "This will allow you to recreate this configuration without redoing the above steps.\n",
		 "For the final step, configure proxy settings for the browser.\n",
		 "Open your browser settings page and go to Advanced Settings.\n",
		 "Open Proxy Settings.\n",
		 "Select your VPN Connection and go to settings.\n",
		 "In the settings window, open Advanced Settings\n",
		 "Go to the section labeled \"SOCKS\"\n",
		 "Input \"localhost\" under proxy address and the source port you chose under port.\n",
		 "The proxy configuration is now complete.\n"
		};

		for (int i = 0; i < 16; ++i) {
			cout << "Step" << i + 1 << ": " << s[i] << "Press ENTER to continue...\n";
			cin.ignore();
		}

	};

	system("clear");
	cout <<"----------------------------------------------------------------------" << endl;
	cout << "All New Users must apply for access and await Administrative Approval" << endl;
	cout <<"----------------------------------------------------------------------" << endl;

	welcomeScreen();

	system("clear");
	return 0;
};
