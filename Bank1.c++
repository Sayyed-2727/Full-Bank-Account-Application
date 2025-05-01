#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>
#include <windows.h> // Include for Sleep function

using namespace std;

const string ClientsNameFile = "ClientsNameFile.txt";

enum enMainMenuOptions {ShowClientList = 1, AddNewClient = 2, DeleteClient = 3, UpdateClientInfo = 4, FindClient = 5, Transactions = 6, Exit = 7};

enum enTransactionsMenuOptions {Deposit = 1, Withdraw = 2, TotalBalancesForAllClients = 3, MainMenu = 4};

struct stClientsData
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0;
    bool MarkToDelete = false;
};

void ShowMainMenuOptions(); // declare a posterior function here to be able to call it before it's line declaration
vector <stClientsData> LoadDataFromFile(string FileName); // declare a posterior function here to be able to call it before it's line declaration
void ShowTransactionsScreen();
void GoBackToMainMenu();


short ReadNumInput(string Message)
{
    short UserInput = 0;
    cout << Message;
    cin >> UserInput;

    return UserInput;
}

string ReadAccountNumber(string Message)
{
    string AccountNumber;
    cout << Message;
    getline(cin >> ws , AccountNumber);

    return AccountNumber ;
}

bool CheckIfClientAlreadyExists( stClientsData Client)
{
    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);
    for (stClientsData& C : vClients)
    {
        if (C.AccountNumber == Client.AccountNumber)
        {
            return true;
        }
    }
    return false;
}

stClientsData ReadNewClient()
{
    stClientsData Client;

    cout << "\nEnter Account Number:\n";
    getline(cin >> ws, Client.AccountNumber);

    while (CheckIfClientAlreadyExists(Client))
    {
        cout << "\nClient with account number [" << Client.AccountNumber << "] already exists. Please enter another Account Number:\n";
        getline(cin >> ws, Client.AccountNumber);
    }
    
    cout << "\nEnter Pin Code:\n";
    getline(cin, Client.PinCode);

    cout << "\nEnter Name:\n";
    getline(cin, Client.Name);

    cout << "\nEnter Phone Number:\n";
    getline(cin, Client.Phone);

    cout << "\nEnter Account Balance:\n";
    cin >> Client.AccountBalance;

    return Client;
}

string ConvertRecordToLine(stClientsData Client, string Delimiter = "#//#")
{
    string ClientRecord = "";
    ClientRecord += Client.AccountNumber + Delimiter;
    ClientRecord += Client.PinCode + Delimiter;
    ClientRecord += Client.Name + Delimiter;
    ClientRecord += Client.Phone + Delimiter;
    ClientRecord += to_string(Client.AccountBalance) ;
    
    return ClientRecord;
}

void AddUserToFile(string ClientLine)        
{
    fstream MyFile;
    MyFile.open(ClientsNameFile, ios::out | ios::app); // Write Mode

    if (MyFile.is_open())
    {
        MyFile << ClientLine << endl;
        MyFile.close();
    }
    
}

void AddNewUser()
{
    stClientsData NewClient;
    NewClient = ReadNewClient();
    AddUserToFile(ConvertRecordToLine(NewClient));
}

void AddingNewClientScreen()
{
    cout << "-----------------------------------------" <<endl;
    cout << "\tAdd New Clients Screen\t" <<endl;
    cout << "-----------------------------------------" <<endl;

    cout << "\nAdding new client...\n\n";
}

void AddUser()
{
    char AddMoreUser = 'Y';

    do
    {
        system("cls");
        AddingNewClientScreen();
        AddNewUser();

        cout << "\n\nClient added successfully. Do you want to add more users? (Y/N)\n\n\n";
        cin >> AddMoreUser;
    } while (toupper(AddMoreUser) == 'Y');
    
}

vector <string> SplitString(string s1, string Delimiter = "#//#")
{
    vector <string> vwords;
    string sword = "";
    short pos = 0;
    while ((pos= s1.find(Delimiter)) != std::string::npos)
    {
        sword = s1.substr(0,pos);
        if (sword != "")
        {
            vwords.push_back(sword);
        }
        
        s1.erase(0,pos + Delimiter.length());
    }
    if (s1 != "")
    {
        vwords.push_back(s1);
    }
    
    return vwords;
}

stClientsData ConvertLineDataToRecord(string LineData, string Delimiter = "#//#")
{
    stClientsData ClientData;
    vector <string> vword;
    vword = SplitString(LineData, Delimiter);

    ClientData.AccountNumber = vword[0];
    ClientData.PinCode = vword[1];
    ClientData.Name = vword[2];
    ClientData.Phone = vword[3];
    ClientData.AccountBalance = stod(vword[4]); // from string to double

    return ClientData;

}

vector <stClientsData> LoadDataFromFile(string FileName)
{
    vector <stClientsData> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read mode.
    
    if (MyFile.is_open())
    {
        string Line;
        stClientsData Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineDataToRecord(Line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
    
}

void PrintClientRecord(stClientsData Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ClearScreen()
{
    system("cls");
}

void ShowClientsHeader()
{


    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (stClientsData Client : vClients)
        {

            PrintClientRecord(Client);
            cout << endl;
        }

}

void ShowAllClientsScreen()
{
    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);


    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tTheres no clients to display!";
    }
    
    else
    {
        ShowClientsHeader();
        Sleep(5000); // Pause for 5 seconds
        ClearScreen(); // Clear the screen after 5 seconds
    }    
    cout  << "\n_______________________________________________________";
    cout << "_________________________________________\n\n";
    
    
}

void GoBackToMainMenu()
{
    cout << "Press any key to go back to the main menu....\n";
    system("pause>0");
    ShowMainMenuOptions();
}

bool CheckIfAccountNumberExists(string AccountNumber, stClientsData& Client)
{
    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);

    for (stClientsData& c : vClients )
    {
        if (c.AccountNumber == AccountNumber)
        {
            Client = c;
            return true;
        }
        
    }
    return false;
}

void PrintClientData(stClientsData Client)
{
    cout << "\nThe followin data are the client details:\n";
    cout << "----------------------------------------------\n" << endl;
    
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code       : " << Client.PinCode << endl;
    cout << "Account Name   : " << Client.Name << endl;
    cout << "Account Phone  : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "----------------------------------------------\n" << endl;
}

char ClientConfirmationMessage(string Message)
{
    char UserConfirmation;
    cout << Message;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cin >>UserConfirmation;
    return UserConfirmation;
}

void SaveClientDataToFile(vector <stClientsData> vClients)
{
    fstream MyFile;
    MyFile.open(ClientsNameFile, ios::out); // Overwrite

    if (MyFile.is_open())
    {    
        string Line;
    
        for (stClientsData& c : vClients)
        {
            if (c.MarkToDelete != true)
            {
                MyFile << ConvertRecordToLine(c) << endl;

            }
        }
        
        MyFile.close();
    }
}

bool MarkClientToBeDeleted(vector <stClientsData>& vClients, string AccountNumber)
{
    for (stClientsData& c : vClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            c.MarkToDelete = true;
            return true;
        }
    } 
    return false;
}

void DeleteClientByAccountNumber()
{
    cout << "-----------------------------------------" <<endl;
    cout << "\tDelete Client Screen\t" <<endl;
    cout << "-----------------------------------------" <<endl;

    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);
    string AccountNumber = ReadAccountNumber("\nPlease Enter Account Number:\n");
    short UserInput = 0;
    stClientsData Client;
    char DeleteConfirmation = 'Y';

    if (!CheckIfAccountNumberExists(AccountNumber, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] hasn't been found!\n\nPress 1 to try with different Account Number or 2 to go to the main menu......" << endl;
        cin >> UserInput;
        if (UserInput == 1)
        {
            // DeleteClientByAccountNumber();
        }
        else if (UserInput == 2)
        {
            ShowMainMenuOptions();
        }
    }

    PrintClientData(Client);
    char UserAnswer = ClientConfirmationMessage("\n\nAre you sure you want to delete this client from DataBase?\n");
    if(toupper(UserAnswer) != 'Y')
    {
        ShowMainMenuOptions();
    }


    MarkClientToBeDeleted(vClients, AccountNumber);
    SaveClientDataToFile(vClients);
    cout << "\n\nClient Deleted Successfully.\n\n";
}

void ChangeRecordInfo(stClientsData& Client)
{
    cout << "\nPlease enter Pin Code:\n";
    getline(cin >> ws , Client.PinCode);
    cout << "\nPlease enter Name:\n";
    getline(cin , Client.Name);
    cout << "\nPlease enter Phone Number:\n";
    getline(cin, Client.Phone);
    cout << "\nPlease enter Account Balance:\n";
    cin >> Client.AccountBalance;
}

void UpdateRecord(vector <stClientsData>& vClient, string AccountNumber)
{
    for(stClientsData& c : vClient)
    {
        if (c.AccountNumber == AccountNumber)
        {
            ChangeRecordInfo(c);
        }
        
    }
}

void UpdateClientData()
{
    cout << "-----------------------------------------" <<endl;
    cout << "\tUpdate Client Info Screen\t" <<endl;
    cout << "-----------------------------------------" <<endl;

    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);
    string AccountNumber = ReadAccountNumber("\nPlease enter Account Number:\n");
    stClientsData Client;    
    short UserInput = 0;

    if (!CheckIfAccountNumberExists(AccountNumber,Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] hasn't been found!\n\nPress 1 to try with different Account Number or 2 to go to the main menu......" << endl;
        cin >> UserInput;
        if (UserInput == 1)
        {
            // UpdateClientData();
        }
        else if (UserInput == 2)
        {
            ShowMainMenuOptions();
        }
    }

    PrintClientData(Client);
    char UserAnswer = ClientConfirmationMessage("\n\nAre you sure you want to update this client?\n");
    if(toupper(UserAnswer) != 'Y')
    {
        ShowMainMenuOptions();
    }

    UpdateRecord(vClients, AccountNumber);
    SaveClientDataToFile(vClients);
    cout << "\n\nClient has been updated successfully!\n" << endl;

}

void FindClientByAccountNumber()
{
    cout << "-----------------------------------------" << endl;
    cout << "\tFind Client Screen\t" <<endl;
    cout << "-----------------------------------------" <<endl;

    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);
    string AccountNumber = ReadAccountNumber("\nPlease enter Account Number:\n");
    stClientsData Client;
    short UserInput = 0;

    if (! CheckIfAccountNumberExists(AccountNumber, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] hasn't been found!\n\nPress 1 to try with different Account Number or 2 to go to the main menu......" << endl;
        cin >> UserInput;
        if (UserInput == 1)
        {
            // FindClientByAccountNumber();
        }
        else if (UserInput == 2)
        {
            ShowMainMenuOptions();
        }
    }

    PrintClientData(Client);
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions Menu...\n";
    system("pause>0");
    ShowTransactionsScreen();
}

void IncreaseBalance(vector <stClientsData>& vClient, string AccountNumber, int AmountToIncrease)
{
    for( stClientsData& c : vClient)
    {
        if (c.AccountNumber == AccountNumber)
        {
            c.AccountBalance += AmountToIncrease;
        }
    }
}

void ErrorMessageInTransactionsPage()
{
    cout << "\nInvalid entry. Please try again...\n\n";
    system("pause");
    GoBackToTransactionsMenu();
}

void DepositToAClientScreen()
{
    cout << "-----------------------------------------" << endl;
    cout << "\tDeposit Screen\t" <<endl;
    cout << "-----------------------------------------" <<endl;

    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);
    string AccountNumber = ReadAccountNumber("\nPlease enter Account Number:\n");
    stClientsData Client;
    short UserInput = 0;
    if (!CheckIfAccountNumberExists(AccountNumber, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] hasn't been found!\n\nPress 1 to try with different Account Number or 2 to go to the transactions menu......" << endl;
        cin >> UserInput;
        if (UserInput == 1)
        {
            // DepositToAClientScreen();
        }
        else if (UserInput == 2)
        {
            GoBackToTransactionsMenu();
        } 
        else
        {
            ErrorMessageInTransactionsPage();
        }
        
    }
    else
    {
        PrintClientData(Client);
        int DepositAmount = ReadNumInput("\nPlease Enter Deposit Amount:\n");
        if (DepositAmount)
        {
            char UserConfirmation = 'n';
            cout << "\nAre you sure you want to confirm the transaction? (Y/N)\n";
            cin >> UserConfirmation;

            if ((toupper(UserConfirmation)) != 'Y')
            {
                cout << "\nNever Mind. The transaction has not been confirmed.\n\nReturning to the main menu.....\n\n";
                GoBackToTransactionsMenu();
            }

            else
            {
                IncreaseBalance(vClients, AccountNumber, DepositAmount);
                SaveClientDataToFile(vClients);
                cout << "\nTransaction completed successfully!" << endl;
                cout << Client.Name << "'s new balance is: " << Client.AccountBalance + DepositAmount << endl << endl;
                GoBackToTransactionsMenu();
            }
        }

        else
        {
            cout << "You have to enter a valid amount! Please try again.\n\n";
            DepositToAClientScreen();
        }
    }
}

void DecreaseAmountToAClient(vector <stClientsData>& Vclients, string AccountNumber, double AmountToDecrease)
{
    for(stClientsData& c : Vclients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            c.AccountBalance -= AmountToDecrease;
        }
    }
}

void TotalBalancesScreen()
{
    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);

    cout << "\t\t\t\tList Of Total Client's Balances [" << vClients.size() << "] Client(s)" << endl;
    cout << "__________________________________________________________________________________________________________________\n" << endl;
    cout << "| " << setw(20) << left << "Account Number" << "| " << setw(40) << left << "Client Name" << "| " << setw(15) << left << "Balance" << endl;
    cout << "__________________________________________________________________________________________________________________\n" << endl;

    double TotalBalances = 0;
    if (vClients.size() != 0)
    {
    
        for(stClientsData& c : vClients)
        {
            cout << "| " << setw(20) << left << c.AccountNumber << "| " << setw(40) << left << c.Name << "| " << setw(15) << left << c.AccountBalance << endl;
            TotalBalances += c.AccountBalance;
        }
        cout << "\n\t\t\t\tTotal Balances = " << TotalBalances << endl;
    }
    else
    {    
        cout << "\t\t\tNo clients available in the system!";
    }

    GoBackToTransactionsMenu();
}

void WithdrawScreen()
{
    cout << "-----------------------------------------" << endl;
    cout << "\t\tWithdraw Screen\t" <<endl;
    cout << "-----------------------------------------\n" << endl;

    vector <stClientsData> vClients = LoadDataFromFile(ClientsNameFile);
    stClientsData Client;
    string AccountNumber = ReadAccountNumber("Please enter Account Number:\n");
    short UserInput = 0;
    if (!(CheckIfAccountNumberExists(AccountNumber,Client)))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] hasn't been found!\n\nPress 1 to try with different Account Number or 2 to go to the transactions menu......" << endl;
        cin >> UserInput;
        if (UserInput == 1)
        {
            // WithdrawScreen();
        }
        else if (UserInput == 2)
        {
            GoBackToTransactionsMenu();
        } 
        else
        {
            ErrorMessageInTransactionsPage();
        }    
    }

    else
    {
        PrintClientData(Client);
        int WithdrawAmount = ReadNumInput("\nPlease enter the amount you want to withdraw?\n");
        double AvailableBalance = Client.AccountBalance;

        while (WithdrawAmount > AvailableBalance)
        {
            cout << "\nThe amount you want to withdraw exceeds your available balance." << endl;
            cout << "Since you can only withdraw up to : [" << AvailableBalance << "]" << endl;
            cout << "\nPlease enter another amount:\n";
            cin >> WithdrawAmount;
        }
        
        char UserConfirmation = ClientConfirmationMessage("\nAre sure you want to confirm this operation? (y/n)\n");
        if (toupper(UserConfirmation) != 'Y' )
        {
            cout << "\nDon't worry! We're not going to confirm this operation." << endl;
            GoBackToTransactionsMenu();
        }

        else
        {
            // DecreaseAmountToAClient(vClients, AccountNumber, WithdrawAmount); OR:
            IncreaseBalance(vClients, AccountNumber, WithdrawAmount * -1);

            SaveClientDataToFile(vClients);
            cout << "\nOperation has been completed successfully!\nYour balance now is: " << AvailableBalance - WithdrawAmount << endl;
        }
    }
}

void PerformTransactionsPage(enTransactionsMenuOptions TransactionsMenuOptions)
{
    switch (TransactionsMenuOptions)
    {
    case enTransactionsMenuOptions::Deposit:
    {   
        system("cls");
        DepositToAClientScreen();
        break;
    }
    case enTransactionsMenuOptions::Withdraw:
    {   
        system("cls");
        WithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case enTransactionsMenuOptions::TotalBalancesForAllClients :
    {
        system("cls");
        TotalBalancesScreen();
        break;
    }
    case enTransactionsMenuOptions::MainMenu:
    {
        ShowMainMenuOptions();
    
    }
    }
}

void ShowTransactionsScreen()
{
    system("cls");
    cout << "=================================================" << endl;
    cout << "\t\tTransactions Menu Screen\t\t\t\t" << endl;
    cout << "=================================================" << endl << endl;
    cout << "\t[1] Deposit." << endl;
    cout << "\t[2] Withdraw." << endl;
    cout << "\t[3] Total Balance For All Clients." << endl;
    cout << "\t[4] Go Back To Main Menu.\n" << endl;
    cout << "=================================================" << endl << endl;

    PerformTransactionsPage((enTransactionsMenuOptions)ReadNumInput("Please Enter a Number From (1 to 4), What Do You Want To Do?\n"));
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void StartApp(enMainMenuOptions MainMenuOption)
{

    switch (MainMenuOption)
    {
    case enMainMenuOptions::ShowClientList :
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::AddNewClient :
        system("cls");
        AddUser();
        ShowMainMenuOptions();
        break;
    
    case enMainMenuOptions::DeleteClient :
        system("cls");
        DeleteClientByAccountNumber();
        GoBackToMainMenu();
        break;
    
    case enMainMenuOptions::UpdateClientInfo :
        system("cls");
        UpdateClientData();
        GoBackToMainMenu();
        break;    
    
    case enMainMenuOptions::FindClient :
        system("cls");
        FindClientByAccountNumber();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::Transactions:
        system("cls");
        ShowTransactionsScreen();
        GoBackToMainMenu();
        break;
    
    case enMainMenuOptions::Exit :
        system("cls");
        ShowEndScreen();    // Exit the function and program
        break;
    }
}

short ReadMainMenuOption()
{
    cout << "What do you want to do? ";
    short UserOption = 0;
    cin >> UserOption;
    return UserOption;
}
void ShowMainMenuOptions()
{
    ClearScreen();
    cout << "=================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "=================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n" ;
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "=================================================\n";

    StartApp((enMainMenuOptions)ReadMainMenuOption());
}

int main()
{
    ShowMainMenuOptions();
    system("pause>0");

    return 0;
}

