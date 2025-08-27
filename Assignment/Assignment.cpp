#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <regex>
#include <cctype>
#include <ctype.h>
#include <ctime>

using namespace std;

void mainMenu();
void clearScreen();
void userRegister();
void userLogin();
bool isEmailValid(string email);
void userMenu(string userName);
void adminMenu(string userName);
string toLowerSTR(string strings);
void createEventSeats();
bool isFutureDate(string dateStr);
bool timeValid(const string &timeStr);
void displayUpcomingConcert();

struct User
{
    string username;
    string password;
    string email;
};

struct Seat
{
    char row;       // A, B, C...
    int number;     // 1–10
    string section; // VIP / Regular / Economy
    double price;
    bool isBooked;
};

struct Concert
{
    string concertName;
    string artist;
    string venue;
    string date;
    string time;
};

vector<Concert> loadConcerts(const string &filename);

// main function
int main()
{
    mainMenu();
    return 0;
}


void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}


void mainMenu()
{
    int choice;

    clearScreen();
    cout << "Welcome to blah blah blah\n"
         << endl;
    displayUpcomingConcert();
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

        if (toLowerSTR(newUser.username) == "admin")
        {
            cout << "'ADMIN' is a reserved word. Please try another.\n";
            continue;
        }

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
    bool pwMatch = false;
    vector<User> users;

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
    while (getline(readFile, line))
    {
        if (line.empty())
            continue;
        User u;
        int pos1 = line.find(";");
        int pos2 = line.rfind(";");

        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2)
            continue;

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

        if (toLowerSTR(userName) == "admin" && password == "admin")
        {
            adminMenu(userName);
            return; // exit the program
        }

        bool existUser = false;
        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].username == userName)
            {
                existUser = true;
                if (users[i].password == password)
                {
                    pwMatch = true;
                    break;
                }
                else
                {
                    clearScreen();
                    cout << "User Login Failed\nPlease try again\n\nUsername or password incorrect.\n";
                }
            }
        }
        if (!existUser)
        {
            clearScreen();
            cout << "User Login Failed\nPlease try again\n\nUsername or password incorrect.\n";
        }
    }
    if (pwMatch)
    {
        clearScreen();
        cout << "login successful";
        userMenu(userName);
    }
}
string toLowerSTR(string strings)
{

    string result = strings;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
void adminMenu(string userName)
{
    int choice;
    while (true)
    {
        clearScreen();
        cout << "Welcome " << userName << "!" << endl;
        cout << "1. Event Creation\n2. Event Monitoring\n3. Event Reporting\n4. Logout and exit.";
        cout << "\nEnter your choice :";
        cin >> choice;
        switch (choice)
        {
        case 1:
            createEventSeats();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            return;
        default:
            cout << "Invalid option. Try again.";
        }
    }
}
void userMenu(string userName)
{
    int choice;
    cout << "\nWelcome " << userName << "!" << endl;
    cout << "1. Event Register\n2. Profile\n4. Logout and exit.";
    // profile is to view past ticket also with update email and password
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
        case 4:
            return;
        default:
            cout << "Invalid option. Try again.";
        }
    }
}

bool isFutureDate(string dateStr)
{
    // allow using / instead of -
    for (char &c : dateStr)
    {
        if (c == '/')
            c = '-';
    }

    // Extract day, month, year
    int day, month, year;
    char dash1, dash2;
    stringstream ss(dateStr);
    ss >> day >> dash1 >> month >> dash2 >> year;

    // Check if day, month and year are separated
    if (dash1 != '-' || dash2 != '-')
    {
        cout << "Please use \"/\" or \"-\".\n";
        return false;
    }

    // Basic range check
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900)
    {
        cout << "Please enter a valid date.\n";
        return false;
    }

    // Get current system date
    time_t now = time(0);
    tm current = *localtime(&now);

    int currentDay = current.tm_mday;
    int currentMonth = current.tm_mon + 1;
    int currentYear = current.tm_year + 1900;

    // Compare year, month, day
    if (year > currentYear)
        return true;
    if (year == currentYear && month > currentMonth)
        return true;
    if (year == currentYear && month == currentMonth && day >= currentDay)
        return true;

    cout << "Please enter a future date.\n";
    return false;
}

bool timeValid(const string &timeStr)
{
    // must use :
    if (timeStr.find(':') == string::npos)
    {
        cout << "Please use HH:MM as the format.\n";
        return false;
    }

    int hour, minute;
    char colon;
    stringstream ss(timeStr);
    ss >> hour >> colon >> minute;

    // Check basic time format
    if (colon != ':' || hour < 0 || hour > 23 || minute < 0 || minute > 59)
    {
        cout << "Hours must be 00-23 and minutes 00-59.\n";
        return false;
    }
    return true;
}

void createEventSeats()
{

    clearScreen();
    string eventName, artistName, eventVenue, eventDate, startTime, endTime;
    cin.ignore();
    cout << "Enter concert Name: ";
    getline(cin, eventName);
    cout << "Enter Artist Name(s): ";
    getline(cin, artistName);
    cout << "Enter concert Venue: ";
    getline(cin, eventVenue);

    do
    {
        cout << "Enter concert date (dd-mm-yyyy or d-m-yyyy): ";
        getline(cin, eventDate);
    } while (!isFutureDate(eventDate));

    do
    {
        cout << "Enter start time (HH:MM): ";
        getline(cin, startTime);
    } while (!timeValid(startTime));

    do
    {
        cout << "Enter End Time (HH:MM): ";
        getline(cin, endTime);
    } while (!timeValid(endTime));

    int rows, columns, vipRows, regularRows;
    cout << "Enter number of rows (max 26): ";
    cin >> rows;
    while (rows < 10)
    {
        cout << "At least 10 rows are required. Please enter again: ";
        cin >> rows;
    }
    while (rows > 26)
    {
        cout << "Maximum 26 rows. Please enter again: ";
        cin >> rows;
    }

    cout << "Enter number of seats/row: ";
    cin >> columns;
    while (columns < 10)
    {
        cout << "At least 10 seats per row. Please enter again: ";
        cin >> columns;
    }
    while (columns > 25)
    {
        cout << "Cannot exceed 25 seats per row. Please enter again: ";
        cin >> columns;
    }

    // categorize rows
    while (true)
    {
        cout << "Enter number of VIP rows: ";
        cin >> vipRows;
        cout << "Enter number of Regular rows: ";
        cin >> regularRows;

        if (vipRows + regularRows > rows)
        {
            cout << "VIP + Regular rows exceed total rows. Try again.\n";
        }
        else
        {
            break;
        }
    }

    string seatFileName = "seats_" + eventName + ".txt";
    // make filename nice one
    replace(seatFileName.begin(), seatFileName.end(), ' ', '_');

    // seats de layout
    clearScreen();
    cout << "Preview:\n\n";

    for (int i = 0; i < rows; i++)
    {
        if (i == 0)
            cout << "VIP:\n";
        else if (i == vipRows)
            cout << "\nRegular:\n";
        else if (i == vipRows + regularRows)
            cout << "\nNormal:\n";

        for (int j = 1; j <= columns; j++)
        {
            cout << "[" << (char)('A' + i) << j << "] ";
        }
        cout << endl;
    }

    // set seat prices
    double vipPrice, regPrice, normalPrice;
    cout << "\nSet the price\n";
    cout << "Enter VIP seat price: RM ";
    cin >> vipPrice;
    cout << "Enter Regular seat price: RM ";
    cin >> regPrice;
    cout << "Enter Normal seat price: RM ";
    cin >> normalPrice;

    vector<Seat> seatList;
    for (int i = 0; i < rows; i++)
    {
        string section;
        if (i < vipRows)
            section = "VIP";
        else if (i < vipRows + regularRows)
            section = "Regular";
        else
            section = "Normal";

        for (int j = 1; j <= columns; j++)
        {
            Seat seat;
            seat.row = 'A' + i;
            seat.number = j;
            seat.section = section;
            seat.isBooked = false;

            if (section == "VIP")
                seat.price = vipPrice;
            else if (section == "Regular")
                seat.price = regPrice;
            else
                seat.price = normalPrice;

            seatList.push_back(seat);
        }
    }

    // Save events
    ofstream eventFile("events.txt", ios::app);
    eventFile << eventName << ";" << artistName << ";" << eventVenue << ";"
              << eventDate << ";" << startTime << ";" << endTime << ";"
              << seatFileName << endl;
    eventFile.close();

    // file validation
    ofstream file(seatFileName);
    if (!file)
    {
        cout << "Error creating seat file.\n";
        return;
    }

    // Save seats
    for (Seat s : seatList)
    {
        file << s.row << ";" << s.number << ";" << s.section << ";"
             << s.price << ";" << s.isBooked << "\n";
    }
    file.close();

    cout << "\nEvent and seat layout with pricing saved successfully.\nBack to admin menu in 3 second.";
    Sleep(3000);
}

vector<Concert> loadConcerts(const string &filename)
{
    vector<Concert> concerts;
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string concertName, artist, venue, date, time;

        getline(ss, concertName, ';');
        getline(ss, artist, ';');
        getline(ss, venue, ';');
        getline(ss, date, ';');
        getline(ss, time, ';');

        // Push into vector
        concerts.push_back({concertName, artist, venue, date, time});
    }
    return concerts;
}

void displayUpcomingConcert()
{
    static size_t index = 0; // remembers last shown concert
    vector<Concert> concerts = loadConcerts("events.txt");

    if (concerts.empty())
    {
        cout << "=================================\n";
        cout << "No upcoming event.\n";
        cout << "=================================\n\n";
        return;
    }

    Concert c = concerts[index];
    cout << "COMING SOON\n";
    cout << "Title:  " << c.concertName << "\n";
    cout << "Artist: " << c.artist << "\n";
    cout << "Venue:  " << c.venue << "\n";
    cout << "Date:   " << c.date << "\n";
    cout << "Time:   " << c.time << "\n";
    cout << "-----------------------------------\n\n";

    index = (index + 1) % concerts.size(); // rotate
}