#include<iostream>
#include<iomanip>
#include<windows.h>
#include<fstream>
#include<sstream>
#include<string>
#include<cctype>
#include<conio.h>
#include<set>
#include<ctime>
#include <limits> // Required for std::numeric_limits
using namespace std;
//=====================================================
// Admin Login Credentials
//=====================================================

int id = 1;
string password = "admin123";

//=====================================================
// Global Variables
//=====================================================
string openedCNIC, userPassword;

//=====================================================
// TO STORE ALL PRVIOUSLY GENERATED NUMBERS
//=====================================================

set<int> usedNumbers;

//=====================================================
// Data structure for taxpayer
//=====================================================

struct Data{

    string Name, Gender, FillerStatus, CNIC, Contact_Number, Income, Asset, Password;
    long double Income_Tax, Asset_Tax;
};

//=====================================================
// Node structure for doubly linked list
//=====================================================

struct Node{

    Data taxpayer;
	Node *Previous;
	Node *Next;
	 
	Node(string CNIC,string Password,string Name,string Gender,string FillerStatus,string Contact_Number,string Income,string Asset,long double Income_Tax,long double Asset_Tax){
		
        taxpayer.CNIC = CNIC;
        taxpayer.Password = Password;
		taxpayer.Name = Name;
        taxpayer.Gender = Gender;
        taxpayer.FillerStatus = FillerStatus;
        taxpayer.Contact_Number = Contact_Number;   
        taxpayer.Income = Income;
        taxpayer.Asset = Asset;
        taxpayer.Income_Tax = Income_Tax;
        taxpayer.Asset_Tax = Asset_Tax;
		Next = Previous = NULL;
	}
};

//=====================================================
// Linked List structure
//=====================================================

	Node* Head;
	Node* Tail;

    bool isEmpty(){
			
		return(Head == NULL);
	}

    void Insert_Taxpayer(string CNIC,string Password,string Name,string Gender,string FillerStatus,string Contact_Number,string Income,string Asset,long double Income_Tax,long double Asset_Tax){

		if(isEmpty()){
			
			Node* newnode = new Node(CNIC, Password, Name, Gender, FillerStatus, Contact_Number, Income, Asset, Income_Tax, Asset_Tax);
			Head = Tail = newnode;
		}     
		else{
			
			Node* newnode = new Node(CNIC, Password, Name, Gender, FillerStatus, Contact_Number, Income, Asset, Income_Tax, Asset_Tax);
			Tail->Next = newnode;
			newnode->Previous = Tail;
			Tail = newnode;
		}
	}

    int Delete_Taxpayer(string cnic){
        
        if(isEmpty()){

            return 0; // No taxpayers registered
        }
        Node* current = Head;

        // Traverse until we find the CNIC
        while(current != NULL && current->taxpayer.CNIC != cnic){

            current = current->Next;
        }

        if (current == NULL){

            return 1; // CNIC not found
        }

        // Case 1: Deleting if at start
        if(current == Head){

            Head = Head->Next;
            if(Head != NULL){

                Head->Previous = NULL;
            }
            else{

                Tail = NULL; // List became empty
            }
        }
        // Case 2: Deleting if at end
        else if(current == Tail){

            Tail = Tail->Previous;
            if(Tail != NULL){

                Tail->Next = NULL;
            } 
            else{

                Head = NULL; // List became empty
            }
        }
        // Case 3: Deleting if in middle
        else{

            current->Previous->Next = current->Next;
            current->Next->Previous = current->Previous;
        }
        delete current;
        return 2; // Successfully deleted
    }

//=====================================================
// Function to read data from file and populate linked list
//=====================================================

void Read_data_From_File(){

    Head = NULL;
    Tail = NULL;
    ifstream file("taxpayers.txt");
    string cnic, password, name, gender, FillerStatus, contact, incomevalue, assetvalue, Income_Tax, Asset_Tax;

    while (getline(file, cnic, ',')){
         
        getline(file, password, ',');
        getline(file, name, ',');
        getline(file, gender, ',');
        getline(file, FillerStatus, ',');
        getline(file, contact, ',');
        getline(file, incomevalue, ',');
        getline(file, assetvalue, ',');
        getline(file, Income_Tax, ',');
        getline(file, Asset_Tax);
        // Insert into linked list
        Insert_Taxpayer(cnic, password, name, gender, FillerStatus, contact, incomevalue, assetvalue, stold(Income_Tax), stold(Asset_Tax));
        if(file.eof()){
        	
            break;
        }
    }
    file.close();
}

//=====================================================
// Function to write linked list data to file
//=====================================================

void Write_data_To_File(){

    ofstream temp_file("temp.txt");
    Node* temp = Head;
    while(temp != NULL){

        temp_file.setf(ios::fixed);
        temp_file << setprecision(2);
        temp_file << temp->taxpayer.CNIC << "," << temp->taxpayer.Password << "," << temp->taxpayer.Name << "," 
                  << temp->taxpayer.Gender << "," << temp->taxpayer.FillerStatus << "," << temp->taxpayer.Contact_Number << ","
                  << temp->taxpayer.Income << "," << temp->taxpayer.Asset << "," << temp->taxpayer.Income_Tax << "," 
                  << temp->taxpayer.Asset_Tax << endl;
        temp = temp->Next;
    }
    temp_file.close();
    remove("taxpayers.txt");
    rename("temp.txt", "taxpayers.txt");
}

//=====================================================
// HELPER: TO FREE MEMORY AFTER USE
//=====================================================

void Free_Space(){

    Node* current = Head;
    while(current != NULL){
            
        Node* next = current->Next;
        delete current;
        current = next;
    }
    Head = Tail = NULL;

}

//=====================================================
// HELPER: TO GENERATE RECIPT NUMBER
//=====================================================

int GenerateRandom4Digit(){

    int number;

    do{

        number = 1000 + rand() % 9000;   // random number between 1000-9999
    }
    while( usedNumbers.count(number) > 0 );  // repeat if already used

    usedNumbers.insert(number);  // store it so it never repeats
    return number;
}

//=====================================================
// HELPER: GET DIGITS ONLY INPUT FUNCTION
//=====================================================

string getDigitsOnly(int maxLength){

    string input;
    char ch;

    while (true){

        ch = _getch();

        // ENTER -> finish input
        if (ch == 13) {
            cout << endl;
            break;
        }

        // digits only + length limit
        if (ch >= '0' && ch <= '9' && input.length() < maxLength) {
            input += ch;
            cout << ch;
        }

        // Backspace
        else if (ch == 8 && !input.empty()) {
            input.pop_back();
            cout << "\b \b";
        }

        // ignore all other keys (letters, symbols, etc.)
    }

    return input;
}


class Admin{

    public:

        string Name, Gender, FillerStatus, CNIC, Contact_Number, Income, Asset, Password;
        long double IncomeTaxRate, AssetTaxRate, Income_Tax, Asset_Tax;

        bool adminLogin(){

            system("cls");

            int enteredID;
            string enteredPassword;
            char ch;

            cout << "========== ADMIN LOGIN ==========\n\n";

            // Input Admin ID
            cout << "Enter Admin ID: ";
            cin >> enteredID;

            // Clear buffer
            cin.ignore(1000, '\n');

            // Input Password with Masking
            cout << "Enter Admin Password: ";
            enteredPassword = "";

            while(true){

                ch = _getch();

                if (ch == '\r'){

                    break;
                }

                if (ch == '\b'){

                    if (!enteredPassword.empty()){

                        enteredPassword.pop_back();
                        cout << "\b \b";
                    }
                }
                else{

                    enteredPassword.push_back(ch);
                    cout << '*';
                }
            }
            cout << "\n\n";

            // Authentication Check
            if (enteredID == id && enteredPassword == password)
            {
                cout << "Login Successful!\n";
                Sleep(2000);
                return true;
            }
            else
            {
                cout << "Invalid ID or Password!\n";
                Sleep(2000);
                return false;
            }
        }

        void DisplayAlltaxpayers() {

            system("cls");

            cout << "--------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\t\t\t\t\t\tRegistered Taxpayers\n";
            cout << "--------------------------------------------------------------------------------------------------------------------------\n";

            cout << left 
                 << setw(15) << "CNIC"
                 << setw(15) << "Name"
                 << setw(10) << "Gender"
                 << setw(15) << "Status"
                 << setw(15) << "Contact"
                 << setw(12) << "Income"
                 << setw(12) << "Assets"
                 << setw(12) << "IncomeTax"
                 << setw(12) << "AssetTax"
                 << endl;

            cout << "--------------------------------------------------------------------------------------------------------------------------\n";

            if(isEmpty()){
				
				cout<< "There are no Registered Users!" << endl;
			}
			else{
				
				Node* temp = Head;
				while(temp!=NULL){
					
                    cout << left 
                         << setw(15) << temp->taxpayer.CNIC
                         << setw(15) << temp->taxpayer.Name
                         << setw(10) << temp->taxpayer.Gender
                         << setw(15) << temp->taxpayer.FillerStatus
                         << setw(15) << temp->taxpayer.Contact_Number
                         << setw(12) << temp->taxpayer.Income
                         << setw(12) << temp->taxpayer.Asset
                         << setw(12) << temp->taxpayer.Income_Tax
                         << setw(12) << temp->taxpayer.Asset_Tax
                         << endl;
                    temp = temp->Next;
				}
				cout<< endl;
			}
            cout << "--------------------------------------------------------------------------------------------------------------------------\n";
            system("pause");
        }
        
        void Add_taxpayer(){

            system("cls");
            int count_cnic = 0, count_name = 0, count_contactno = 0, count_income = 0, count_asset = 0, count_password = 0;

            cout << "User Registration!" << endl;
            cout << "---------------------------------\n";

            while(1){

                if(count_name > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_name = 0;
                    return;
                }

                bool validName = true;
                cout << "Enter Name: ";
                cin >> ws; // to ignore leftover newline
                getline(cin, Name);

                for(int i = 0; i < Name.length(); i++){

                    if(!isalpha(Name[i]) && Name[i] != ' '){

                        validName =  false;
                    }
                }

                if(!validName){

                    cout << "Invalid Name! The name must use Upper and Lower Case Character." << endl;
                    cout << "Please try again." << endl;
                    cout << "---------------------------------\n";
                    Sleep(2000);
                    system("cls");
                    count_name++;
                }
                else{

                    break;
                }
            }

            while(1){

                if(count_cnic > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_cnic = 0;
                    return;
                }
                cout << "Enter CNIC (without dashes): ";
                CNIC = getDigitsOnly(13);

                bool validCNIC = true;

                // Check length = 13
                if(CNIC.length() != 13){

                    validCNIC = false;
                }
                else{

                    for(int j = 0; j < 13; j++){

                        if(!isdigit(CNIC[j])){

                            validCNIC = false;
                            break;
                        }
                    }
                }

                if(!validCNIC){

                    cout << "Invalid CNIC! It must be exactly 13 digits." << endl;
                    cout << "Please try again." << endl;
                    cout << "---------------------------------\n";
                    Sleep(2000);
                    system("cls");
                    count_cnic++;
                    continue;
                }
                else{

                    // Check if CNIC already exists

                    bool exists = false;
                    Node* temp = Head;
                    while(temp != NULL){

                        if(temp->taxpayer.CNIC == CNIC){

                            exists = true;
                            break;
                        }
                        temp = temp->Next;
                    }

                    if(exists){

                        cout << "CNIC already exists! Please try again." << endl;
                        cout << "---------------------------------\n";
                        Sleep(2000);
                        system("cls");
                        count_cnic++;
                    }
                    else{

                        break;
                    }
                }
            }

            int digit_count = 0, upper_count = 0, lower_count = 0, special_count = 0;

            while(1){

                if(count_password > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_password = 0;
                    return;
                }
                cout<< "Password should have:" << endl;
                cout<< "1. Must be between 4-12 Characters\n2. At least one uppercase letter" << endl;
                cout<< "3. At least one lowercase letter\n4. At least one digit\n5. At least one special character" << endl;
                cout << "Set Password: ";
                cin >> Password;

                bool validPassword = false;

                if(Password.length() >= 4 && Password.length() <= 12){

                    validPassword = true;
                }
                if(validPassword){

                    for(int j = 0; j < Password.length(); j++){

                        if(isdigit(Password[j])){

                            digit_count++;
                        }
                        if(isupper(Password[j])){

                            upper_count++;
                        }
                        if(islower(Password[j])){

                            lower_count++;
                        }
                        if(!isalnum(Password[j])){

                            special_count++;
                        }
                    }
                    if(digit_count < 1 || upper_count < 1 || lower_count < 1 || special_count < 1){

                        validPassword = false;
                    }
                }

                if(!validPassword){

                    cout << "Invalid Password! It must be between 4-12 Character with( atleast 1 uppercase, 1 lowercase, 1 special, 1 didgit)." << endl;
                    cout << "Please try again." << endl;
                    cout << "---------------------------------\n";
                    Sleep(2000);
                    system("cls");
                    count_password++;
                    digit_count = upper_count = lower_count = special_count = 0;
                    continue;
                }
                else{
                    // Reset counts for potential reuse
                    digit_count = upper_count = lower_count = special_count = 0;
                    break;
                }
            }

            int gchoice;
            while(1){

                cout << "1. Male\n2. Female" << endl;
                cout << "Enter Gender: ";
                cin >> gchoice;

                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }

                if(gchoice == 1){

                    Gender = "Male";
                    break;
                }
                else if(gchoice == 2){

                    Gender = "Female";
                    break;
                }
                else{

                    cout << "Invalid choice! Try again." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }
            }

            int fchoice;
            while(1){

                cout << "1. Filler\n2. Non-Filler" << endl;
                cout << "select Filler Status: ";
                cin >> fchoice;

                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }

                if(fchoice == 1){

                    FillerStatus = "Filler";
                    break;
                }
                else if(fchoice ==  2){

                    FillerStatus = "Non-Filler";
                    break;
                }
                else{

                    cout<< "Invalid choice!" << endl;
                    cout<< "Try again." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }
            }

            while(1){

                if(count_contactno > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_contactno = 0;
                    return;
                }
                cout << "Enter Contact Number: ";
                Contact_Number = getDigitsOnly(11);

                bool validContact = true;

                // Check length = 11
                if(Contact_Number.length() != 11){

                    validContact = false;
                }
                else{

                    for(int j = 0; j < 11; j++){

                        if(!isdigit(Contact_Number[j])){

                            validContact = false;
                            break;
                        }
                    }
                }

                if(!validContact){

                    cout << "Invalid Contact Number! It must be exactly 11 digits." << endl;
                    Sleep(2000);
                    system("cls");
                    count_contactno ++;
                    continue;
                }
                else{
                    
                    bool exists = false;
                    Node* check = Head;
                    while(check != NULL){

                        if(check->taxpayer.Contact_Number == Contact_Number){

                            exists = true;
                            break;
                        }
                        check = check->Next;
                    }

                    if(exists){

                        cout << "Contact Number already exists! Please try again." << endl;
                        cout << "---------------------------------\n";
                        Sleep(2000);
                        system("cls");
                        count_contactno++;
                        continue;
                    }
                    else{

                        break;
                    }
                }
            }

            while(1){

                if(count_income > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_income = 0;
                    return;
                }
                
                cout << "Enter Income: ";
                cin >> Income;
                bool validIncome = true;

                for(int j = 0; j < Income.length(); j++){

                    if(!isdigit(Income[j])){

                        validIncome = false;
                        break;
                    }
                }

                if(!validIncome){

                    cout << "Invalid Income! Only digits are allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    count_income ++;
                    continue;
                }
                else{

                    break;
                }
            }
            long long incomeValue = stoll(Income);

            if(incomeValue < 25000){

                IncomeTaxRate = 0.1; // 10%
            }
            else if(incomeValue >= 25000 && incomeValue < 100000){

                IncomeTaxRate = 0.15; // 15%
            }
            else if(incomeValue >= 100000 && incomeValue < 500000){

                IncomeTaxRate = 0.2; // 20%
            }
            else{

                IncomeTaxRate = 0.25; // 25%
            }
            
            asset_label:
            if(count_asset > 2){

                cout << "Too many invalid attempts. Exiting..." << endl;
                Sleep(2000);
                system("cls");
                count_asset = 0;
                return;
            }
            
            cout << "Enter Asset: ";
            cin >> Asset;

            bool validAsset = true;

            for(int j = 0; j < Asset.length(); j++){

                if(!isdigit(Asset[j])){

                    validAsset = false;
                    break;
                }
            }

            if(!validAsset){

                cout << "Invalid Asset! Only digits are allowed." << endl;
                Sleep(2000);
                system("cls");
                count_asset ++;
                goto asset_label;
            }
            long long assetValue = stoll(Asset);

            if(FillerStatus == "Filler"){

                if(assetValue < 500000){

                    AssetTaxRate = 0.10;  // 10%
                }
                else if(assetValue >= 500000 && assetValue < 1500000){

                    AssetTaxRate = 0.15;  // 15%
                }
                else if(assetValue >= 1500000 && assetValue < 10000000){
                    
                    AssetTaxRate = 0.20;  // 20%
                }
                else{

                    AssetTaxRate = 0.25;  // 25%
                }
            }
            else{

                if(assetValue < 500000){

                    AssetTaxRate = 0.15;  // 15%
                }
                else if(assetValue >= 500000 && assetValue < 1500000){

                    AssetTaxRate = 0.20;  // 20%
                }
                else if(assetValue >= 1500000 && assetValue < 10000000){
                    
                    AssetTaxRate = 0.25;  // 25%
                }
                else{

                    AssetTaxRate = 0.30;  // 30%
                }
            }

            Income_Tax = incomeValue * IncomeTaxRate;
            Asset_Tax = assetValue * AssetTaxRate;

            // Insert into linked list
            Insert_Taxpayer(CNIC, Password, Name, Gender, FillerStatus, Contact_Number, Income, Asset, Income_Tax, Asset_Tax);

            cout << "\nUser registered successfully!\n";
            cout << "---------------------------------\n";
            cout << "Name: " << Name << endl;
            cout << "CNIC: " << CNIC << endl;
            cout << "Income Tax(Monthly): " << Income_Tax << endl;
            cout << "Asset Tax(Yearly): " << Asset_Tax << endl;
            cout << "---------------------------------\n";
            system("pause");
            return;
        }

        void Search_taxpayer(){

            string searchCNIC;
            while(1){

                system("cls");
                cout << "-----------------------------------------\n";
                cout << "            SEARCH TAXPAYER\n";
                cout << "-----------------------------------------\n";

                cout << "Enter CNIC to search: ";
                searchCNIC = getDigitsOnly(13);

                if(searchCNIC == ""){

                    cout<< "Enter a CNIC to be checked." << endl;
                    system("pause");
                    continue;
                }
                else{

                    break;
                }
            }
            bool found = false;
            Node* temp = Head;

            while(temp != NULL){

                if (searchCNIC == temp->taxpayer.CNIC){
                    
                    system("cls");
                    cout << "\n-----------------------------------------\n";
                    cout << "          TAXPAYER DETAILS\n";
                    cout << "-----------------------------------------\n";
                    cout << "Name: " << temp->taxpayer.Name << endl;
                    cout << "CNIC: " << temp->taxpayer.CNIC << endl;
                    cout << "Gender: " << temp->taxpayer.Gender << endl;
                    cout << "Filer Status: " << temp->taxpayer.FillerStatus << endl;
                    cout << "Contact: " << temp->taxpayer.Contact_Number << endl;
                    cout << "Income: " << temp->taxpayer.Income<< endl;
                    cout << "Assets: " << temp->taxpayer.Asset << endl;
                    cout << "Income Tax(Monthly): " << temp->taxpayer.Income_Tax << endl;
                    cout << "Asset Tax(Yearly): " << temp->taxpayer.Asset_Tax << endl;
                    cout << "-----------------------------------------\n";
                    system("pause");
                    found = true;
                    break;
                }
                temp = temp->Next;
            }
            if(!found){

                cout << "\nNo taxpayer found with CNIC: " << searchCNIC << endl;
                system("pause");
                return;
            }
            else{

                return;
            }
        }

        void Update_Taxpayer_Details(){

            string Entered_CNIC, password, name, contact, incomevalue, assetvalue, Income_Tax, Asset_Tax;
            Node* temp = Head;
            
            name = temp->taxpayer.Name;
            Contact_Number = temp->taxpayer.Contact_Number;
            incomevalue = temp->taxpayer.Income;
            assetvalue = temp->taxpayer.Asset;
            Password = temp->taxpayer.Password;

            while(1){

                system("cls");
                cout<< "---------------------------------\n";
                cout<< "     UPDATE TAXPAYER DETAILS\n";
                cout<< "---------------------------------\n";

                cout << "Enter CNIC of taxpayer to update: ";
                Entered_CNIC = getDigitsOnly(13);

                if(Entered_CNIC == ""){

                    cout<< "Enter a CNIC to be updated." << endl;
                    system("pause");
                    continue;
                }
                else{

                    break;
                }
            }

            while (temp != NULL)
            {
                if(temp->taxpayer.CNIC == Entered_CNIC){

                    int choice;
                    while (1){

                        system("cls");
                        cout << "What do you want to update?" << endl;
                        cout << "1. Name\n2. Contact Number\n3. Income\n4. Asset\n5. Password\n6. Save & Exit" << endl;
                        cout << "Enter your choice: ";
                        cin >> choice;

                        if(cin.fail()){

                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid input! Only numbers allowed." << endl;
                            Sleep(2000);
                            system("cls");
                            continue;
                        }

                        if(choice == 1){

                            cout << "Enter new Name: ";
                            cin >> ws;
                            getline(cin, name);
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }
                        else if(choice == 2){

                            int count_contactno = 0;
                            while(1){

                                if(count_contactno > 2){

                                    cout << "Too many invalid attempts. Exiting..." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_contactno = 0;
                                    return;
                                }

                                cout << "Enter Contact Number: ";
                                Contact_Number = getDigitsOnly(11);

                                bool validContact = true;
                                // Check length = 11
                                if(Contact_Number.length() != 11){

                                    validContact = false;
                                }
                                else{

                                    for(int j = 0; j < 11; j++){

                                        if(!isdigit(Contact_Number[j])){

                                            validContact = false;
                                            break;
                                        }
                                    }
                                }

                                if(!validContact){

                                    cout << "Invalid Contact Number! It must be exactly 11 digits." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_contactno ++;
                                    continue;
                                }
                                else{

                                    bool exists = false;
                                    Node* check = Head;
                                    while(check != NULL){

                                        if(check->taxpayer.Contact_Number == Contact_Number){

                                            exists = true;
                                            break;
                                        }
                                        check = check->Next;
                                    }

                                    if(exists){

                                        cout << "Contact Number already exists! Please try again." << endl;
                                        cout << "---------------------------------\n";
                                        Sleep(2000);
                                        system("cls");
                                        count_contactno++;
                                        continue;
                                    }
                                    else{

                                        cout<< "Change Successful!" << endl;
                                        Sleep(1500);
                                        break;
                                    }
                                }
                            }
                            continue;
                        }
                        else if(choice == 3){

                            int count_income = 0;
                            while(1){

                                if(count_income > 2){

                                    cout << "Too many invalid attempts. Exiting..." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_income = 0;
                                    return;
                                }
                                cout << "Enter Income: ";
                                cin >> Income;

                                bool validIncome = true;
                                for(int j = 0; j < Income.length(); j++){

                                    if(!isdigit(Income[j])){

                                        validIncome = false;
                                        break;
                                    }
                                }

                                if(!validIncome){

                                    cout << "Invalid Income! Only digits are allowed." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_income ++;
                                    continue;
                                }
                            }
                            long long incValue = stoll(incomevalue);
                                
                            if(incValue < 25000){

                                IncomeTaxRate = 0.1; // 10%
                            }
                            else if(incValue >= 25000 && incValue < 100000){

                                IncomeTaxRate = 0.15; // 15%
                            }
                            else if(incValue >= 100000 && incValue < 500000){

                                IncomeTaxRate = 0.2; // 20%
                            }
                            else{

                                IncomeTaxRate = 0.25; // 25%
                            }
                            long double newTax = incValue * IncomeTaxRate;
                            incomevalue = to_string(newTax);
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }
                        else if(choice == 4){

                            int count_asset = 0;
                            asset_label:
                            if(count_asset > 2){

                                cout << "Too many invalid attempts. Exiting..." << endl;
                                Sleep(2000);
                                system("cls");
                                count_asset = 0;
                                return;
                            }
                            cout << "Enter Asset: ";
                            cin >> Asset;

                            bool validAsset = true;

                            for(int j = 0; j < Asset.length(); j++){

                                if(!isdigit(Asset[j])){

                                    validAsset = false;
                                    break;
                                }
                            }

                            if(!validAsset){

                                cout << "Invalid Asset! Only digits are allowed." << endl;
                                Sleep(2000);
                                system("cls");
                                count_asset ++;
                                goto asset_label;
                            }
                            long long assetValue = stoll(Asset);

                            if(temp->taxpayer.FillerStatus == "Filler"){

                                if(assetValue < 500000){

                                    AssetTaxRate = 0.10;  // 10%
                                }
                                else if(assetValue >= 500000 && assetValue < 1500000){

                                    AssetTaxRate = 0.15;  // 15%
                                }
                                else if(assetValue >= 1500000 && assetValue < 10000000){
                                    
                                    AssetTaxRate = 0.20;  // 20%
                                }
                                else{

                                    AssetTaxRate = 0.25;  // 25%
                                }
                            }
                            else{

                                if(assetValue < 500000){

                                    AssetTaxRate = 0.15;  // 15%
                                }
                                else if(assetValue >= 500000 && assetValue < 1500000){

                                    AssetTaxRate = 0.20;  // 20%
                                }
                                else if(assetValue >= 1500000 && assetValue < 10000000){
                                    
                                    AssetTaxRate = 0.25;  // 25%
                                }
                                else{

                                    AssetTaxRate = 0.30;  // 30%
                                }
                            }
                            long double newAssetTax = assetValue * AssetTaxRate;
                            assetvalue = to_string(newAssetTax);
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }
                        else if(choice == 5){

                            int count_password = 0, digit_count = 0, upper_count = 0, lower_count = 0, special_count = 0;
                            while(1){

                                if(count_password > 2){

                                    cout << "Too many invalid attempts. Exiting..." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_password = 0;
                                    return;
                                }
                                cout<< "New Password should have:" << endl;
                                cout<< "1. Must be between 4-12 Characters\n2. At least one uppercase letter" << endl;
                                cout<< "3. At least one lowercase letter\n4. At least one digit\n5. At least one special character" << endl;
                                cout << "Set Password: ";
                                cin >> Password;

                                bool validPassword = false;

                                if(Password.length() >= 4 && Password.length() <= 12){

                                    validPassword = true;
                                }
                                if(validPassword){

                                    for(int j = 0; j < Password.length(); j++){

                                        if(isdigit(Password[j])){

                                            digit_count++;
                                        }
                                        if(isupper(Password[j])){

                                            upper_count++;
                                        }
                                        if(islower(Password[j])){

                                            lower_count++;
                                        }
                                        if(!isalnum(Password[j])){

                                            special_count++;
                                        }
                                    }
                                    if(digit_count < 1 || upper_count < 1 || lower_count < 1 || special_count < 1){

                                        validPassword = false;
                                    }
                                }

                                if(!validPassword){

                                    cout << "Invalid Password! It must be exactly 8 Character with( atleast 1 uppercase, 1 lowercase, 1 special, 1 didgit)." << endl;
                                    cout << "Please try again." << endl;
                                    cout << "---------------------------------\n";
                                    digit_count = upper_count = lower_count = special_count = 0;
                                    Sleep(2000);
                                    system("cls");
                                    count_password++;
                                    continue;
                                }
                            }
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }

                        else if(choice == 6){

                            // Update the taxpayer details in the linked list
                            temp->taxpayer.Name = name;
                            temp->taxpayer.Contact_Number = Contact_Number;
                            temp->taxpayer.Income = incomevalue;
                            temp->taxpayer.Asset = assetvalue;
                            temp->taxpayer.Password = Password;
                            cout << "Profile updated successfully!" << endl;
                            system("pause");
                            return;
                        }
                        else{

                            cout << "Invalid choice!" << endl;
                            system("pause");
                        }
                    }
                }
            }
        }

        void Remove_Taxpayer(){

            system("cls");
            string targetCNIC;
            while(1){

                cout << "---------------------------------\n";
                cout << "      DELETE TAXPAYER\n";
                cout << "---------------------------------\n";
                cout << "Enter CNIC to delete: ";
                targetCNIC = getDigitsOnly(13);
                if(targetCNIC == ""){

                    cout<< "Enter a CNIC to be deleted." << endl;
                    system("pause");
                    continue;
                }
                else{

                    break;
                }
            }

            int result = Delete_Taxpayer(targetCNIC);

            if(result == 0){

                cout << "No Taxpayer are Registered!" << endl;
                system("pause");
                return;
            }
            else if(result == 1){

                cout << "No Taxpayer with Entered CNIC!" << endl;
                system("pause");
                return;
            }
            else if(result == 2){

                cout << "Taxpayer deleted successfully.\n";
                system("pause");
                return;
            }
        }

        void Update_Record(){
        
            while(1){

                system("cls");
                int choice;
                cout<< "1. Update Taxpayer Details\n2. Remove Taxpayer\n3. Exit" << endl;
                cout<< "Enter your choice: ";
                cin >> choice;
                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }

                if(choice == 1){

                    system("cls");
                    Update_Taxpayer_Details();
                }
                else if(choice == 2){

                    system("cls");
                    Remove_Taxpayer();
                }
                else if(choice == 3){

                    return;
                }
                else{

                    cout << "Invalid choice!" << endl;
                    continue;
                }
            }
        }
        
        void Admin_menu(){

            system("cls");
            int check = adminLogin();
            int choice;
            if(check){

                while (1)
                {
                    system("cls");
                    cout << "Welcome to Admin Menu!" << endl;
                    cout << "1. Add taxpayer\n2. View all taxpayers\n3. Search taxpayer\n4. Update Record\n5. Exit" << endl;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    if(cin.fail()){

                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input! Only numbers allowed." << endl;
                        Sleep(2000);
                        system("cls");
                        continue;
                    }
                
                    if(choice == 1){

                        system("cls");
                        Add_taxpayer();
                        Write_data_To_File();
                    }
                    else if(choice == 2){
                    
                        system("cls");
                        DisplayAlltaxpayers();
                    }
                    else if(choice == 3){

                        system("cls");
                        Search_taxpayer();
                    }
                    else if(choice == 4){

                        system("cls");
                        Update_Record();
                        Write_data_To_File();
                    }
                    else if(choice == 5){

                        Write_data_To_File();
                        return;
                    }
                    else{
                    
                        cout << "Invalid choice!" << endl;
                        continue;
                    }
                }
            }
            else{

                return;
            }
        }
};

class User{

    public:

        string Name, Gender, FillerStatus, CNIC, Contact_Number, Income, Asset, Password;
        long double IncomeTaxRate, AssetTaxRate, Income_Tax, Asset_Tax;

        void UserRegistration(){

            system("cls");
            int count_cnic = 0, count_name = 0, count_contactno = 0, count_income = 0, count_asset = 0, count_password = 0;

            cout << "User Registration!" << endl;
            cout << "---------------------------------\n";

            while(1){

                if(count_name > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_name = 0;
                    return;
                }

                bool validName = true;
                cout << "Enter Name: ";
                cin >> ws; // to ignore leftover newline
                getline(cin, Name);

                for(int i = 0; i < Name.length(); i++){

                    if(!isalpha(Name[i]) && Name[i] != ' '){

                        validName =  false;
                    }
                }

                if(!validName){

                    cout << "Invalid Name! The name must use Upper and Lower Case Character." << endl;
                    cout << "Please try again." << endl;
                    cout << "---------------------------------\n";
                    Sleep(2000);
                    system("cls");
                    count_name++;
                }
                else{

                    break;
                }
            }

            while(1){

                if(count_cnic > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_cnic = 0;
                    return;
                }
                cout << "Enter CNIC (without dashes): ";
                CNIC = getDigitsOnly(13);

                bool validCNIC = true;

                // Check length = 13
                if(CNIC.length() != 13){

                    validCNIC = false;
                }
                else{

                    for(int j = 0; j < 13; j++){

                        if(!isdigit(CNIC[j])){

                            validCNIC = false;
                            break;
                        }
                    }
                }

                if(!validCNIC){

                    cout << "Invalid CNIC! It must be exactly 13 digits." << endl;
                    cout << "Please try again." << endl;
                    cout << "---------------------------------\n";
                    Sleep(2000);
                    system("cls");
                    count_cnic++;
                    continue;
                }
                else{

                    // Check if CNIC already exists

                    bool exists = false;
                    Node* temp = Head;
                    while(temp != NULL){

                        if(temp->taxpayer.CNIC == CNIC){

                            exists = true;
                            break;
                        }
                        temp = temp->Next;
                    }

                    if(exists){

                        cout << "CNIC already exists! Please try again." << endl;
                        cout << "---------------------------------\n";
                        Sleep(2000);
                        system("cls");
                        count_cnic++;
                    }
                    else{

                        break;
                    }
                }
            }

            int digit_count = 0, upper_count = 0, lower_count = 0, special_count = 0;
            while(1){

                if(count_password > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_password = 0;
                    return;
                }
                cout<< "Password should have:" << endl;
                cout<< "1. Must be between 4-12 Characters\n2. At least one uppercase letter" << endl;
                cout<< "3. At least one lowercase letter\n4. At least one digit\n5. At least one special character" << endl;
                cout << "Set Password: ";
                cin >> Password;

                bool validPassword = false;

                if(Password.length() >= 4 && Password.length() <= 12){

                    validPassword = true;
                }
                if(validPassword){

                    for(int j = 0; j < Password.length(); j++){

                        if(isdigit(Password[j])){

                            digit_count++;
                        }
                        if(isupper(Password[j])){

                            upper_count++;
                        }
                        if(islower(Password[j])){

                            lower_count++;
                        }
                        if(!isalnum(Password[j])){

                            special_count++;
                        }
                    }
                    if(digit_count < 1 || upper_count < 1 || lower_count < 1 || special_count < 1){

                        validPassword = false;
                    }
                }

                if(!validPassword){

                    cout << "Invalid Password! It must be between 4-12 Character with( atleast 1 uppercase, 1 lowercase, 1 special, 1 didgit)." << endl;
                    cout << "Please try again." << endl;
                    cout << "---------------------------------\n";
                    Sleep(2000);
                    system("cls");
                    count_password++;
                    digit_count = upper_count = lower_count = special_count = 0;
                    continue;
                }
                else{
                    // Reset counts for potential reuse
                    digit_count = upper_count = lower_count = special_count = 0;
                    break;
                }
            }

            int gchoice;
            while(1){

                cout << "1. Male\n2. Female" << endl;
                cout << "Enter Gender: ";
                cin >> gchoice;

                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }

                if(gchoice == 1){

                    Gender = "Male";
                    break;
                }
                else if(gchoice == 2){

                    Gender = "Female";
                    break;
                }
                else{

                    cout << "Invalid choice! Try again." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }
            }

            int fchoice;
            while(1){

                cout << "1. Filler\n2. Non-Filler" << endl;
                cout << "select Filler Status: ";
                cin >> fchoice;

                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }

                if(fchoice == 1){

                    FillerStatus = "Filler";
                    break;
                }
                else if(fchoice ==  2){

                    FillerStatus = "Non-Filler";
                    break;
                }
                else{

                    cout<< "Invalid choice!" << endl;
                    cout<< "Try again." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }
            }

            while(1){

                if(count_contactno > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_contactno = 0;
                    return;
                }
                cout << "Enter Contact Number: ";
                Contact_Number = getDigitsOnly(11);

                bool validContact = true;

                // Check length = 11
                if(Contact_Number.length() != 11){

                    validContact = false;
                }
                else{

                    for(int j = 0; j < 11; j++){

                        if(!isdigit(Contact_Number[j])){

                            validContact = false;
                            break;
                        }
                    }
                }

                if(!validContact){

                    cout << "Invalid Contact Number! It must be exactly 11 digits." << endl;
                    Sleep(2000);
                    system("cls");
                    count_contactno ++;
                    continue;
                }
                else{
                    
                    bool exists = false;
                    Node* temp = Head;
                    while(temp != NULL){

                        if(temp->taxpayer.Contact_Number == Contact_Number){

                            exists = true;
                            break;
                        }
                        temp = temp->Next;
                    }

                    if(exists){

                        cout << "Contact Number already exists! Please try again." << endl;
                        cout << "---------------------------------\n";
                        Sleep(2000);
                        system("cls");
                        count_contactno++;
                        continue;
                    }
                    else{

                        break;
                    }
                }
            }

            while(1){

                if(count_income > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_income = 0;
                    return;
                }
                
                cout << "Enter Income: ";
                cin >> Income;
                bool validIncome = true;

                for(int j = 0; j < Income.length(); j++){

                    if(!isdigit(Income[j])){

                        validIncome = false;
                        break;
                    }
                }

                if(!validIncome){

                    cout << "Invalid Income! Only digits are allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    count_income ++;
                    continue;
                }
                else{

                    break;
                }
            }
            long long incomeValue = stoll(Income);

            if(incomeValue < 25000){

                IncomeTaxRate = 0.1; // 10%
            }
            else if(incomeValue >= 25000 && incomeValue < 100000){

                IncomeTaxRate = 0.15; // 15%
            }
            else if(incomeValue >= 100000 && incomeValue < 500000){

                IncomeTaxRate = 0.2; // 20%
            }
            else{

                IncomeTaxRate = 0.25; // 25%
            }
            
            while(1){

                if(count_asset > 2){

                    cout << "Too many invalid attempts. Exiting..." << endl;
                    Sleep(2000);
                    system("cls");
                    count_asset = 0;
                    return;
                }
                
                cout << "Enter Asset: ";
                cin >> Asset;

                bool validAsset = true;

                for(int j = 0; j < Asset.length(); j++){

                    if(!isdigit(Asset[j])){

                        validAsset = false;
                        break;
                    }
                }

                if(!validAsset){

                    cout << "Invalid Asset! Only digits are allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    count_asset ++;
                }
                else{

                    break;
                }
            }
            long long assetValue = stoll(Asset);

            if(FillerStatus == "Filler"){

                if(assetValue < 500000){

                    AssetTaxRate = 0.10;  // 10%
                }
                else if(assetValue >= 500000 && assetValue < 1500000){

                    AssetTaxRate = 0.15;  // 15%
                }
                else if(assetValue >= 1500000 && assetValue < 10000000){
                    
                    AssetTaxRate = 0.20;  // 20%
                }
                else{

                    AssetTaxRate = 0.25;  // 25%
                }
            }
            else{

                if(assetValue < 500000){

                    AssetTaxRate = 0.15;  // 15%
                }
                else if(assetValue >= 500000 && assetValue < 1500000){

                    AssetTaxRate = 0.20;  // 20%
                }
                else if(assetValue >= 1500000 && assetValue < 10000000){
                    
                    AssetTaxRate = 0.25;  // 25%
                }
                else{

                    AssetTaxRate = 0.30;  // 30%
                }
            }

            Income_Tax = incomeValue * IncomeTaxRate;
            Asset_Tax = assetValue * AssetTaxRate;

            // Save to file
            ofstream file("taxpayers.txt", ios::app);
            if(!file){

                cout << "Error: Could not open file to save data.\n";
                return;
            }

            // Insert into linked list
            Insert_Taxpayer(CNIC, Password, Name, Gender, FillerStatus, Contact_Number, Income, Asset, Income_Tax, Asset_Tax);

            cout << "\nUser registered successfully!\n";
            cout << "---------------------------------\n";
            cout << "Name: " << Name << endl;
            cout << "CNIC: " << CNIC << endl;
            cout << "Income Tax(Monthly): " << Income_Tax << endl;
            cout << "Asset Tax(Yearly): " << Asset_Tax << endl;
            cout << "---------------------------------\n";
            system("pause");
            return;
        }

        int searchUserByCNIC(const string& targetCNIC){

            bool found = false;
            Node* temp = Head;

            while(temp != NULL){

                if(temp->taxpayer.CNIC == targetCNIC){

                    found = true;
                    userPassword = temp->taxpayer.Password;
                    return 0;
                }
                temp = temp->Next;
            }

            if(!found){

                return 1;
            }
        }

        void VeiwProfile(){

            system("cls");
            Node* temp = Head;
            while(temp != NULL){

                if(temp->taxpayer.CNIC == openedCNIC){

                    cout << "---------------------------------\n";
                    cout << "Name: " << temp->taxpayer.Name << endl;
                    cout << "CNIC: " << temp->taxpayer.CNIC << endl;
                    cout << "Gender: " << temp->taxpayer.Gender << endl;
                    cout << "Filler Status: " << temp->taxpayer.FillerStatus << endl;
                    cout << "Contact Number: " << temp->taxpayer.Contact_Number << endl;
                    cout << "Income: " << temp->taxpayer.Income << endl;
                    cout << "Asset: " << temp->taxpayer.Asset << endl;
                    cout << "Income Tax(Monthly): " << temp->taxpayer.Income_Tax<< endl;
                    cout << "Asset Tax(Yearly): " << temp->taxpayer.Asset_Tax<< endl;
                    cout<<"---------------------------------\n";
                    system("pause");
                }
                temp = temp->Next;
            }
        }

        void UpdateProfile(){

            system("cls");
            string Entered_CNIC, password, name, contact, incomevalue, assetvalue, Income_Tax, Asset_Tax;
            Node* temp = Head;

            name = temp->taxpayer.Name;
            Contact_Number = temp->taxpayer.Contact_Number;
            incomevalue = temp->taxpayer.Income;
            assetvalue = temp->taxpayer.Asset;
            Password = temp->taxpayer.Password;

            while(temp != NULL){
                
                if(temp->taxpayer.CNIC == openedCNIC){

                    int choice;
                    while (1){

                        system("cls");
                        cout<< "---------------------------------\n";
                        cout<< "     UPDATE DETAILS\n";
                        cout<< "---------------------------------\n";
                        cout << "What do you want to update?" << endl;
                        cout << "1. Name\n2. Contact Number\n3. Income\n4. Asset\n5. Password\n6. Save & Exit" << endl;
                        cout << "Enter your choice: ";
                        cin >> choice;

                        if(cin.fail()){

                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid input! Only numbers allowed." << endl;
                            Sleep(2000);
                            system("cls");
                            continue;
                        }

                        if(choice == 1){

                            system("cls");
                            cout << "Enter new Name: ";
                            cin >> ws;
                            getline(cin, name);
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }
                        else if(choice == 2){

                            int count_contactno = 0;
                            while(1){

                                if(count_contactno > 2){

                                    cout << "Too many invalid attempts. Exiting..." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_contactno = 0;
                                    return;
                                }

                                system("cls");
                                cout << "Enter Contact Number: ";
                                Contact_Number = getDigitsOnly(11);

                                bool validContact = true;
                                // Check length = 11
                                if(Contact_Number.length() != 11){

                                    validContact = false;
                                }
                                else{

                                    for(int j = 0; j < 11; j++){

                                        if(!isdigit(Contact_Number[j])){

                                            validContact = false;
                                            break;
                                        }
                                    }
                                }

                                if(!validContact){

                                    cout << "Invalid Contact Number! It must be exactly 11 digits." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_contactno ++;
                                    continue;
                                }
                                else{

                                    bool exists = false;
                                    Node* scan = Head;

                                    while(scan != NULL){

                                        // Skip the node being updated
                                        if(scan->taxpayer.CNIC != temp->taxpayer.CNIC){

                                            if(scan->taxpayer.Contact_Number == Contact_Number){

                                                exists = true;
                                                break;
                                            }
                                        }
                                        scan = scan->Next;
                                    }

                                    if(exists){

                                        cout << "Contact Number already exists! Please try again." << endl;
                                        Sleep(2000);
                                        continue;
                                    }
                                    else{

                                        
                                        cout<< "Change Successful!" << endl;
                                        Sleep(1500);
                                        break;
                                    }
                                }
                            }
                            continue;
                        }
                        else if(choice == 3){

                            int count_income = 0;
                            while(1){

                                if(count_income > 2){

                                    cout << "Too many invalid attempts. Exiting..." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_income = 0;
                                    return;
                                }
                                cout << "Enter Income: ";
                                cin >> Income;

                                bool validIncome = true;
                                for(int j = 0; j < Income.length(); j++){

                                    if(!isdigit(Income[j])){

                                        validIncome = false;
                                        break;
                                    }
                                }

                                if(!validIncome){

                                    cout << "Invalid Income! Only digits are allowed." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_income ++;
                                    continue;
                                }
                            }
                            long long incValue = stoll(incomevalue);
                                
                            if(incValue < 25000){

                                IncomeTaxRate = 0.1; // 10%
                            }
                            else if(incValue >= 25000 && incValue < 100000){

                                IncomeTaxRate = 0.15; // 15%
                            }
                            else if(incValue >= 100000 && incValue < 500000){

                                IncomeTaxRate = 0.2; // 20%
                            }
                            else{

                                IncomeTaxRate = 0.25; // 25%
                            }
                            long double newTax = incValue * IncomeTaxRate;
                            incomevalue = to_string(newTax);
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }
                        else if(choice == 4){

                            int count_asset = 0;
                            asset_label:
                            if(count_asset > 2){

                                cout << "Too many invalid attempts. Exiting..." << endl;
                                Sleep(2000);
                                system("cls");
                                count_asset = 0;
                                return;
                            }
                            cout << "Enter Asset: ";
                            cin >> Asset;

                            bool validAsset = true;

                            for(int j = 0; j < Asset.length(); j++){

                                if(!isdigit(Asset[j])){

                                    validAsset = false;
                                    break;
                                }
                            }

                            if(!validAsset){

                                cout << "Invalid Asset! Only digits are allowed." << endl;
                                Sleep(2000);
                                system("cls");
                                count_asset ++;
                                goto asset_label;
                            }
                            long long assetValue = stoll(Asset);

                            if(temp->taxpayer.FillerStatus == "Filler"){

                                if(assetValue < 500000){

                                    AssetTaxRate = 0.10;  // 10%
                                }
                                else if(assetValue >= 500000 && assetValue < 1500000){

                                    AssetTaxRate = 0.15;  // 15%
                                }
                                else if(assetValue >= 1500000 && assetValue < 10000000){
                                    
                                    AssetTaxRate = 0.20;  // 20%
                                }
                                else{

                                    AssetTaxRate = 0.25;  // 25%
                                }
                            }
                            else{

                                if(assetValue < 500000){

                                    AssetTaxRate = 0.15;  // 15%
                                }
                                else if(assetValue >= 500000 && assetValue < 1500000){

                                    AssetTaxRate = 0.20;  // 20%
                                }
                                else if(assetValue >= 1500000 && assetValue < 10000000){
                                    
                                    AssetTaxRate = 0.25;  // 25%
                                }
                                else{

                                    AssetTaxRate = 0.30;  // 30%
                                }
                            }
                            long double newAssetTax = assetValue * AssetTaxRate;
                            assetvalue = to_string(newAssetTax);
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }
                        else if(choice == 5){

                            int count_password = 0, digit_count = 0, upper_count = 0, lower_count = 0, special_count = 0;
                            while(1){

                                if(count_password > 2){

                                    cout << "Too many invalid attempts. Exiting..." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    count_password = 0;
                                    digit_count = upper_count = lower_count = special_count = 0;
                                    return;
                                }
                                cout<< "New Password should have:" << endl;
                                cout<< "1. Must be between 4-12 Characters\n2. At least one uppercase letter" << endl;
                                cout<< "3. At least one lowercase letter\n4. At least one digit\n5. At least one special character" << endl;
                                cout << "Set Password: ";
                                cin >> Password;

                                bool validPassword = false;

                                // Check length = 8
                                if(Password.length() >= 4 && Password.length() <= 12){

                                    validPassword = true;
                                }
                                if(validPassword){

                                    for(int j = 0; j < Password.length(); j++){

                                        if(isdigit(Password[j])){

                                            digit_count++;
                                        }
                                        if(isupper(Password[j])){

                                            upper_count++;
                                        }
                                        if(islower(Password[j])){

                                            lower_count++;
                                        }
                                        if(!isalnum(Password[j])){

                                            special_count++;
                                        }
                                    }
                                    if(digit_count < 1 || upper_count < 1 || lower_count < 1 || special_count < 1){

                                        validPassword = false;
                                    }
                                }

                                if(!validPassword){

                                    cout << "Invalid Password! It must be exactly 8 Character with( atleast 1 uppercase, 1 lowercase, 1 special, 1 didgit)." << endl;
                                    cout << "Please try again." << endl;
                                    cout << "---------------------------------\n";
                                    digit_count = upper_count = lower_count = special_count = 0;
                                    Sleep(2000);
                                    system("cls");
                                    count_password++;
                                    continue;
                                }
                            }
                            cout<< "Change Successful!" << endl;
                            Sleep(1500);
                            continue;
                        }

                        else if(choice == 6){

                            // Update the taxpayer details in the linked list
                            temp->taxpayer.Name = name;
                            temp->taxpayer.Contact_Number = Contact_Number;
                            temp->taxpayer.Income = incomevalue;
                            temp->taxpayer.Asset = assetvalue;
                            temp->taxpayer.Password = Password;
                            cout << "Profile updated successfully!" << endl;
                            system("pause");
                            return;
                        }
                        else{

                            cout << "Invalid choice!" << endl;
                            system("pause");
                        }
                    }
                }
                else{

                    temp = temp->Next;
                }
            }
        }

        void CalculateTax(){

            system("cls");
            ofstream taxfile("tax.txt", ios::app);
            if (!taxfile.is_open()){

                cout << "Error: Could not open file.\n";
                system("pause");
                return;
            }
            long double incomevalue, assetvalue, Income_Tax, Asset_Tax, tax = 0.0;
            string interval, Receipt_no;
            Node* temp = Head;
            
            while (temp != NULL){

                if(temp->taxpayer.CNIC == openedCNIC){

                    Income_Tax = temp->taxpayer.Income_Tax;
                    Asset_Tax = temp->taxpayer.Asset_Tax;
                    break;
                }
                temp = temp->Next;
            }

            while(1){

                int choice;
                cout<< "1. 3 Months\n2. 6 Months\n3. 1 Year\n4. Exit"<< endl;
                cout<< "Select time period to calculate tax: ";
                cin>> choice;

                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }

                if( choice == 1){

                    system("cls");
                    tax = Income_Tax*3;
                    interval = "3 months";
                    cout<< "Tax for 3 months is: "<< tax << endl;
                    system("pause");
                    break;
                }
                else if( choice == 2){

                    system("cls");
                    tax = Income_Tax*6;
                    interval = "6 months";
                    cout<< "Tax for 6 months is: "<< tax << endl;
                    system("pause");
                    break;
                }
                else if( choice == 3){

                    system("cls");
                    tax = Income_Tax*12;
                    tax = tax + Asset_Tax;
                    interval = "1 Financial Year";
                    cout<< "Tax for 1 year is: "<< tax << endl;
                    system("pause"); 
                    break;
                }
                else if( choice == 4){

                    cout << "Exiting..." << endl;
                    return;
                }
                else{

                    cout << "Invalid choice!" << endl;
                    system("pause");
                    continue;
                }
            }
            int check;
            ostringstream oss;
            oss << fixed << std::setprecision(0) << tax;
            string ftax = oss.str();
            int Generater_num = GenerateRandom4Digit();
            Receipt_no = to_string(Generater_num);
            string Receipt_Generated = Receipt_no;
            cout<< "Generating Tax Receipt..." << endl;
            taxfile << Receipt_no << "," << temp->taxpayer.CNIC << "," << temp->taxpayer.Name << "," << interval << "," << ftax << endl;
            cout<< "---------------------------------\n";
            while(1){

                cout<< "1. View Tax Receipt\n2. Exit"<< endl;
                cout<< "Enter your choice: ";
                cin>> check;

                if(cin.fail()){

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Only numbers allowed." << endl;
                    Sleep(2000);
                    system("cls");
                    continue;
                }
                else{

                    break;
                }
            }
            if(check == 1){

                ViewTaxReceipt(Receipt_Generated);
            }
            else if(check == 2){

                return;
            }
            else{

                cout << "Invalid Option!" << endl;
            }
            taxfile.close();
        }

        void ViewTaxReceipt(string RG){

            system("cls");
            ifstream taxfile("tax.txt");

            string t_cnic, t_name, t_interval, t_tax, Receipt_no;
            bool found = false;

            while(getline(taxfile, Receipt_no, ',')){

                getline(taxfile, t_cnic, ',');
                getline(taxfile, t_name, ',');
                getline(taxfile, t_interval, ',');
                getline(taxfile, t_tax);

                if(Receipt_no == RG){
                    
                    cout << "---------------------------------\n";
                    cout << "Tax Receipt"<<endl;
                    cout << "Receipt No.:"<< Receipt_no <<endl;
                    cout << "Name: " << t_name << endl;
                    cout << "CNIC: " << t_cnic << endl;
                    cout << "Time Period: " << t_interval << endl;
                    cout << "Total Tax: " << t_tax << endl;
                    cout << "---------------------------------\n";
                    system("pause");
                    found = true;
                    break;
                }
            }
            if (!found){

                cout << "No tax receipt found!\n";
                system("pause");
            }
            taxfile.close();
            return;
        }

        void ViewTaxPaymentHistory(){

            system("cls");
            ifstream taxfile("tax.txt");

            string t_cnic, t_name, t_interval, t_tax, Receipt_no;
            bool found = false;

            while(getline(taxfile, Receipt_no, ',')){

                getline(taxfile, t_cnic, ',');
                getline(taxfile, t_name, ',');
                getline(taxfile, t_interval, ',');
                getline(taxfile, t_tax);

                if(t_cnic == openedCNIC){
                    
                    cout << "---------------------------------\n";
                    cout << "Tax Receipt:" << Receipt_no <<endl;
                    cout << "Name: " << t_name << endl;
                    cout << "CNIC: " << t_cnic << endl;
                    cout << "Time Period: " << t_interval << endl;
                    cout << "Total Tax: " << t_tax << endl;
                    cout << "---------------------------------\n" << endl;
                    found = true;
                }
            }
            system("pause");
            if (!found){

                cout << "No tax receipt found!\n";
                system("pause");
            }
            taxfile.close();
            return;
        }

        void existinguser(){

            system("cls");
            string targetCNIC, enteredPassword;
            int check, sc;
            openedCNIC = "";
            userPassword = "";
            cout << "Enter CNIC (without dashes): ";
            targetCNIC = getDigitsOnly(13); 
            sc = searchUserByCNIC(targetCNIC);
            cout << "Enter Password: ";
            cin >> enteredPassword;
            if(enteredPassword != userPassword){

                cout << "Incorrect Password!" << endl;
                system("pause");
                return;
            }
            else{

                cout << "Login Successful!" << endl;
                system("pause");
                if (sc == 0){

                    while(1){

                        system("cls");
                        openedCNIC = targetCNIC;
                        cout << "1. View Profile\n2. Update Profile\n3. Calculate Tax" << endl;
                        cout << "4. View tax payment history\n5. Exit" << endl;
                        cout << "Please select an option: ";
                        cin >> check;
                        if(cin.fail()){

                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid input! Only numbers allowed." << endl;
                            Sleep(2000);
                            system("cls");
                            continue;
                        }
                        if (check == 1){

                            VeiwProfile();
                            continue;
                        }
                        else if (check == 2){

                            UpdateProfile();
                            Write_data_To_File();
                            continue;
                        }
                        else if (check == 3){

                            system("cls");
                            CalculateTax();
                            continue;
                        }
                        else if (check == 4){

                            system("cls");
                            ViewTaxPaymentHistory();
                            continue;
                        }
                        else if (check == 5){

                            cout << "Exiting..." << endl;
                            openedCNIC = "";
                            userPassword = "";
                            Write_data_To_File();
                            Sleep(2000);
                            return;
                        }
                        else{

                            cout << "Invalid choice!" << endl;
                            continue;
                        }
                    }
                }
                else if (sc == 1)
                {

                    cout << "User not found!" << endl;
                    system("pause");
                    return;
                }
            }
        }

        void UserMenu(){

            int choice, c;
            system("cls");
            while (1)
            {
                system("cls");
                cout << "Welcome to the User Menu!" << endl;
                while(1){

                    cout << "1. New User\n2. Exsisting user\n3. Exit" << endl;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    if(cin.fail()){

                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input! Only numbers allowed." << endl;
                        Sleep(2000);
                        system("cls");
                        continue;
                    
                    }
                    else{

                        break;
                    }
                }
                if(choice == 1){

                    system("cls");
                    UserRegistration();
                    Write_data_To_File();
                    continue;
                }
                else if(choice == 2){

                    while(1){
                        
                        system("cls");
                        if(isEmpty()){

                            cout << "NO Registered Users!" << endl;
                            system("pause");
                            break;
                        }
                        else{
                            
                            while(1){

                                cout<< "1. Proceed to your account\n2. Back to main menu"<<endl;
                                cout<< "Enter your choice: ";
                                cin>>c;
                                if(cin.fail()){

                                    cin.clear();
                                    cin.ignore(1000, '\n');
                                    cout << "Invalid input! Only numbers allowed." << endl;
                                    Sleep(2000);
                                    system("cls");
                                    continue;
                                }
                                else{

                                    break;
                                }
                            }
                            if(c == 1){

                                existinguser();
                                break;
                            }
                            else if(c == 2){

                                break;
                            }
                            else{

                                cout << "Invalid choice!" << endl;
                                continue;
                            }
                        }
                    }
                }
                else if(choice == 3){

                    Write_data_To_File();
                    return;
                }
                else{

                    cout << "Invalid choice!" << endl;
                }
            }
        }
};

int main() {

    Admin admin; 
    User user;
    srand(time(0));
    int choice;
    Read_data_From_File();

    while(true){

        system("cls");
        cout << "======================" << endl;
        cout << " Tax Management System" << endl;
        cout << "======================" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. User Menu" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if(cin.fail()){

            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Only numbers allowed." << endl;
            Sleep(2000);
            system("cls");
            continue;
        }

        if(choice == 1){
        	
            system("cls");
            admin.Admin_menu();
            continue;
        }
        else if(choice == 2){
        	
            system("cls");
            user.UserMenu();
            continue;
        }
        else if(choice == 3){
        	
            Write_data_To_File();
            Free_Space();
            cout << "Exiting program" << endl;
            break; // Exit the loop and program
        }
        else {
        	
            cout << "Invalid choice! " << endl;
            system("pause");
            continue;
        }
    }
    return 0;
}