#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <C:\Users\HP\GitHub\MyLibraries\privateLibraries\inputs.h>

using namespace std;

string path = R"(C:\Users\HP\Desktop\)";

struct AccountData
{

    string accountNumber;
    string PIN;
    string name;
    string phone;
    double balance;
    bool isMarkedForDelete = false;


};

enum enMainScreenChoices { showClients = 1, add, Delete, updateClientInfo, findClient, eTransactions, ExitFromMain };

enum enUpdateClientChoices { PIN = 1, name, phone, ExitFromUpdate };

enum enTransactions { edeposit = 1, ewithdraw, etotalBalances, ExitFromTransactions };

void saveVectorToFile(const vector<AccountData>& accounts, string fileName);

bool findClientByAccountNumber(string accountNumber, vector<AccountData>& accounts, AccountData*& account)
{

    for (AccountData& ad : accounts)
    {

        if (accountNumber == ad.accountNumber)
        {

            account = &ad;
            return true;
        }

    }
    return false;
}

bool findClientByAccountNumber(string accountNumber, const vector<AccountData>& accounts)
{

    for (const AccountData& ad : accounts)
    {

        if (accountNumber == ad.accountNumber)
        {

            return true;
        }

    }
    return false;
}


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

enUpdateClientChoices getUpdateClientChoices()
{


    return (enUpdateClientChoices)inputs::readNumberFromTo("Choose a Choice from the List [1-4]", 1, 4);


}

AccountData convertLineToRecord(string line)
{
    if (!line.empty()) {
        vector <string>words = splitString(line);

        AccountData ad;

        ad.accountNumber = words.at(0);
        ad.PIN = words.at(1);
        ad.name = words.at(2);
        ad.phone = words.at(3);
        ad.balance = stod(words.at(4));
        return ad;
    }
    else
        return {};



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
        accountNumberFoundedBefore = findClientByAccountNumber(accountNumber, accounts);

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

void addClients(vector<AccountData>& accounts)
{

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

    saveVectorToFile(accounts, "Records");

}

vector<AccountData> getAccountsFromFile(string fileName)
{

    vector<AccountData> accounts;



    fstream file;
    file.open(path + fileName + ".txt", ios::in);

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

void printAccountNotFound(string accountNumber)
{

    cout << "=====================================================\n";
    cout << " Client With Account Number(" << accountNumber << ") Not Found!\n";
    cout << "=====================================================\n";
    Sleep(2000);


}

void saveVectorToFile(const vector<AccountData>& accounts, string fileName)
{

    fstream file;
    file.open(path + fileName + ".txt", ios::out);

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

void showClientsList(const vector<AccountData>& accounts)
{
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
            account.isMarkedForDelete = true;

    }




}

void deleteClient(vector<AccountData>& accounts, string accountNumber)
{
    AccountData* account = nullptr;
    if (findClientByAccountNumber(accountNumber, accounts, account))
    {
        char c;

        if (pinCheckPassed(*account)) {
            showClientCard(*account);

            cout << "Are You Sure You want to Delete This Client? Y/N\n";
            cin >> c; cin.ignore(1000, '\n');
            if (tolower(c) == 'y')
            {
                markClientForDelete(accounts, accountNumber);
                saveVectorToFile(accounts, "Records");
                accounts = getAccountsFromFile("Records");
                cout << "\n===========================\n";
                cout << " Client Deleted Sccssfully!\n";
                cout << "===========================\n";
                Sleep(2000);

            }
        }



    }
    else
    {
        printAccountNotFound(accountNumber);

    }


}

void deleteClientScreen(vector<AccountData>& accounts)
{

    system("cls");
    cout << "--------------------------------\n";
    cout << "     Delete Client Screen\n";
    cout << "--------------------------------\n";
    string accountNumber = inputs::readText_emptyNotAllowed("Enter An Account Number: ");

    deleteClient(accounts, accountNumber);


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

    return (enTransactions)inputs::readNumberFromTo("Choose A Choice From The List [1 - 4]: ", 1, 4);

}

void performDeposit(AccountData& account, double amount)
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
    AccountData* account;
    if (findClientByAccountNumber(accountNumber, accounts, account))
    {
        showClientCard(*account);


        double amount = inputs::readPositiveDoubleNumber("Amount to Deposit: ");

        char c = inputs::readChar("Are You Sure You want To Perform The Deposit On This Account? Y / N\n");

        if (tolower(c) == 'y') {
            performDeposit(*account, amount);

            cout << "=====================================================================\n";
            cout << " The Deposit Compeleted Sccssfully  , The Balance is: " << account->balance << endl;
            cout << "=====================================================================\n";

            saveVectorToFile(accounts, "Records");

            system("pause");
        }





    }
    else
        printAccountNotFound(accountNumber);


}

void performWithdraw(AccountData& account, double amount)
{

    account.balance -= amount;


}

void withdraw(vector<AccountData>& accounts)
{
    system("cls");
    cout << "--------------------------\n";
    cout << "      Withdraw Screen\n";
    cout << "--------------------------\n";
    string accountNumber = inputs::readText_emptyNotAllowed("Enter an Account Number: ");

    AccountData* account;

    if (findClientByAccountNumber(accountNumber, accounts, account))
    {

        showClientCard(*account);
        double amount = inputs::readPositiveDoubleNumber("Amount To Withdraw: ");

        while (amount > account->balance)
        {

            cout << "Amount Exceeds The Balance , you can withdraw up to: " << account->balance << endl;
            amount = inputs::readPositiveDoubleNumber("Enter Another Amount To withdraw: ");
        }

        char c = inputs::readChar("Are You Sure You want To Perform Withdraw on this account? Y / N\n");

        if (tolower(c) == 'y')
        {

            performWithdraw(*account, amount);
            cout << "=====================================================================\n";
            cout << " The Withdraw Compeleted Sccssfully  , The Balance is: " << account->balance << endl;
            cout << "=====================================================================\n";
            saveVectorToFile(accounts, "Records");
            system("pause");

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
    cout << " [3] Show All Balances.\n";
    cout << " [4] Return to The Main Menu.\n";



}


void showAllBalances(vector<AccountData>& accounts)
{
    system("cls");
    double sum = 0;
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

    cout << "\t\t\t\tTotal Balances: " << sum << endl;

    system("pause");
}


void transactions(vector<AccountData>& accounts)
{


    enTransactions userChoice;

    do
    {
        system("cls");
        showTransactionsMenu();

        userChoice = getTransactionsUserChoice();

        switch (userChoice)
        {

        case edeposit:deposit(accounts); break;
        case ewithdraw:withdraw(accounts); break;
        case etotalBalances:showAllBalances(accounts); break;
        case ExitFromTransactions:return;


        }


    } while (userChoice != 4);



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

void updateClient(vector<AccountData>& accounts, string accountNumber)
{
    AccountData* ad = nullptr;
    if (findClientByAccountNumber(accountNumber, accounts, ad))
    {
        if (pinCheckPassed(*ad)) {
            enUpdateClientChoices userChoice;

            do {
                system("cls");

                printUpdateScreen();

                showClientCard(*ad);

                showUpdateChoices();
                userChoice = getUpdateClientChoices();

                switch (userChoice)
                {
                case PIN: updatePIN(*ad); break;
                case phone:updatePhone(*ad); break;
                case name:updateName(*ad); break;
                case ExitFromUpdate:return;

                }



            } while (userChoice != 4);

        }

    }
    else
    {

        printAccountNotFound(accountNumber);


    }


}

void updateClientScreen(vector<AccountData>& accounts)
{

    system("cls");

    printUpdateScreen();

    string accountNumber = inputs::readText_emptyNotAllowed("\nEnter Account Number: ");


    updateClient(accounts, accountNumber);

    saveVectorToFile(accounts, "Records");

    accounts = getAccountsFromFile("Records");



}


void findClientScreen(vector<AccountData>& accounts)
{
    system("cls");
    cout << "----------------------------\n";
    cout << "    Find Client Screen\n";
    cout << "----------------------------\n";

    string accountNumber = inputs::readText_emptyNotAllowed("Enter an Account Number: ");
    AccountData* ad = nullptr;

    if (findClientByAccountNumber(accountNumber, accounts, ad))
    {

        showClientCard(*ad);
        system("pause");

    }
    else
    {
        printAccountNotFound(accountNumber);
    }


}


enMainScreenChoices getMainMenuChoice()
{

    return (enMainScreenChoices)inputs::readNumberFromTo("Choose Number From The List [1 - 7]: ", 1, 7);

}



void showMainMenu();


void mainScreen(string fileName)
{


    vector<AccountData> accounts = getAccountsFromFile(fileName);
    enMainScreenChoices userChoice;
    do {
        system("cls");
        showMainMenu();
        userChoice = getMainMenuChoice();
        switch (userChoice)
        {

        case add:addClients(accounts); break;
        case Delete:deleteClientScreen(accounts); break;
        case updateClientInfo:updateClientScreen(accounts); break;
        case eTransactions:transactions(accounts); break;
        case findClient:findClientScreen(accounts); break;
        case showClients:showClientsList(accounts); break;
        case ExitFromMain:return;


        }

    } while (userChoice != 7);



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
    cout << "     [7] Exit.\n";



}

void mainProgram()
{
    string fileName = "Records";
    mainScreen(fileName);

}

int main()
{
    mainProgram();

    return 0;
}

