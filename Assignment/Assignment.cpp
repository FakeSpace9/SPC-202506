#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <regex>

using namespace std;

void mainMenu();
void clearScreen();
void userRegister();
void userLogin();
bool isEmailValid(string email);
void userMenu(string userName);

struct User
{
    string username;
    string password;
    string email;
};

// main function
int main()
{
    mainMenu();
    return 0;
}

void mainMenu()
{
    int choice = -1;

    cout << "Welcome to blah blah blah" << endl;
    cout << "\n1. User Registeration.\n2. Login\n3. Exit\nEnter your choice : ";

    cin >> choice;

    switch (choice)
    {
    case 1:
        userRegister();
        break;

    case 2:
        userLogin();
        break;

    case 3:
        return;

    default:
        cout << "Invalid option, Please try again.";
        Sleep(3000);
        clearScreen();
        mainMenu();
        break;
    }
}

void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}

void userRegister()
{
    cout << "User Registration";
    clearScreen();
    User newUser;
    string confirmPassword;

    // load all the user data into vector
    vector<User> users;
    ifstream readFile("userDetails.txt");
    string line;
    while (!readFile.eof())
    {
        getline(readFile, line);
        User u;
        int pos1 = line.find(";");
        int pos2 = line.rfind(";");

        u.username = line.substr(0, pos1);
        u.email = line.substr(pos1 + 1, pos2 - pos1 - 1);
        u.password = line.substr(pos2 + 1);
        users.push_back(u);
    }
    readFile.close();

    // validation for username
    while (true)
    {
        cout << "Enter Username: ";
        cin >> newUser.username;

        bool exists = false;
        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].username == newUser.username)
            {
                exists = true;
                break;
            }
        }

        if (newUser.username.length() < 3)
        {
            cout << "Username must be at least 3 characters long.\n";
        }
        else if (exists) // prompt user name taken
        {
            cout << "Username already taken. Please choose another.\n";
        }
        else
        {
            break;
        }
    }

    // validation for password
    while (true)
    {
        cout << "Enter Password: ";
        cin >> newUser.password;
        cout << "Confirm Password: ";
        cin >> confirmPassword;

        if (newUser.password != confirmPassword)
        {
            cout << "Passwords do not match. Try again.\n";
        }
        else if (newUser.password.length() < 8)
        {
            cout << "Password must be at least 8 characters long.\n";
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        cout << "Enter Email: ";
        cin >> newUser.email;
        if (!isEmailValid(newUser.email))
        {
            cout << "Invalid Email format.\n";
        }
        else
        {
            break;
        }
    }

    ofstream file; // open or create the file call userDetails
    file.open("userDetails.txt", ios::app);
    if (!file)
    {
        cout << "File cannot be open.";
        return;
    }
    file << newUser.username << ";" << newUser.email << ";" << newUser.password << "\n";
    file.close();

    cout << "Register Successfully. \nReturn to main menu in 3 second.";
    Sleep(3000);
    clearScreen();
    main(); // back to main menu
}

bool isEmailValid(string email)
{
    regex userEmailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex_match(email, userEmailRegex);
}

void userLogin()
{
    clearScreen();
    string password;
    string userName;
    bool pwMatch;
    vector<User> users;
    bool existUser;

    // create or open file first
    ofstream ensureFile("userDetails.txt", ios::app);
    ensureFile.close();
    // try to read file
    ifstream readFile("userDetails.txt");

    if (!readFile)
    {
        cout << "File cannot be open.";
        return;
    }
    string line;

    cout << "User Login\n";

    // read user data to vector

    while (!readFile.eof())
    {
        getline(readFile, line);
        User u;
        int pos1 = line.find(";");
        int pos2 = line.rfind(";");

        u.username = line.substr(0, pos1);
        u.email = line.substr(pos1 + 1, pos2 - pos1 - 1);
        u.password = line.substr(pos2 + 1);
        users.push_back(u);
    }
    readFile.close();

    while (!pwMatch)
    {
        cout << "Enter your username : ";
        cin >> userName;
        cout << "Enter password : ";
        cin >> password;

        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].username == userName)
            {
                if (users[i].password == password)
                {
                    pwMatch = true;
                    break;
                }
                else
                {
                    cout << "Username or password incorrect.\n";
                }
            }
        }
        if (!existUser)
        {
            cout << "Username or password incorrect.\n";
        }
    }
    if (pwMatch)
    {
        clearScreen();
        cout << "login successful";
        userMenu(userName);
    }
}
void userMenu(string userName)
{
    int choice;
    cout << "\nWelcome " << userName << "!" << endl;
    cout << "1. Event Register\n2. Event Monitoring\n3. Event Reporting";
    while (true)
    {
        cout << "\nEnter your choice :";
        cin >> choice;
        switch (choice)
        {
        case 1:
            // code
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            cout << "Invalid option. Try again.";
        }
    }
}

// help