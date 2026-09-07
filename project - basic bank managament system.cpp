#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <C:\Users\HP\GitHub\MyLibraries\privateLibraries\inputs.h>

using namespace std;

const string path = R"(C:\Users\HP\Desktop\)";
const string usersFileName = "Users.txt";
const string clientsFileName = "Records.txt";




struct AccountData
{

    string accountNumber;
    string PIN;
    string name;
    string phone;
    double balance;
    bool isMarkedForDelete = false;


};

struct User
{

    string username;
    string password;
    bool mainMenuPermissions[7] = {0,0,0,0,0,0,0};
    bool isMarkedForDelete;
    
};

User currentUser;


enum enMainScreenChoices { eShowClients = 1, eAddClient, eDeleteClient, eUpdateClientInfo, eFindClient, eTransactions, eManageUsers ,  eLogout };

enum enUpdateClientChoices { PIN = 1, name, phone, ExitFromUpdate };

enum enTransactions { eDeposit = 1, eWithdraw , eTransfer, eTotalBalances, ExitFromTransactions };

enum enManageUsers {eShowUsers = 1, eAddUser , eDeleteUser , eUpdateUser ,eFindUser, ExitFromManageUser};

enum enUpdateUser {password = 1, givePermissions , takePermissions , exitFromUserUpdate};

void saveVectorToFile(const vector<AccountData>& accounts, string fileName);

vector<string> splitString(string line, string delim = " // ")
{
    vector<string> words;

    size_t pos = 0;

    while ((pos = line.find(delim)) != string::npos)
    {

        if (pos > 0)
            words.push_back(line.substr(0, pos));

        line.erase(0, pos + delim.length());
    }

    if (!line.empty())
    {

        words.push_back(line);

    }

    return words;

}


AccountData convertLineToRecord(string line)
{

    vector <string>words = splitString(line);

    AccountData ad;

    ad.accountNumber = words.at(0);
    ad.PIN = words.at(1);
    ad.name = words.at(2);
    ad.phone = words.at(3);
    ad.balance = stod(words.at(4));
    return ad;



}

vector<AccountData> getAccountsFromFile(string fileName)
{

    vector<AccountData> accounts;



    fstream file;
    file.open(path + fileName , ios::in);

    if (file.is_open())
    {

        string line;
        while (getline(file, line))
        {

            accounts.push_back(convertLineToRecord(line));

        }
        file.close();

    }
    else
    {
        cout << "======================\n";
        cout << " File Doesn't Exists !\n";
        cout << "======================\n";
        Sleep(2000);
    }



    return accounts;

}


bool clientExistsByAccountNumber(string accountNumber)
{

   const vector<AccountData> accounts = getAccountsFromFile(clientsFileName);

    for(const AccountData & account:accounts)
    {
    
        if (account.accountNumber == accountNumber)
            return true;

    }

    return false;
}

bool findClientByAccountNumber(string accountNumber, AccountData& account)
{
   const vector<AccountData> accounts = getAccountsFromFile(clientsFileName);
    for ( const AccountData& ad : accounts)
    {

        if (accountNumber == ad.accountNumber)
        {

            account = ad;
            return true;
        }

    }
    return false;
}





enUpdateClientChoices getUpdateClientChoices()
{


    return (enUpdateClientChoices)inputs::readNumberFromTo("Choose a Choice from the List [1-4]", 1, 4);


}






string convertRecordToLine(const AccountData& ad, string delim = " // ")
{

    string line = "";

    line += ad.accountNumber + delim;
    line += ad.PIN + delim;
    line += ad.name + delim;
    line += ad.phone + delim;
    line += to_string(ad.balance);


    return line;
}



void fillAccountData(const vector<AccountData>& accounts, AccountData& ad)
{
    string accountNumber;
    bool accountNumberFoundedBefore = false;
    do {
        accountNumber = inputs::readText_emptyNotAllowed("Enter Your Account Number: ");
        accountNumberFoundedBefore = clientExistsByAccountNumber(accountNumber);

        if (accountNumberFoundedBefore)
        {

            cout << "This Account Number Is Already Exists , please Enter Another Number.\n";

        }

    } while (accountNumberFoundedBefore);

    ad.accountNumber = accountNumber;


    cout << "Enter Your PIN code: ";
    getline(cin, ad.PIN);

    cout << "Enter Your Name: ";
    getline(cin, ad.name);

    cout << "Enter Your Phone Number: ";
    getline(cin, ad.phone);


    ad.balance = inputs::readPositiveDoubleNumber("Enter Account Balance: ");



}

void addClient(vector<AccountData>& accounts)
{
    AccountData ad;
    fillAccountData(accounts, ad);



    accounts.push_back(ad);
    cout << "===========================\n";
    cout << "  Client Added Sccssfully!\n";
    cout << "===========================\n";
    Sleep(2000);

}

void addClients()
{
    vector<AccountData> accounts = getAccountsFromFile(clientsFileName);
    char c;

    do {
        system("cls");
        cout << "-------------------------------\n";
        cout << "      Add Client Screen\n";
        cout << "-------------------------------\n";
        addClient(accounts);
        cout << "\nDo You Want To add more clints? press y if yes.\n";
        cin >> c;
        cin.ignore(100, '\n');

    } while (tolower(c) == 'y');

    saveVectorToFile(accounts, clientsFileName);


}


void printAccountNotFound(string accountNumber)
{

   std::cout << "=====================================================\n";
    cout << " Client With Account Number(" << accountNumber << ") Not Found!\n";
    cout << "=====================================================\n";
    Sleep(2000);


}

void saveVectorToFile(const vector<AccountData>& accounts, string fileName)
{

    fstream file;
    file.open(path + fileName , ios::out);

    string line;

    for (const AccountData& account : accounts)
    {
        if (!account.isMarkedForDelete)
        {
            line = convertRecordToLine(account);
            file << line << endl;


        }


    }
    file.close();

}

void showClientsList()
{
    vector<AccountData> accounts = getAccountsFromFile(clientsFileName);
    system("cls");
    if (accounts.size() > 0) {

        cout << "\n\t\t\t\t\tClient List (" << accounts.size() << ") Client(s).\n";
        cout << "\n_______________________________________________________________________________________________________________________\n\n";

        cout << "| Account Number   |  Pin Code  | Client Name                                            | Phone Number     | Balance   \n";
        cout << "________________________________________________________________________________________________________________________\n\n";
        for (short pos = 0; pos < accounts.size(); pos++) {
            cout << "| " << setw(17) << left << accounts[pos].accountNumber << "| " << setw(11) << left << accounts[pos].PIN;
            cout << "| " << setw(55) << left << accounts[pos].name << "| " << setw(17) << left << accounts[pos].phone << "| " << setw(10) << left << accounts[pos].balance << "\n";
        }

        cout << "________________________________________________________________________________________________________________________\n";

        system("pause");
    }
    else
    {
        cout << "========================\n";
        cout << "   The List is empty !\n";
        cout << "========================\n";
        Sleep(2000);

    }
}


void showClientCard(const AccountData& account)
{

    cout << "=====================================\n";
    cout << "    Account Number: " << account.accountNumber << endl;
    cout << "    PIN Code      : " << account.PIN << endl;
    cout << "    Name          : " << account.name << endl;
    cout << "    Phone Number  : " << account.phone << endl;
    cout << "    Balance       : " << account.balance << endl;
    cout << "=====================================\n";


}

bool pinCheckPassed(const AccountData& account)
{



    int counter = 3;
    string pin;

    do {

        pin = inputs::readText_emptyNotAllowed("Enter Your PIN code: ");

        if (account.PIN == pin)
            return true;
        else
        {
            counter--;
            cout << "Incorrect PIN code , please Enter The correct PIN code to perform change.\n";
            cout << "Tries Left: " << counter << endl;
        }


    } while (pin != account.PIN && counter > 0);

    cout << "your tries left is (" << counter << ")\nPlease Try Again Later.\n";
    Sleep(2000);
    return false;

}

void markClientForDelete(vector<AccountData>& accounts, const string& accountNumber)
{
    for (AccountData& account : accounts)
    {

        if (account.accountNumber == accountNumber)
        {
            account.isMarkedForDelete = true;
            break;
        }

    }




}

void deleteClient(vector<AccountData>& accounts, string accountNumber)
{
    AccountData account;
    if (findClientByAccountNumber(accountNumber,account))
    {
        char c;

        if (pinCheckPassed(account)) {
            showClientCard(account);

            cout << "Are You Sure You want to Delete This Client? Y/N\n";
            cin >> c; cin.ignore(1000, '\n');
            if (tolower(c) == 'y')
            {
                markClientForDelete(accounts, accountNumber);
                saveVectorToFile(accounts, clientsFileName);
                accounts = getAccountsFromFile(clientsFileName);
                cout << "\n===========================\n";
                cout << " Client Deleted Sccssfully!\n";
                cout << "===========================\n";
               

            }
        }



    }
    else
    {
        printAccountNotFound(accountNumber);

    }


}

void deleteClientScreen()
{
    vector<AccountData> accounts = getAccountsFromFile(clientsFileName);
    char c;
    do {
        system("cls");
        cout << "--------------------------------\n";
        cout << "     Delete Client Screen\n";
        cout << "--------------------------------\n";
        string accountNumber = inputs::readText_emptyNotAllowed("Enter An Account Number: ");

        deleteClient(accounts, accountNumber);

        cout << "\n\nDo you want To Delete Another Client? Y / N\n";
        cin >> c; cin.ignore(1000, '\n');

    } while (tolower(c) == 'y');

}

void showUpdateChoices()
{
    cout << "What Do you want to update?\n";
    cout << "1 - PIN code.\n";
    cout << "2 - Name.\n";
    cout << "3 - phone Number.\n";
    cout << "4 - Return.\n";

}


enTransactions getTransactionsUserChoice()
{

    return (enTransactions)inputs::readNumberFromTo("Choose A Choice From The List [1 - 5]: ", 1, 5);

}

void performDeposit(AccountData& account , double amount)
{




    account.balance += amount;





}

void deposit(vector<AccountData>& accounts)
{
    system("cls");
    cout << "------------------------\n";
    cout << "    Deposit Screen\n";
    cout << "------------------------\n";

    string accountNumber = inputs::readText_emptyNotAllowed("Enter account Number: ");
    

    if (clientExistsByAccountNumber(accountNumber))
    {

        for (AccountData& account : accounts) {
            
            if (account.accountNumber == accountNumber) {
                showClientCard(account);

                double amount = inputs::readPositiveDoubleNumber("Amount To Deposit: ");

                char c = inputs::readChar("Are You Sure You want To Perform The Deposit On This Account? Y / N\n");

                if (tolower(c) == 'y') {
                    performDeposit(account, amount);

                    cout << "=====================================================================\n";
                    cout << " The Deposit Compeleted Sccssfully  , The Balance is: " << account.balance << endl;
                    cout << "=====================================================================\n";

                    saveVectorToFile(accounts, clientsFileName);
                    accounts = getAccountsFromFile(clientsFileName);
                    cout << "Press any key to continue...\n";
                    system("pause >0");

                    break;
                }
            }
        }





    }
    else
        printAccountNotFound(accountNumber);


}

bool performWithdraw(AccountData& account , double amount)
{
    char c;
    if (amount > account.balance) {
       
        
        
            cout << "Amount Exceeds The Balance , you can withdraw up to: " << account.balance << endl;
            Sleep(2000);
            return false;
            

    }
    else
    {
        account.balance -= amount;
        return true;
    }

    account.balance -= amount;


}

void withdraw(vector<AccountData>& accounts)
{
    system("cls");
    cout << "--------------------------\n";
    cout << "      Withdraw Screen\n";
    cout << "--------------------------\n";
    string accountNumber = inputs::readText_emptyNotAllowed("Enter an Account Number: ");

    

    if (clientExistsByAccountNumber(accountNumber))
    {

        for (AccountData& account : accounts) {
          
            if (account.accountNumber == accountNumber) {
                showClientCard(account);

                double amount = inputs::readPositiveDoubleNumber("Amount To Withdraw: ");

                char c = inputs::readChar("Are You Sure You want To Perform Withdraw on this account? Y / N\n");

                if (tolower(c) == 'y')
                {
                    if (performWithdraw(account, amount)) {
                        cout << "=====================================================================\n";
                        cout << " The Withdraw Compeleted Sccssfully  , The Balance is: " << account.balance << endl;
                        cout << "=====================================================================\n";
                        saveVectorToFile(accounts, clientsFileName);
                        cout << "Press any key to continue...\n";
                        system("pause >0");
                    }

                }break;
            }
        }


    }
    else
        printAccountNotFound(accountNumber);


}

void showTransactionsMenu()
{

    cout << "--------------------------\n";
    cout << "    Transactions Screen\n";
    cout << "--------------------------\n";
    cout << " [1] Deposit.\n";
    cout << " [2] Withdraw.\n";
    cout << " [3] Transfer.\n";
    cout << " [4] Show All Balances.\n";
    cout << " [5] Return to The Main Menu.\n";



}

string readAccountNumber(string message,const vector<AccountData>&accounts)
{

    string accountNumber;
    bool founded;
    do {
        accountNumber = inputs::readText_emptyNotAllowed(message);
        founded = clientExistsByAccountNumber(accountNumber);

        if (!founded)
            printAccountNotFound(accountNumber);

    } while (!founded);

    

    return accountNumber;


}

void showTransferScreenHead()
{

    cout << "----------------------------\n";
    cout << "       Transfer Screen\n";
    cout << "----------------------------\n";


}

void transfer(vector<AccountData> &accounts)
{
    system("cls");

    showTransferScreenHead();
    string reciverNumber;
    string senderNumber;
    do {
        reciverNumber = readAccountNumber("Enter The Reciver Account Number: ", accounts);
        senderNumber = readAccountNumber("Enter The Sender Account Number:", accounts);

        if (reciverNumber == senderNumber)
            cout << "Transferring between the same Account Isn't Allowed.\n";

    } while (reciverNumber == senderNumber);

        system("cls");
        showTransferScreenHead();
        
        AccountData* sender = nullptr;
        AccountData* reciver = nullptr;

        for (AccountData& account : accounts) {
            
            if (!reciver && account.accountNumber == reciverNumber)
            {
            
                 reciver = &account;
                

            }

            if(!sender && account.accountNumber == senderNumber)
            {
            
                 sender = &account;
                
            
            }

            if (sender && reciver) {

                cout << "The Reciver Client:\n";
                showClientCard(*reciver);
                cout << "\nThe Sender Client:\n";
                showClientCard(*sender);

                double amount;
                do {
                    amount = inputs::readPositiveDoubleNumber("\nAmount To Transfer: ");
                    if (amount > sender->balance)
                    {
                        cout << "Insufficient Balance.\n";
                        Sleep(2000);
                        
                    }
                } while (amount > sender->balance);
                char c = inputs::readChar("\nAre You Sure You Want To perform This operation? Y / N\n");

                if (tolower(c) == 'y') {

                    sender->balance -= amount;
                    reciver->balance += amount;
                    cout << "=========================================================\n";
                    cout << " Transferred Sccssfully !\n(" << sender->accountNumber << ") Balance is : " << sender->balance << endl;
                    cout << "\n(" << reciver->accountNumber << ") Balance is : " << reciver->balance << endl;
                    cout << "=========================================================\n";
                    saveVectorToFile(accounts, clientsFileName);
                    accounts = getAccountsFromFile(clientsFileName);
                    system("pause");
                }
                break;
            }

        }
    }

void showAllBalances(vector<AccountData>& accounts)
{
    system("cls");
   long double sum = 0;
    cout << "\n\t\t\t\t\Balances List (" << accounts.size() << ") Client(s).\n";
    cout << "\n_______________________________________________________________________________________\n\n";

    cout << "| Account Number   | Client Name                                            | Balance   \n";
    cout << "________________________________________________________________________________________\n\n";
    for (short pos = 0; pos < accounts.size(); pos++)
    {

        cout << "| " << setw(17) << left << accounts[pos].accountNumber << "| " << setw(55) << left << accounts[pos].name << "| " << setw(10) << left << accounts[pos].balance << endl;
        sum += accounts[pos].balance;

    }

    cout << "________________________________________________________________________________________\n\n";

    cout << "\t\t\t\tTotal Balances: " << sum << endl<<endl;

    system("pause");
}


void transactions()
{

    vector<AccountData> accounts = getAccountsFromFile(clientsFileName);
    enTransactions userChoice;

    do
    {
        system("cls");
        showTransactionsMenu();

        userChoice = getTransactionsUserChoice();

        switch (userChoice)
        {

        case eDeposit:deposit(accounts); break;
        case eWithdraw:withdraw(accounts); break;
        case eTotalBalances:showAllBalances(accounts); break;
        case eTransfer:transfer(accounts); break;
        case ExitFromTransactions:return;


        }


    } while (userChoice != 5);



}


void updatePIN(AccountData& account)
{





    string newPIN;
    newPIN = inputs::readText_emptyNotAllowed("Enter Your New PIN Code: ");
    account.PIN = newPIN;
    cout << "================================\n";
    cout << "Your New PIN Code is: " << account.PIN << endl;
    cout << "================================\n";

    Sleep(2500);








}

void updateName(AccountData& account)
{



    string newName;
    newName = inputs::readText_emptyNotAllowed("Enter Your New Name: ");
    account.name = newName;
    cout << "================================\n";
    cout << "Your New Name is: " << account.name << endl;
    cout << "================================\n";
    Sleep(2500);






}

void updatePhone(AccountData& account)
{



    account.phone = inputs::readText_emptyNotAllowed("Enter Your New Phone Number: ");

    cout << "========================================================\n";
    cout << " Your Phone Number has been changed to: " << account.phone << endl;
    cout << "========================================================\n";

    Sleep(2500);

}



void printUpdateScreen()
{
    cout << "----------------------------\n";
    cout << "   Update Client Screen\n";
    cout << "----------------------------\n";


}

void updateClient(AccountData &ad)
{
    
        if (pinCheckPassed(ad)) {
            enUpdateClientChoices userChoice;

            do {
                system("cls");

                printUpdateScreen();

                showClientCard(ad);

                showUpdateChoices();
                userChoice = getUpdateClientChoices();

                switch (userChoice)
                {
                case PIN: updatePIN(ad); break;
                case phone:updatePhone(ad); break;
                case name:updateName(ad); break;
                case ExitFromUpdate:return;

                }



            } while (userChoice != 4);

        }

    }
    




void updateClientScreen()
{
    vector<AccountData> accounts ;
    char c;
    do {

        accounts = getAccountsFromFile(clientsFileName);
        system("cls");

        printUpdateScreen();

        string accountNumber = inputs::readText_emptyNotAllowed("\nEnter Account Number: ");
        
        while(!clientExistsByAccountNumber(accountNumber))
        {
            system("cls");
            printUpdateScreen();
            printAccountNotFound(accountNumber);
            accountNumber = inputs::readText_emptyNotAllowed("\nEnter Account Number: ");

        }

        
        for (AccountData& account : accounts) {
            if (account.accountNumber == accountNumber)
            {      
                
                updateClient(account);
                break;
            }
        }

        saveVectorToFile(accounts, clientsFileName);
        
        cout << "Do You Want To Update More Clients? Y / N\n";
        cin >> c; cin.ignore(1000, '\n');

    } while (tolower(c) == 'y');

}


void findClientScreen()
{
    system("cls");
    cout << "----------------------------\n";
    cout << "    Find Client Screen\n";
    cout << "----------------------------\n";

    string accountNumber = inputs::readText_emptyNotAllowed("Enter an Account Number: ");
    AccountData ad;

    if (findClientByAccountNumber(accountNumber, ad))
    {

        showClientCard(ad);
        system("pause");

    }
    else
    {
        printAccountNotFound(accountNumber);
    }


}


enMainScreenChoices getMainMenuChoice()
{
    enMainScreenChoices option;
    do {
        option = (enMainScreenChoices)inputs::readNumberFromTo("Choose Number From The List: ", 1, 8);

        if (option!=eLogout  && !currentUser.mainMenuPermissions[option - 1])
        {
            cout << "Access Denied , Please choose an option within your Permissions or contact Your Admin.\n";
        }

    } while (option != eLogout && !currentUser.mainMenuPermissions[option - 1]);
    
    return option;
}

User convertLineToUser(string line)
{
    User user;
    vector<string> userData = splitString(line);

    user.username = userData[0];
    user.password = userData[1];
    for (short i = 0; i < 7; i++) {

        user.mainMenuPermissions[i] = stoi(userData[2].substr(0, 1));
        userData[2].erase(0, 1);
    }
    return user;


}

vector<User> getUsersFromFile(string usersFileName)
{

    fstream file;
    vector<User> users;
    file.open(path + usersFileName, ios::in);

    if (file.is_open())
    {

        string line;
        while (getline(file, line))
        {

            users.push_back(convertLineToUser(line));

        }

        return users;
    }



}

bool isValidPassword(string password)
{


    if (password.length() < 6)
    {
        cout << "The Password Must be at least from 6 characters.\n";
        return false;

    }


    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;

    for (int i = 0; i < password.length(); i++)
    {
        if (password[i] > 32 && password[i] <= 47 || password[i] == 64)
        {
            hasSpecialChar = true;
        }
        else if (password[i] >= 48 && password[i] <= 57)
        {
            hasDigit = true;

        }
        else if (password[i] >= 97 && password[i] <= 122)
            hasLower = true;
        else if (password[i] >= 65 && password[i] <= 90)
            hasUpper = true;
        else
        {
            cout << "This Character(" << password[i] << ") it's not supported.\n";
            return false;
        }

        if (hasUpper && hasLower && hasDigit && hasSpecialChar)
            break;

    }




    if (!hasSpecialChar)
    {

        cout << "The Password Must contain at least one special character(!,@,#,$,%,&,*,^,...etc)\n";
        return hasSpecialChar;

    }

    if (!hasDigit)
    {
        cout << "The Password must contain at least one number.\n";
        return hasDigit;
    }

    if (!hasUpper || !hasLower)
    {

        cout << "The Password Must contain at least one capital letter and at least one smaller letter.\n";
        return hasUpper && hasLower;
    }

    return true;
}

string convertUserToLine(const User &user , string delim = " // ")
{

    string line;

    line = user.username + delim;
    line += user.password + delim;
    for(short i = 0;i<7;i++)
    line += to_string(user.mainMenuPermissions[i]);
    
    return line;


}



void saveUsersToFile(const vector<User>&users , string usersFileName)
{

    fstream file;
    file.open(path + usersFileName , ios::out);
    
    
    for (const User& user : users)
    {
         if(!user.isMarkedForDelete)
         file << convertUserToLine(user) << endl;
    }

    file.close();

}

bool findUserByUsername(string username)
{
     vector<User> users = getUsersFromFile(usersFileName);

    for(User &user :users)
    {
    
        if(user.username == username)
        {
            
            return true;

        }
    
    
    }
    
    
    return false;
    

}

bool isUsernameTakenBefore(const vector<User>&users,string username)
{

    for (const User& user : users)
        if (user.username == username)
            return true;
    return false;


}

void print_AddUserScreen_()
{

    cout << "-------------------------\n";
    cout << "    Add User Screen\n";
    cout << "-------------------------\n";


}

string readPassword()
{

    string password;
    bool validPassword;

    do
    {
        validPassword = true;
        getline(cin, password);
        if (!isValidPassword(password))
        {

            validPassword = false;
            cout << "ReEnter The Password: ";
        }


    } while (!validPassword);

    return password;

}

User readUserInfo(const vector<User>&users)
{
    User user;
    bool isUsernameAvailable;
    do {
        isUsernameAvailable = true;
        cout << "\nEnter The Username: ";
        getline(cin, user.username);
        
        if (isUsernameTakenBefore(users, user.username))
        {
            isUsernameAvailable = false;
            system("cls");
            print_AddUserScreen_();
            cout << "\nThe User Name [" << user.username << "] is taken Before ,Please Enter Another one.\n";
        }

    } while (!isUsernameAvailable);
    cout << "Enter The Password: ";
    user.password = readPassword();
    return user;

}

void print_GivePermissionScreen_()
{

    cout << "----------------------------\n";
    cout << "   Give Permission Screen\n";
    cout << "----------------------------\n";

}

void giveAllPermissions(User &user)
{

    for (short i = 0; i < 7; i++)
        if(!user.mainMenuPermissions[i])
        user.mainMenuPermissions[i] = true;


}

void showPermissions()
{

    cout << "\n  [1] Show Clients List.\n";
    cout << "  [2] Add Client.\n";
    cout << "  [3] Delete Client.\n";
    cout << "  [4] Update Client.\n";
    cout << "  [5] Find Client.\n";
    cout << "  [6] Transactions.\n";
    cout << "  [7] Manage Users.\n";
    cout << "  [8] All Permissions.\n";




}

void givePermission(enMainScreenChoices permission , User &user)
{
   

     if (!user.mainMenuPermissions[permission - 1])
        user.mainMenuPermissions[permission - 1] = true;
    else
        cout << "This Permission Already Given to " << user.username << endl;

}

void givePermissionsToUserScreen(User &user, const User&currentUser)
{
    system("cls");
    print_GivePermissionScreen_();
    if (user.username == currentUser.username)
    {
        cout << "You Can't Give Permissions To This User Right Now.\n";
        Sleep(2000);
        return;
    }
    
    char c;
    do {
        system("cls");
        print_GivePermissionScreen_();
        showPermissions();
        enMainScreenChoices permission = (enMainScreenChoices)inputs::readNumberFromTo("Your Input: ", 1, 8);
        if (permission == 8)
        {
            giveAllPermissions(user); return;
        }
        givePermission(permission, user);
        cout << "Do you want to give more permissions? Y / N.\n";
        cin >> c; cin.ignore(1000, '\n');

    } while (tolower(c) == 'y');



}

void givePermissionsToUserScreen(User& user)
{
   
    char c;
    do {
        system("cls");
        print_GivePermissionScreen_();
        showPermissions();
        enMainScreenChoices permission = (enMainScreenChoices)inputs::readNumberFromTo("Your Input: ", 1, 8);
        if (permission == 8)
        {
            giveAllPermissions(user); return;
        }
        givePermission(permission, user);
        cout << "Do you want to give more permissions? Y / N.\n";
        cin >> c; cin.ignore(1000, '\n');

    } while (tolower(c) == 'y');



}

void addUser(vector<User>&users)
{
    User user = readUserInfo(users);
    user.isMarkedForDelete = false;
    givePermissionsToUserScreen(user);
    users.push_back(user);

}

void addUsers(vector<User>&users , string usersFileName)
{
    char c;

    do {
        system("cls");
        print_AddUserScreen_();

        addUser(users);
        cout << "\nUser Added Sccssfully.\n";
        cout << "\nDo you want to add more users? Y / N.\n";
        cin >> c; cin.ignore(1000, '\n');

    } while (tolower(c) == 'y');

    saveUsersToFile(users, usersFileName);

}

void addMainAdmin(vector<User>&users)
{
    User admin;
    admin.username = "Admin";
    cout << "There is No Admin For That System , Admin with Username\"Admin\" Added.\n";
    cout << "\nEnter The Password For The Admin User: ";
    admin.password = readPassword();
    admin.isMarkedForDelete = false;
    giveAllPermissions(admin);

    users.push_back(admin);

    cout << "Admin User Added Sccssfully!\n";
    Sleep(2000);

    saveUsersToFile(users, usersFileName);

}

enManageUsers getManageUsersOption()
{

    return (enManageUsers)inputs::readNumberFromTo("Your Input: ", 1, 5);

}

void print_manageUsersScreen_()
{

    cout << "-------------------------\n";
    cout << "   Manage Users Screen\n";
    cout << "-------------------------\n";


}

void showManageUsersScreen()
{

    cout << "\n  [1] Show Users List.\n";
    cout << "  [2] Add Users.\n";
    cout << "  [3] Delete User.\n";
    cout << "  [4] Update User.\n";
    cout << "  [5] Return To The Main Menu.\n";



}

void printUserData(const User &user)
{

    cout << setw(10) << left << user.username << "|" << setw(14) << left << user.password << "|";
    for(short per = 0 ; per<7;per++)
    {
        

        if (user.mainMenuPermissions[per])
            cout << "      yes    |";
        else
            cout << "      no     |";
    
    
    }
    cout << "\n";

}

void showUsersList(const vector<User> &users)
{

    system("cls");
    cout << "__________|______________|_________________________________________________________________________________________________|\n";
    cout << "          |              |                                   P e r m i s s i o n s                                         |\n";
    cout << " Username |   Password   |_________________________________________________________________________________________________|\n";
    cout << "          |              |show Clients |Add clients  |Delete Client|Update Client|Find Client  |Transactions |Manage Users |\n";
    cout << "----------|--------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|\n";
    for(const User &user : users)
    {
    
        printUserData(user);
    
    }
    cout << "----------|--------------|-------------------------------------------------------------------------------------------------|\n";

    cout << "\nPress Any key To Exit....\n";
    system("pause >0");

}

bool isDeleteConfirmed(string username)
{
    char c;
    cout << "Ary You Sure To Delete Client: " << username << " ? Y / N" << endl;
    cin >> c; cin.ignore(1000, '\n');
    return tolower(c) == 'y';


}

void print_deleteUserScreen_()
{

    cout << "------------------------\n";
    cout << "   Delete User Screen\n";
    cout << "------------------------\n";


}



void deleteUser(vector<User>& users, string usersFileName, User& currentUser)
{
    system("cls");
    print_deleteUserScreen_();
    string username = inputs::readText_emptyNotAllowed("Enter The username for The User you want to delete: ");
    if (!findUserByUsername(username))
    {
        cout << "User with username [" << username << "] hasn't been found.\n";
        Sleep(2000);
        return;
    }
    if (username == currentUser.username)
    {
        cout << "\nCan't Delete The Current User.\n"; Sleep(2000);
        return;
    }

    if (username == "Admin")
    {
        cout << "\nCan't Delete The Main Admin User.\n";
        Sleep(2000);
        return;
    }

    for (User& user : users) {

        if (user.username == username) {

            if (!isDeleteConfirmed(username))
            {
                return;

            }

            user.isMarkedForDelete = true;


            saveUsersToFile(users, usersFileName);
            users = getUsersFromFile(usersFileName);
            cout << "\nClient Deleted Sccssfully!\n";
            cout << "\nPress Any key To Exit.\n";
            system("pause >0");
            break;
        }
    }
}
 
void updateUserPassword(User&user)
{
    system("cls");
    cout << "Enter The new Password: ";
    user.password = readPassword();
    cout << "\nUser " << user.username << " Password Changed To: " << user.password << endl;
    cout << "\nPress Any Key To exit.\n";
    system("pause >0");

}



void print_updateUserScreen_()
{
    cout << "---------------------------\n";
    cout << "    Update User Screen\n";
    cout << "---------------------------\n";

}

void showUpdateUserOptions()
{

    cout << "\n  [1] Update password.\n";
    cout << "  [2] Give Permission(s).\n";
    cout << "  [3] Take Permission(s).\n";
    cout << "  [4] return.\n";

}

enUpdateUser getUpdateUserOption()
{

    return (enUpdateUser)inputs::readNumberFromTo("Your Input: ", 1,4);


}


void takeAllPermissions(User &user)
{

    for (short i = 0; i < 7; i++)
        user.mainMenuPermissions[i] = false;

}

void takePermissionFromUser(User&user , enMainScreenChoices permission)
{
    if (user.mainMenuPermissions[permission - 1])
        user.mainMenuPermissions[permission - 1] = false;
    else
    {
        cout << "This Permission Already Taken off from This user.\n";
        Sleep(2000);
    }
}

void print_TakePermissionScreen()
{

    cout << "-----------------------------------\n";
    cout << "\tTake Permission Screen\n";
    cout << "-----------------------------------\n";

}

void takePermissionsFromUserScreen(User &user , User &currentUser)
{
    system("cls");
    print_TakePermissionScreen();
    
    if (user.username == currentUser.username)
    {
        cout << "You Can't Take Permissions From This Current User.\n"; Sleep(2000);
        return;

    }
    enMainScreenChoices option;
    char c;
    do {
        system("cls");
        print_TakePermissionScreen();
        showPermissions();
        option = (enMainScreenChoices)inputs::readNumberFromTo("Your Input: ", 1, 8);
        if (option == 8)
            takeAllPermissions(user);
        else
            takePermissionFromUser(user, option);

        cout << "Do You Want To Take More Permissions? Y/N\n";
        cin >> c; cin.ignore(1000, '\n');

    } while (tolower(c) == 'y');


}

void updateUser(vector<User> &users)
{ 
    enUpdateUser updateOption;
    system("cls");
    print_updateUserScreen_();
    string username = inputs::readText_emptyNotAllowed("Enter The username for User you want to update: ");
    if (!findUserByUsername(username))
    {
        cout << "User with username [" << username << "] hasn't been found.\n";
        Sleep(2000);
        return;
    }
       if (username == "Admin" && !(currentUser.username == "Admin"))
    {
        cout << "Access Denied.\n";

        Sleep(2000);
        return;

    }

    

        for (User& user : users) {

            if (user.username == username) {

                do {
                    system("cls");
                    print_updateUserScreen_();


                    showUpdateUserOptions();
                    updateOption = getUpdateUserOption();
                    if (updateOption == exitFromUserUpdate)
                        return;
                switch (updateOption)
                {

                case password:

                    updateUserPassword(user);
                    saveUsersToFile(users, usersFileName);
                    break;

                case givePermissions:
                    givePermissionsToUserScreen(user, currentUser);
                    saveUsersToFile(users, usersFileName);
                    break;
                case takePermissions:
                    takePermissionsFromUserScreen(user, currentUser);
                    saveUsersToFile(users, usersFileName);
                    break;
                case exitFromUserUpdate:break;

                  }
                } while (updateOption != exitFromUserUpdate);
                break;
            }
        }
   

}

void manageUsersScreen()
{
    vector<User> users = getUsersFromFile(usersFileName);
    enManageUsers manageOption;
    
    do {
        
        system("cls");
        print_manageUsersScreen_();
        showManageUsersScreen();
        manageOption = getManageUsersOption();
        switch(manageOption)
        {
        
        case eShowUsers:showUsersList(users); break;
        case eAddUser:addUsers(users,usersFileName); break;
        case eDeleteUser:deleteUser(users, usersFileName , currentUser); break;
        case eUpdateUser:updateUser(users); break;
        case ExitFromManageUser:return;
        
        }

    } while (manageOption != 5);
    



}


void showMainMenu();

void logInScreen();
void mainScreen()
{

   
    
    enMainScreenChoices userChoice;
    do {
        system("cls");
        showMainMenu();
        userChoice = getMainMenuChoice();
        switch (userChoice)
        {

        case eAddClient:addClients(); break;
        case eDeleteClient:deleteClientScreen(); break;
        case eUpdateClientInfo:updateClientScreen(); break;
        case eTransactions:transactions(); break;
        case eFindClient:findClientScreen(); break;
        case eShowClients:showClientsList(); break;
        case eManageUsers:manageUsersScreen(); break;
        case eLogout:return;


        }

    } while (true);



}

void showMainMenu()
{

    cout << "------------------------------\n";
    cout << "       Main Menu Screen\n";
    cout << "------------------------------\n";
    cout << "\n     [1] Show Clients List.\n";
    cout << "     [2] Add Client.\n";
    cout << "     [3] Delete Client.\n";
    cout << "     [4] Update Client Info.\n";
    cout << "     [5] Find Client.\n";
    cout << "     [6] Transactions.\n";
    cout << "     [7] Manage Users.\n";
    cout << "     [8] logOut.\n";

}

bool loadUserData(string username, string password,User &currentUser)
{

    vector<User>users = getUsersFromFile(usersFileName);

    for(const User &user:users)
    {
    
        if (username == user.username && password == user.password)
        {
            currentUser = user;
            return true;
        }

    
    }
    return false;

}

void logInScreen()
{
    
    
    string username, password;
    bool  userLoaded ; 
    
        do {
           
            userLoaded = false;
            system("cls");
            cout << "---------------------\n";
            cout << "    LogIn Screen\n";
            cout << "---------------------\n";

            cout << "Enter The Username: ";
            getline(cin, username);
            if (!findUserByUsername(username))
            {
                cout << "User with that Username hasn't been found.\n";
                cout << "Press any key to Retry.\n";
                std::system("pause >0");
                
                continue;
            }


            cout << "Enter The Password: ";
            getline(cin, password);
            userLoaded = loadUserData(username, password, currentUser);

            if (!userLoaded)
            {
                cout << "The Username , or Password isn't Correct.\n";
                Sleep(2000);
            }

        } while (!userLoaded);


        

}




void mainProgram()
{
    system("color 2F");
    vector<User> users = getUsersFromFile(usersFileName);
    if(users.empty())
    {
    
        addMainAdmin(users);
    
    }

    while (true) {
        logInScreen();
        mainScreen();
    }


}



int main()
{
  
    mainProgram();

    return 0;
}

