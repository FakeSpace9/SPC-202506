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
#include <thread>
#include <chrono>
#include <limits>
#include <ctype.h>
#include <conio.h>
#include <ctime>
#include <iomanip>

using namespace std;


struct User {
    string username;
    string password;
    string email;
};

struct Seat {
    char row; // A, B, C...
    int number; // 1–10
    string section; // VIP / Regular / Economy
    double price;
    bool isBooked;
};

struct Concert {
    string concertName;
    string artist;
    string venue;
    string date;
    string startTime;
    string endTime;
};

vector<Concert> loadConcerts(const string &filename);

struct EventStatus {
    string eventName;
    string manualStatus;
    string gateTime;
    string entryTime;
    string startTime;
    string endTime;
    string date;
};

struct TimeEvent {
    string key;
    string value;
};

struct Booking {
    string userName;
    string eventName;
    string eventDate;
    string seats;
    double totalPrice;
    string paymentMethod;
    string bookingDateTime;
    string eventStatus;
};

void displayAvailableEvent(const string &userName);

void viewBookingHistory(const string &userName);

void searchBookings(const vector<Booking> &bookings, const string &userName);

void sortBookingsByDate(vector<Booking> &bookings);

vector<Booking> loadUserBookings(const string &userName);

string determineEventStatus(const string &eventDate);

void displayBookingDetail(const Booking &booking, const string &userName);

Concert getBookingConcertDetails(const string &eventName);

void saveEventStatus(const EventStatus &status);

void loadEventStatus(const string &eventName, EventStatus &status);

void updateEventTimes(EventStatus &status, const Concert &concert);

void displayStatusUpdateMenu(EventStatus &status, const Concert &concert);

void displayMonitoring(const Concert &c);

string getCurrentDateTime();

void eventMonitoring();

void fileComplaint(const string &userName, const string &eventName, const string &userType);

void mainMenu();

void clearScreen();

void userRegister();

void userLogin();

bool isEmailValid(string email);

void userMenu(string userName);

void adminMenu();

string toLowerSTR(string strings);

void createEventSeats();

bool isFutureDate(string dateStr);

bool timeValid(const string &timeStr);

void displayUpcomingConcert();

void eventRegistration(const string &userName,const int choice);

bool checkoutAndPayment(const string &userName, const Concert &concert, const vector<Seat> &selectedSeats,
                        double totalPrice);

string getPasswordInput();

void eventFeedback(string userName);

// main function
int main() {
    mainMenu();
    return 0;
}


void clearScreen() {
    cout << "\033[2J\033[1;1H";
}


void mainMenu() {
    int choice;

    clearScreen();
    cout << "Welcome to blah blah blah\n"
            << endl;
    displayUpcomingConcert();
    cout << "1. User Registration.\n2. Login\n3. Exit\nEnter your choice : ";

    cin >> choice;

    switch (choice) {
        case 1:
            userRegister();
            break;

        case 2:
            userLogin();
            break;

        case 3:
            return;

        default:
            cout << "Invalid option, Please try again in 3 second.";
            Sleep(3000);
            clearScreen();
            mainMenu();
            break;
    }
}

void userRegister() {
    cout << "User Registration";
    clearScreen();
    User newUser;
    string confirmPassword;

    // load all the user data into vector
    vector<User> users;
    ifstream readFile("userDetails.txt");
    string line;
    while (!readFile.eof()) {
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
    while (true) {
        cout << "Enter Username (back to return) : ";
        cin >> newUser.username;

        if (toLowerSTR(newUser.username) == "back") {
            mainMenu();
        }

        if (toLowerSTR(newUser.username) == "admin") {
            cout << "'ADMIN' is a reserved word. Please try another.\n";
            continue;
        }

        bool exists = false;
        for (int i = 0; i < users.size(); i++) {
            if (users[i].username == newUser.username) {
                exists = true;
                break;
            }
        }

        if (newUser.username.length() < 3) {
            cout << "Username must be at least 3 characters long.\n";
        } else if (exists) // prompt username taken
        {
            cout << "Username already taken. Please choose another.\n";
        } else {
            break;
        }
    }

    // validation for password
    while (true) {
        bool isPassG8 = false;
        do {
            cout << "Enter Password: ";
            newUser.password = getPasswordInput();
            if (newUser.password.length() < 8) {
                cout << "Password must be at least 8 characters long.\n";
            } else {
                isPassG8 = true;
                break;
            }
        } while (!isPassG8);
        cout << "Confirm Password: ";
        confirmPassword = getPasswordInput();

        if (newUser.password != confirmPassword) {
            cout << "Passwords do not match. Try again.\n";
        } else { break; }
    }


    while (true) {
        cout << "Enter Email: ";
        cin >> newUser.email;
        if (!isEmailValid(newUser.email)) {
            cout << "Invalid Email format.\n";
        } else {
            break;
        }
    }

    ofstream file; // open or create the file call userDetails
    file.open("userDetails.txt", ios::app);
    if (!file) {
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

bool isEmailValid(string email) {
    regex userEmailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex_match(email, userEmailRegex);
}

string getPasswordInput() {
    string password;
    char ch;
    while (true) {
        ch = _getch(); // get character without showing on console
        if (ch == 13) {
            // Enter key
            cout << endl;
            break;
        } else if (ch == 8) {
            // Backspace
            if (!password.empty()) {
                cout << "\b \b"; // erase last character from console
                password.pop_back();
            }
        } else {
            password.push_back(ch);
            cout << '*'; // print * instead of actual character
        }
    }
    return password;
}

void userLogin() {
    clearScreen();
    string password;
    string userName;
    bool pwMatch = false;
    vector<User> users;
    clearScreen();

    // create or open file first
    ofstream ensureFile("userDetails.txt", ios::app);
    ensureFile.close();
    // try to read file
    ifstream readFile("userDetails.txt");

    if (!readFile) {
        cout << "File cannot be open.";
        return;
    }
    string line;

    cout << "==================\n";
    cout << "User Login\n";
    cout << "==================\n";

    // read user data to vector
    while (getline(readFile, line)) {
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

    while (!pwMatch) {
        cout << "Enter your username (back to return): ";
        cin >> userName;
        if (toLowerSTR(userName) == "back") {
            mainMenu();
        }

        cout << "Enter password : ";
        password = getPasswordInput();


        if (toLowerSTR(userName) == "admin" && password == "admin") {
            adminMenu();
            return; // exit the program
        }

        bool existUser = false;
        for (int i = 0; i < users.size(); i++) {
            if (users[i].username == userName) {
                existUser = true;
                if (users[i].password == password) {
                    pwMatch = true;
                    break;
                } else {
                    clearScreen();
                    cout << "User Login Failed\nPlease try again\n\nUsername or password incorrect.\n";
                }
            }
        }
        if (!existUser) {
            clearScreen();
            cout << "User Login Failed\nPlease try again\n\nUsername or password incorrect.\n";
        }
    }
    if (pwMatch) {
        clearScreen();
        cout << "login successful";
        userMenu(userName);
    }
}

string toLowerSTR(string strings) {
    string result = strings;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void eventFeedback() {
    bool feedbackExist = false;
    while (!feedbackExist) {
        clearScreen();
        cout << "========================\n";
        cout << "Event Feedback\n";
        cout << "========================\n\n";

        vector<Concert> concerts = loadConcerts("events.txt");
        vector<pair<string, int> > eventFeedbacks;
        // save as eventname, number of feedback. eventFeedbacks is the name of vector

        for (const Concert &concert: concerts) {
            // const ensure not modify the file
            //for each concert in the list of concerts, give me a read-only reference called concert that I can use inside the loop.
            string issueFile = "issues_" + concert.concertName + ".txt";
            replace(issueFile.begin(), issueFile.end(), ' ', '_');

            ifstream file(issueFile);
            if (file) {
                int count = 0;
                string line;
                while (getline(file, line)) {
                    if (!line.empty()) count++;
                }
                if (count > 0) {
                    eventFeedbacks.push_back({concert.concertName, count});
                }
                file.close();
            }
        }

        if (eventFeedbacks.empty()) {
            cout << "No feedback found for any events.\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        cout << "Events with Feedback:\n";
        for (size_t i = 0; i < eventFeedbacks.size(); i++) {
            cout << (i + 1) << ". " << eventFeedbacks[i].first
                    << " (" << eventFeedbacks[i].second << " feedback(s))\n";
        }


        int choice;
        bool validChoice = false;

        while (!validChoice) {
            cout << "\nEnter event number to view feedback (0 to return): ";
            cin >> choice;
            if (choice > 0 && choice <= eventFeedbacks.size()) {
                validChoice = true;
                clearScreen();
                string selectedEvent = eventFeedbacks[choice - 1].first;
                string issueFile = "issues_" + selectedEvent + ".txt";
                replace(issueFile.begin(), issueFile.end(), ' ', '_');

                cout << "Feedback for: " << selectedEvent << "\n\n";

                ifstream file(issueFile);
                string line;
                while (getline(file, line)) {
                    if (!line.empty()) {
                        cout << line << "\n";
                    }
                }
                file.close();

                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            } else if (choice == 0) {
                feedbackExist = true;
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
}

void adminMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "Welcome Admin !" << endl;
        cout << "1. Event Creation\n2. Event Monitoring\n3. Event Reporting\n4. Event Feedback\n0. Logout";
        cout << "\nEnter your choice :";

        if (cin >> choice) {
            switch (choice) {
                case 1:
                    createEventSeats();
                    break;
                case 2:
                    eventMonitoring();
                    break;
                case 3:
                    cout << "\nEvent Monitoring is not implemented yet.\n";
                    break;
                case 4:
                    eventFeedback();
                    break;
                case 0:
                    clearScreen();
                    cout << "Logout Successful";
                    for (int i = 0; i < 3; i++) {
                        cout << ".";
                        cout.flush();
                        Sleep(700);
                    }
                    mainMenu();
                    return;
                default:
                    clearScreen();
                    cout << "Invalid option. Try again.\n";
            }
        } else {
            clearScreen();
            cout << "Invalid option. Try again.\n";
            choice = 0;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

void userMenu(string userName) {
    int choice;

    while (true) {
        cout << "\nWelcome " << userName << "!" << endl;
        cout << "1. Event Register\n2. Booking History\n3. Logout";
        // profile is to view past ticket also with update email and password
        cout << "\nEnter your choice :";

        if (cin >> choice) {
            switch (choice) {
                case 1:
                    displayAvailableEvent(userName);
                    break;
                case 2:
                    viewBookingHistory(userName);
                    break;
                case 3:
                    clearScreen();
                    cout << "Logout Successful";
                    for (int i = 0; i < 3; i++) {
                        cout << ".";
                        cout.flush();
                        Sleep(700);
                    }
                    mainMenu();
                    return;
                default:
                    clearScreen();
                    cout << "Invalid option. Try again.";
                    choice = 0;
            }
        } else {
            clearScreen();
            cout << "Invalid option. Try again.";
            choice = 0;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

bool isFutureDate(string dateStr) {
    // allow using / instead of -
    for (char &c: dateStr) {
        if (c == '/')
            c = '-';
    }

    // Extract day, month, year
    int day, month, year;
    char dash1, dash2;
    stringstream ss(dateStr);
    ss >> day >> dash1 >> month >> dash2 >> year;

    // Check if day, month and year are separated
    if (dash1 != '-' || dash2 != '-') {
        cout << "Please use \"/\" or \"-\".\n";
        return false;
    }

    // Basic range check
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
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

bool timeValid(const string &timeStr) {
    // must use :
    if (timeStr.find(':') == string::npos) {
        cout << "Please use HH:MM as the format.\n";
        return false;
    }

    int hour, minute;
    char colon;
    stringstream ss(timeStr);
    ss >> hour >> colon >> minute;

    // Check basic time format
    if (colon != ':' || hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        cout << "Hours must be 00-23 and minutes 00-59.\n";
        return false;
    }
    return true;
}

void createEventSeats() {
    clearScreen();
    string eventName, artistName, eventVenue, eventDate, startTime, endTime;
    int rows, columns, vipRows, regularRows;
    cin.ignore();
    do {
        cout << "Enter concert Name (back to cancel this process): ";
        getline(cin, eventName);
        if (toLowerSTR(eventName) == "back") {
            clearScreen();
            return;
        }
        if (eventName.empty()) {
            cout << "Event name cannot be empty. Please enter again\n";
        }
    } while (eventName.empty());

    do {
        cout << "Enter Artist Name(s): ";
        getline(cin, artistName);
        if (artistName.empty()) {
            cout << "Event name cannot be empty. Please enter again\n";
        }
    } while (artistName.empty());

    do {
        cout << "Enter concert Venue: ";
        getline(cin, eventVenue);
        if (eventVenue.empty()) {
            cout << "Event name cannot be empty. Please enter again\n";
        }
    } while (eventVenue.empty());

    do {
        cout << "Enter concert date (dd-mm-yyyy): ";
        getline(cin, eventDate);
    } while (!isFutureDate(eventDate));

    do {
        cout << "Enter start time (HH:MM): ";
        getline(cin, startTime);
    } while (!timeValid(startTime));

    do {
        cout << "Enter End Time (HH:MM): ";
        getline(cin, endTime);
    } while (!timeValid(endTime));


    cout << "Enter number of rows (max 26): ";
    cin >> rows;
    while (rows < 10) {
        cout << "At least 10 rows are required. Please enter again: ";
        cin >> rows;
    }
    while (rows > 26) {
        cout << "Maximum 26 rows. Please enter again: ";
        cin >> rows;
    }

    cout << "Enter number of seats/row: ";
    cin >> columns;
    while (columns < 10) {
        cout << "At least 10 seats per row. Please enter again: ";
        cin >> columns;
    }
    while (columns > 25) {
        cout << "Cannot exceed 25 seats per row. Please enter again: ";
        cin >> columns;
    }

    // categorize rows
    while (true) {
        cout << "Enter number of VIP rows: ";
        cin >> vipRows;
        cout << "Enter number of Regular rows: ";
        cin >> regularRows;

        if (vipRows + regularRows > rows) {
            cout << "VIP + Regular rows exceed total rows. Try again.\n";
        } else {
            break;
        }
    }

    string seatFileName = "seats_" + eventName + ".txt";
    // make filename nice one
    replace(seatFileName.begin(), seatFileName.end(), ' ', '_');

    // seats de layout
    clearScreen();
    cout << "Preview:\n\n";

    for (int i = 0; i < rows; i++) {
        if (i == 0)
            cout << "VIP:\n";
        else if (i == vipRows)
            cout << "\nRegular:\n";
        else if (i == vipRows + regularRows)
            cout << "\nNormal:\n";

        for (int j = 1; j <= columns; j++) {
            cout << "[" << (char) ('A' + i) << j << "] ";
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
    for (int i = 0; i < rows; i++) {
        string section;
        if (i < vipRows)
            section = "VIP";
        else if (i < vipRows + regularRows)
            section = "Regular";
        else
            section = "Normal";

        for (int j = 1; j <= columns; j++) {
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
    if (!file) {
        cout << "Error creating seat file.\n";
        return;
    }

    // Save seats
    for (Seat s: seatList) {
        file << s.row << ";" << s.number << ";" << s.section << ";"
                << s.price << ";" << s.isBooked << "\n";
    }
    file.close();

    cout << "\nEvent and seat layout with pricing saved successfully.\nBack to admin menu in 3 second.";
    Sleep(3000);
    clearScreen();
}

vector<Concert> loadConcerts(const string &filename) {
    vector<Concert> concerts;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string concertName, artist, venue, date, startTime, endTime;

        getline(ss, concertName, ';');
        getline(ss, artist, ';');
        getline(ss, venue, ';');
        getline(ss, date, ';');
        getline(ss, startTime, ';');
        getline(ss, endTime, ';');

        // Push into vector
        concerts.push_back({concertName, artist, venue, date, startTime, endTime});
    }
    return concerts;
}

void displayUpcomingConcert() {
    static size_t index = 0; // remembers last shown concert
    vector<Concert> concerts = loadConcerts("events.txt");

    if (concerts.empty()) {
        cout << "=================================\n";
        cout << "No upcoming event.\n";
        cout << "=================================\n\n";
        return;
    }
    Concert c = concerts[index];
    cout << "-----------------------------------\nCOMING SOON\n";
    cout << "Title:  " << c.concertName << "\n";
    cout << "Artist: " << c.artist << "\n";
    cout << "Venue:  " << c.venue << "\n";
    cout << "Date:   " << c.date << "\n";
    cout << "Time:   " << c.startTime << "\n";
    cout << "-----------------------------------\n\n";

    index = (index + 1) % concerts.size(); // rotate
}

// Utility: Load seats from file
vector<Seat> loadSeats(const string &seatFileName) {
    vector<Seat> seats;
    ifstream file(seatFileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string rowStr;
        Seat seat;
        string priceStr, bookedStr;

        getline(ss, rowStr, ';');
        seat.row = rowStr.empty() ? '?' : rowStr[0];
        string numStr;
        getline(ss, numStr, ';');
        seat.number = stoi(numStr);
        getline(ss, seat.section, ';');
        getline(ss, priceStr, ';');
        getline(ss, bookedStr, ';');

        seat.price = stod(priceStr);
        seat.isBooked = (bookedStr == "1" || bookedStr == "true");
        seats.push_back(seat);
    }
    return seats;
}

// Save seats back to file
void saveSeats(const string &seatFileName, const vector<Seat> &seats) {
    ofstream file(seatFileName);
    for (const Seat &s: seats) {
        file << s.row << ";" << s.number << ";" << s.section << ";"
                << s.price << ";" << (s.isBooked ? 1 : 0) << "\n";
    }
}

void displayAvailableEvent(const string &userName) {


    vector<Concert> concerts = loadConcerts("events.txt");
    while (true) {
        clearScreen();
        if (concerts.empty()) {
            cout << "No events available.\n";
            return;
        }

        // Concert selection
        cout << "\nAvailable Events:\n";
        for (size_t i = 0; i < concerts.size(); i++) {
            cout << i + 1 << ". " << concerts[i].concertName
                    << " by " << concerts[i].artist
                    << " at " << concerts[i].venue
                    << " on " << concerts[i].date << " " << concerts[i].startTime << "\n";
        }
        cout << "0. Back\n";

        int choice;
        cout << "Select event number (or 0 to go back): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            return;
        }
        if (choice == 0) {
            clearScreen();
            return;
        } // back to previous menu
        if (choice < 1 || choice > (int) concerts.size()) {
            cout << "Invalid choice.\n";
            return;
        }

        eventRegistration(userName,choice);
    }
}

// event registration function
void eventRegistration(const string &userName,const int choice) {
    size_t consumed = 0;
    int number;
    vector<Concert> concerts = loadConcerts("events.txt");



    Concert selected = concerts[choice - 1];
    string seatFileName = "seats_" + selected.concertName + ".txt";
    replace(seatFileName.begin(), seatFileName.end(), ' ', '_');
    vector<Seat> seats = loadSeats(seatFileName);

    // Show seats
    cout << "\nSeat Layout (Available seats shown as code):\n";

    // Collect prices per section for the legend
    double vipPrice = -1.0, regPrice = -1.0, normalPrice = -1.0;
    for (const Seat &s: seats) {
        if (s.section == "VIP" && vipPrice < 0)
            vipPrice = s.price;
        else if (s.section == "Regular" && regPrice < 0)
            regPrice = s.price;
        else if (s.section == "Normal" && normalPrice < 0)
            normalPrice = s.price;
    }

    // Print rows A to Z, grouped by section
    string lastSectionPrinted = "";
    for (char row = 'A'; row <= 'Z'; row++) {
        // Gather seats for this row
        vector<const Seat *> rowSeats;
        string rowSection;
        for (const Seat &s: seats) {
            if (s.row == row) {
                if (rowSection.empty()) rowSection = s.section;
                rowSeats.push_back(&s);
            }
        }
        if (rowSeats.empty()) continue;

        // Sort seats in this row by seat number for clean display
        sort(rowSeats.begin(), rowSeats.end(),
             [](const Seat *a, const Seat *b) { return a->number < b->number; });

        // Print section header when it changes
        if (rowSection != lastSectionPrinted) {
            cout << "\n=== " << rowSection << " ===\n";
            lastSectionPrinted = rowSection;
        }

        // Print row label and seats
        cout << row << "  ";
        for (const Seat *sp: rowSeats) {
            if (sp->isBooked) cout << "[XX] ";
            else cout << "[" << sp->row << sp->number << "] ";
        }
        cout << "\n";
    }

    // Legend and price summary
    cout << "\nNote: [A1] Available   [XX] Booked\n";
    cout << "Prices: ";
    bool firstOut = true;
    if (vipPrice >= 0) {
        cout << (firstOut ? "" : "  ") << "VIP RM " << vipPrice;
        firstOut = false;
    }
    if (regPrice >= 0) {
        cout << (firstOut ? "" : "  ") << "Regular RM " << regPrice;
        firstOut = false;
    }
    if (normalPrice >= 0) { cout << (firstOut ? "" : "  ") << "Normal RM " << normalPrice; }
    cout << "\n";

    // Seat selection
    vector<Seat> selectedSeats;
    string seatCode;
    while (true) {
        cout << "Enter seat code to select, 'done' to proceed checkout, or 'back' to cancel: ";
        cin >> seatCode;
        string seatLower = toLowerSTR(seatCode);
        if (seatLower == "back") {
            return;
        }

        if (seatLower == "done") {
            if (selectedSeats.empty()) {
                cout << "No seat selected please select at least one seat before proceeding.\n";
            }
            break;
        }
        if (seatCode.size() < 2) {
            cout << "Invalid code format.\n";
            continue;
        }

        seatCode[0] = static_cast<char>(toupper(seatCode[0]));

        char row = seatCode[0];
        string numPart = seatCode.substr(1);

        try {
            number = stoi(seatCode.substr(1), &consumed);
        } catch (...) {
            cout << "Invalid seat code.\n";
            continue;
        }
        if (consumed != numPart.size()) {
            cout << "Invalid seat code.\n";
            continue;
        }

        bool found = false;
        for (Seat &s: seats) {
            if (s.row == row && s.number == number && !s.isBooked) {
                selectedSeats.push_back(s);
                s.isBooked = true;
                cout << "Seat " << seatCode << " added.\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "Seat not available.\n";
    }

    if (selectedSeats.empty()) {
        cout << "No seats selected.\n";
        return;
    }

    // ==== Price calculation ====
    double totalPrice = 0;
    for (const Seat &s: selectedSeats) totalPrice += s.price;

    // ==== Checkout ====
    clearScreen();
    if (checkoutAndPayment(userName, selected, selectedSeats, totalPrice)) {
        saveSeats(seatFileName, seats);
        cout << "Booking confirmed!\n";
        cout << "Press enter to return to user menu...";
        cin.get();

        clearScreen();
    } else {
        cout << "Payment failed.\n";
        Sleep(3000);
        clearScreen();
    }
}


bool checkoutAndPayment(const string &userName, const Concert &concert, const vector<Seat> &selectedSeats,
                        double totalPrice) {
    while (true) {
        cout << "\n=== CHECKOUT ===\n";
        cout << "User: " << userName << "\n";
        cout << "Concert: " << concert.concertName << " on " << concert.date << "\n";
        cout << "Seats: ";
        for (auto &s: selectedSeats) cout << s.row << s.number << " ";
        cout << "\nTOTAL: RM " << totalPrice << "\n\n";

        cout << "Select payment method:\n"
                << "0. Back to seat selection\n"
                << "1. Credit/Debit Card\n"
                << "2. Online Banking\n"
                << "3. E-Wallet\n"
                << "Enter your choice: ";

        int method;
        if (!(cin >> method) || method < 0 || method > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            cout << "Invalid payment method.\n";
            continue;
        }
        if (method == 0) return false; // back to seat selection

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string detail;

        while (true) {
            if (method == 1) {
                cout << "Enter card number (or type 'back'): ";
            } else if (method == 2) {
                cout << "Enter bank name (or type 'back'): ";
            } else {
                cout << "Enter e-wallet ID/number (or type 'back'): ";
            }
            getline(cin, detail);
            if (detail == "back") break; // go back to choose method
            if (detail.empty()) {
                cout << "Input cannot be empty.\n";
                continue;
            }

            clearScreen();
            cout << "\nProcessing payment";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                cout.flush();
                Sleep(700);
            }

            clearScreen();
            cout << "\nPayment approved!\n";

            cout << "\n===== RECEIPT =====\n";
            cout << "Customer: " << userName << "\n";
            cout << "Concert: " << concert.concertName << "\n";
            cout << "Date: " << concert.date << "\nSeats: ";
            for (auto &s: selectedSeats) cout << s.row << s.number << " ";
            cout << "\nTotal Paid: RM " << totalPrice << "\n";
            cout << "Payment Method: " << (method == 1 ? "Card" : method == 2 ? "Online Banking" : "E‑Wallet") << "\n";
            cout << "===================\n";

            ofstream hist("purchase_history.txt", ios::app);
            if (hist) {
                hist << userName << ";"
                        << concert.concertName << ";"
                        << concert.date << ";";
                for (size_t i = 0; i < selectedSeats.size(); i++) {
                    hist << selectedSeats[i].row << selectedSeats[i].number;
                    if (i < selectedSeats.size() - 1) hist << ",";
                }
                hist << ";" << totalPrice << ";"
                        << (method == 1 ? "Card" : method == 2 ? "OnlineBanking" : "EWallet")
                        << "\n";
            }
            return true; // payment done
        }
    }
}

void eventMonitoring() {
    vector<Concert> concerts = loadConcerts("events.txt");

    while (true) {
        clearScreen();
        cout << "========== EVENT MONITORING ==========\n";

        if (concerts.empty()) {
            cout << "NO EVENT. THIS WILL BE BACK";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                cout.flush();
                Sleep(700);
            }
            clearScreen();
            return;
        }

        // Display all concerts
        for (size_t i = 0; i < concerts.size(); i++) {
            cout << (i + 1) << ". Concert: " << concerts[i].concertName << endl;
        }
        cout << "0. Back to Admin Menu\n";
        cout << "Enter choice: ";

        int choice;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            cout << "Press Enter to try again...";
            cin.get();
            continue;
        }

        if (choice == 0) {
            clearScreen();
            return;
        }

        if (choice < 1 || choice > (int) concerts.size()) {
            cout << "Invalid choice! Please select a number between 1 and "
                    << concerts.size() << ".\n";
            cout << "Press Enter to try again...";
            cin.ignore();
            cin.get();
            continue;
        }

        displayMonitoring(concerts[choice - 1]);
        return;
    }
}

void saveEventStatus(const EventStatus &status) {
    string statusFile = "schedule_" + status.eventName + ".txt";
    replace(statusFile.begin(), statusFile.end(), ' ', '_');

    stringstream ss;
    ss << "EventName=" << status.eventName << "\n";
    ss << "ManualStatus=" << status.manualStatus << "\n";
    ss << "GateTime=" << status.gateTime << "\n";
    ss << "EntryTime=" << status.entryTime << "\n";
    ss << "StartTime=" << status.startTime << "\n";
    ss << "EndTime=" << status.endTime << "\n";
    ss << "Date=" << status.date << "\n";

    ofstream file(statusFile);
    if (file.is_open()) {
        file << ss.str();
        file.close();
    }
}

// Load event status from file
void loadEventStatus(const string &eventName, EventStatus &status) {
    status.eventName = eventName;
    status.manualStatus = "";

    string statusFile = "schedule_" + eventName + ".txt";
    replace(statusFile.begin(), statusFile.end(), ' ', '_');

    vector<TimeEvent> timeEvents;
    ifstream file(statusFile);
    if (file.is_open()) {
        string line;

        while (getline(file, line)) {
            stringstream lineStream(line);
            string key, value;

            if (getline(lineStream, key, '=') && getline(lineStream, value)) {
                timeEvents.push_back({key, value});
            }
        }
        file.close();

        for (const TimeEvent &event: timeEvents) {
            if (event.key == "ManualStatus") status.manualStatus = event.value;
            else if (event.key == "GateTime") status.gateTime = event.value;
            else if (event.key == "EntryTime") status.entryTime = event.value;
            else if (event.key == "StartTime") status.startTime = event.value;
            else if (event.key == "EndTime") status.endTime = event.value;
            else if (event.key == "Date") status.date = event.value;
        }
    }
}

// Update event times (if no manual override, use auto calculation)
void updateEventTimes(EventStatus &status, const Concert &concert) {
    auto calcTime = [](string t, int minusHour, int minusMin) {
        int h, m;
        char colon;
        stringstream ss(t);
        ss >> h >> colon >> m;
        // subtract
        m -= minusMin;
        if (m < 0) {
            m += 60;
            h -= 1;
        }
        h -= minusHour;
        if (h < 0) h += 24;
        char buf[6];
        sprintf_s(buf, "%02d:%02d", h, m);
        return string(buf);
    };
    status.gateTime = calcTime(concert.startTime, 1, 0); // gate = start - 1 hour
    status.entryTime = calcTime(concert.startTime, 0, 30); // entry = start - 30 min
    status.startTime = concert.startTime;
    status.endTime = concert.endTime;
    status.date = concert.date;

    saveEventStatus(status);
}

// Get current status (consider manual settings)
string getStatusWithOverride(const EventStatus &status) {
    if (!status.manualStatus.empty()) {
        return status.manualStatus;
    }

    // Use auto status logic
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    char buffer[6];
    sprintf_s(buffer, "%02d:%02d", local.tm_hour, local.tm_min);
    string currentTime(buffer);

    auto toMinutes = [](const string &t) {
        int h, m;
        char colon;
        stringstream ss(t);
        ss >> h >> colon >> m;
        return h * 60 + m;
    };

    int cur = toMinutes(currentTime);
    int gate = toMinutes(status.gateTime);
    int entry = toMinutes(status.entryTime);
    int start = toMinutes(status.startTime);
    int end = toMinutes(status.endTime);

    if (cur < gate) return "Upcoming (Auto)";
    else if (cur < entry) return "Gate Open (Auto)";
    else if (cur < start) return "Entry In Progress (Auto)";
    else if (cur < end) return "Ongoing (Auto)";
    else return "Completed (Auto)";
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    char buffer[20];
    sprintf_s(buffer, "%02d-%02d-%04d %02d:%02d",
              local.tm_mday, local.tm_mon + 1, local.tm_year + 1900,
              local.tm_hour, local.tm_min);
    return string(buffer);
}

void displayStatusUpdateMenu(EventStatus &status, const Concert &concert) {
    int choice;

    while (true) {
        clearScreen();
        cout << "========== UPDATE EVENT STATUS ==========\n";
        cout << "Event: " << concert.concertName << "\n";
        cout << "Current Status: " << getStatusWithOverride(status) << "\n\n";

        cout << "Set status to:\n";
        cout << " 1) Cancelled\n";
        cout << " 2) Postponed\n";
        cout << " 3) Upcoming (manual)\n";
        cout << " 4) Gate Open (manual)\n";
        cout << " 5) Entry In Progress (manual)\n";
        cout << " 6) Ongoing (manual)\n";
        cout << " 7) Clear manual override (use Auto)\n";
        cout << " 8) Update Times\n";
        cout << " 9) Back\n";
        cout << "Enter choice: ";

        if (cin >> choice) {
            switch (choice) {
                case 1:
                    status.manualStatus = "Cancelled";
                    saveEventStatus(status);
                    break;

                case 2:
                    status.manualStatus = "Postponed";
                    saveEventStatus(status);
                    break;

                case 3:
                    status.manualStatus = "Upcoming (Manual)";
                    saveEventStatus(status);
                    break;

                case 4:
                    status.manualStatus = "Gate Open (Manual)";
                    {
                        time_t now = time(0);
                        tm local;
                        localtime_s(&local, &now);

                        char timeBuffer[6];
                        sprintf_s(timeBuffer, "%02d:%02d", local.tm_hour, local.tm_min);
                        status.gateTime = string(timeBuffer);

                        char dateBuffer[11];
                        sprintf_s(dateBuffer, "%02d-%02d-%04d",
                                  local.tm_mday, local.tm_mon + 1, local.tm_year + 1900);
                        status.date = string(dateBuffer);
                    }
                    saveEventStatus(status);
                    break;

                case 5:
                    status.manualStatus = "Entry In Progress (Manual)";
                    {
                        time_t now = time(0);
                        tm local;
                        localtime_s(&local, &now);

                        char timeBuffer[6];
                        sprintf_s(timeBuffer, "%02d:%02d", local.tm_hour, local.tm_min);
                        status.entryTime = string(timeBuffer);

                        char dateBuffer[11];
                        sprintf_s(dateBuffer, "%02d-%02d-%04d",
                                  local.tm_mday, local.tm_mon + 1, local.tm_year + 1900);
                        status.date = string(dateBuffer);
                    }
                    saveEventStatus(status);
                    break;

                case 6:
                    status.manualStatus = "Ongoing (Manual)";
                    {
                        time_t now = time(0);
                        tm local;
                        localtime_s(&local, &now);

                        char timeBuffer[6];
                        sprintf_s(timeBuffer, "%02d:%02d", local.tm_hour, local.tm_min);
                        status.startTime = string(timeBuffer);

                        char dateBuffer[11];
                        sprintf_s(dateBuffer, "%02d-%02d-%04d",
                                  local.tm_mday, local.tm_mon + 1, local.tm_year + 1900);
                        status.date = string(dateBuffer);
                    }
                    saveEventStatus(status);
                    break;

                case 7:
                    status.manualStatus = "";
                    updateEventTimes(status, concert);
                    break;

                case 8: {
                    cout << "\nCurrent Times:\n";
                    cout << "Gate Time : " << status.gateTime << "\n";
                    cout << "Entry Time: " << status.entryTime << "\n";
                    cout << "Start Time: " << status.startTime << "\n";
                    cout << "End Time  : " << status.endTime << "\n\n";

                    cout << "Which time to update?\n";
                    cout << "1) Gate Time\n";
                    cout << "2) Entry Time\n";
                    cout << "3) Start Time\n";
                    cout << "4) End Time\n";
                    cout << "5) Cancel\n";
                    cout << "Enter choice: ";

                    int timeChoice;
                    if (cin >> timeChoice) {
                        cin.ignore();

                        if (timeChoice >= 1 && timeChoice <= 4) {
                            string newTime;
                            cout << "Enter new time (HH:MM): ";
                            getline(cin, newTime);

                            if (timeValid(newTime)) {
                                switch (timeChoice) {
                                    case 1: status.gateTime = newTime;
                                        break;
                                    case 2: status.entryTime = newTime;
                                        break;
                                    case 3: status.startTime = newTime;
                                        break;
                                    case 4: status.endTime = newTime;
                                        break;
                                }
                                saveEventStatus(status);
                            }
                        } else if (timeChoice != 5) {
                            cout << "Invalid choice! Please enter 1-5.\n";
                            Sleep(1000);
                        }
                    } else {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input! Please enter a number.\n";
                        Sleep(1000);
                    }
                }
                break;

                case 9:
                    displayMonitoring(concert);
                    return;

                default:
                    cout << "Invalid choice! Please enter 1-9.";
                    break;
            }
        } else {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.";
        }
    }
}

void displayMonitoring(const Concert &c) {
    clearScreen();

    // Load or create event status
    EventStatus status;
    loadEventStatus(c.concertName, status);

    string statusFile = "schedule_" + c.concertName + ".txt";
    replace(statusFile.begin(), statusFile.end(), ' ', '_');

    ifstream checkFile(statusFile);
    bool fileExists = checkFile.good();
    checkFile.close();

    if (!fileExists || status.gateTime.empty() || status.entryTime.empty()) {
        updateEventTimes(status, c);
    }

    // Load issues
    vector<string> issues;
    string issueFile = "issues_" + c.concertName + ".txt";
    replace(issueFile.begin(), issueFile.end(), ' ', '_');
    ifstream fin(issueFile);
    if (fin) {
        stringstream buffer;
        buffer << fin.rdbuf();
        fin.close();

        string line;
        while (getline(buffer, line)) {
            if (!line.empty()) {
                issues.push_back(line);
            }
        }
    }

    int choice;
    while (true) {
        // Display details
        cout << "========== EVENT MONITORING ==========\n";
        cout << left << setw(8) << "Concert: " << c.concertName << endl;
        cout << left << setw(8) << "Artist : " << c.artist << endl;
        cout << left << setw(8) << "Venue  : " << c.venue << endl;
        cout << left << setw(8) << "Date" << ": " << status.date << endl;
        cout << left << setw(8) << "Gate" << ": " << status.gateTime << endl;
        cout << left << setw(8) << "Entry" << ": " << status.entryTime << endl;
        cout << left << setw(8) << "Start" << ": " << status.startTime << endl;
        cout << left << setw(8) << "End" << ": " << status.endTime << endl;
        cout << left << setw(8) << "Status : " << getStatusWithOverride(status) << endl;

        cout << "Issues : " << endl;
        if (issues.empty()) {
            cout << "   (no issues yet)\n";
        } else {
            for (const string &s: issues) {
                cout << "   - " << s << endl;
            }
        }

        cout << "=====================================================\n";
        cout << "Options:\n";
        cout << "  1. Update Event Status\n";
        cout << "  2. View/Add Organiser Comments\n";
        cout << "  3. Back to Admin Menu\n";
        cout << "Enter choice: ";

        if (cin >> choice) {
            switch (choice) {
                case 1:
                    displayStatusUpdateMenu(status, c);
                    return;

                case 2:
                    fileComplaint("admin", c.concertName, "Organiser");
                    displayMonitoring(c);
                    return;

                case 3:
                    clearScreen();
                    return;

                default:
                    clearScreen();
                    cout << "Invalid choice! Please enter 1, 2, or 3.\n";
                    break;
            }
        } else {
            clearScreen();
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

vector<Booking> loadUserBookings(const string &userName) {
    vector<Booking> userBookings;
    ifstream file("purchase_history.txt");
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string user, event, date, seats, priceStr, payment;

        getline(ss, user, ';');
        getline(ss, event, ';');
        getline(ss, date, ';');
        getline(ss, seats, ';');
        getline(ss, priceStr, ';');
        getline(ss, payment, ';');

        if (user == userName) {
            Booking booking;
            booking.userName = user;
            booking.eventName = event;
            booking.eventDate = date;
            booking.seats = seats;
            booking.totalPrice = stod(priceStr);
            booking.paymentMethod = payment;
            booking.bookingDateTime = getCurrentDateTime();
            booking.eventStatus = determineEventStatus(date);
            userBookings.push_back(booking);
        }
    }
    file.close();
    return userBookings;
}

// Determine if event is upcoming or past
string determineEventStatus(const string &eventDate) {
    // dd-mm-yyyy format
    int day, month, year;
    char dash1, dash2;
    stringstream ss(eventDate);
    ss >> day >> dash1 >> month >> dash2 >> year;

    // Get current date and time
    time_t now = time(0);
    tm current = *localtime(&now);
    int currentDay = current.tm_mday;
    int currentMonth = current.tm_mon + 1;
    int currentYear = current.tm_year + 1900;
    int currentHour = current.tm_hour;
    int currentMin = current.tm_min;

    // Compare dates
    if (year > currentYear) return "Upcoming";
    if (year == currentYear && month > currentMonth) return "Upcoming";
    if (year == currentYear && month == currentMonth && day > currentDay) return "Upcoming";


    if (year == currentYear && month == currentMonth && day == currentDay) {
        // Load concerts to find the event time for today's events
        vector<Concert> concerts = loadConcerts("events.txt");
        for (const Concert &c: concerts) {
            if (c.date == eventDate) {
                // Parse event end time to determine if event is over
                int eventHour, eventMin;
                char colon;
                stringstream timeStream(c.endTime);
                timeStream >> eventHour >> colon >> eventMin;

                // Convert to minutes for easier comparison
                int currentTimeInMin = currentHour * 60 + currentMin;
                int eventEndTimeInMin = eventHour * 60 + eventMin;

                // If current time is after event end time, it's past
                if (currentTimeInMin > eventEndTimeInMin) {
                    return "Past";
                } else {
                    return "Upcoming";
                }
            }
        }
        // If event not found in concerts list, default to upcoming for same day
        return "Upcoming";
    }

    return "Past";
}

// Sort bookings by event date in descending order
void sortBookingsByDate(vector<Booking> &bookings) {
    sort(bookings.begin(), bookings.end(), [](const Booking &a, const Booking &b) {
        // Parse dates and compare
        int dayA, monthA, yearA, dayB, monthB, yearB;
        char dash;

        stringstream ssA(a.eventDate), ssB(b.eventDate);
        ssA >> dayA >> dash >> monthA >> dash >> yearA;
        ssB >> dayB >> dash >> monthB >> dash >> yearB;

        if (yearA != yearB) return yearA > yearB;
        if (monthA != monthB) return monthA > monthB;
        return dayA > dayB;
    });
}

// Search bookings by keyword
void searchBookings(const vector<Booking> &bookings, const string &userName) {
    if (bookings.empty()) {
        cout << "No bookings found to search.\n";
        Sleep(1000);
        return;
    }

    string keyword;
    cin.ignore();
    cout << "Enter search keyword (event name or venue): ";
    getline(cin, keyword);

    // Convert keyword to lowercase for case-insensitive search
    transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

    vector<Booking> searchResults;
    for (const Booking &booking: bookings) {
        Concert concert = getBookingConcertDetails(booking.eventName);

        // Convert event name and venue to lowercase for comparison
        string eventLower = booking.eventName;
        transform(eventLower.begin(), eventLower.end(), eventLower.begin(), ::tolower);

        string venueLower = concert.venue;
        transform(venueLower.begin(), venueLower.end(), venueLower.begin(), ::tolower);

        // Search in both event name and venue
        if (eventLower.find(keyword) != string::npos ||
            venueLower.find(keyword) != string::npos) {
            searchResults.push_back(booking);
        }
    }


    if (searchResults.empty()) {
        cout << "No bookings found matching '" << keyword << "'.\n";
        Sleep(1000);
        return;
    }

    clearScreen();
    cout << "=== SEARCH RESULTS ===\n";
    for (size_t i = 0; i < searchResults.size(); i++) {
        Concert concert = getBookingConcertDetails(searchResults[i].eventName);
        cout << (i + 1) << ". Concert: " << searchResults[i].eventName
                << " (Date: " << searchResults[i].eventDate << ")\n";
        cout << "   Venue: " << (concert.venue.empty() ? "N/A" : concert.venue) << "\n";
        cout << searchResults[i].eventStatus << "\n";
        cout << "--------------------------\n";
    }

    cout << "Options:\n";
    cout << "Your can enter number for detail\n";
    cout << "Back to Booking History = \"B\"\n";
    cout << "Enter Choice: ";

    string choice;
    cin >> choice;

    if (toLowerSTR(choice) == "b") {
        return;
    }

    try {
        int index = stoi(choice);
        if (index >= 1 && index <= (int) searchResults.size()) {
            displayBookingDetail(searchResults[index - 1], userName);
        } else {
            cout << "Invalid choice!\n";
            Sleep(1000);
        }
    } catch (...) {
        cout << "Invalid input!\n";
        Sleep(1000);
    }
}

// Get concert details for a booking
Concert getBookingConcertDetails(const string &eventName) {
    vector<Concert> concerts = loadConcerts("events.txt");
    for (const Concert &c: concerts) {
        if (c.concertName == eventName) {
            return c;
        }
    }
    // Return empty concert if not found
    return {"", "", "", "", "", ""};
}

// Display detailed booking information
void displayBookingDetail(const Booking &booking, const string &userName) {
    clearScreen();

    // Get concert details
    Concert concert = getBookingConcertDetails(booking.eventName);

    int choice;

    while (true) {
        cout << "=== BOOKING DETAILS ===\n";
        cout << "Concert: " << booking.eventName << "\n";
        cout << "Artist : " << (concert.artist.empty() ? "N/A" : concert.artist) << "\n";
        cout << "Venue  : " << (concert.venue.empty() ? "N/A" : concert.venue) << "\n";
        cout << "Date   : " << booking.eventDate << "\n";
        cout << "Start  : " << (concert.startTime.empty() ? "N/A" : concert.startTime) << "\n";
        cout << "End    : " << (concert.endTime.empty() ? "N/A" : concert.endTime) << "\n";
        cout << "Seats  : " << booking.seats << "\n";
        cout << "Total Paid : RM " << booking.totalPrice << "\n";
        cout << "Payment Method: " << booking.paymentMethod << "\n";
        cout << "Status : " << booking.eventStatus << "\n";
        cout << "=====================================================\n";

        cout << "Options:\n";
        cout << "  1. Feedback/Complain\n";
        cout << "  2. Back to Booking History\n";
        cout << "Enter Choice: ";

        if (cin >> choice) {
            switch (choice) {
                case 1:
                    fileComplaint(userName, booking.eventName, "User");
                    return;
                case 2:
                    return;
                default:
                    clearScreen();
                    cout << "Invalid choice! Please enter 1 or 2.\n";
                    break;
            }
        } else {
            clearScreen();
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

void viewBookingHistory(const string &userName) {
    while (true) {
        clearScreen();
        vector<Booking> bookings = loadUserBookings(userName);

        if (bookings.empty()) {
            cout << "=== BOOKING HISTORY ===\n";
            cout << "No booking history found for " << userName << ".\n";
            cout << "Press Enter to return to menu...";
            cin.ignore();
            cin.get();
            return;
        }

        // Sort bookings by date (most recent first)
        sortBookingsByDate(bookings);

        cout << "=== BOOKING HISTORY ===\n";
        for (size_t i = 0; i < bookings.size(); i++) {
            cout << (i + 1) << ". Concert: " << bookings[i].eventName
                    << " (Date: " << bookings[i].eventDate << ")\n";
            cout << bookings[i].eventStatus << "\n";
            cout << "--------------------------\n";
        }

        cout << "Options:\n";
        cout << "Your can enter number for detail\n";
        cout << "Search Bookings = \"S\"\n";
        cout << "Back to User Menu = \"B\"\n";
        cout << "Enter Choice: ";

        string choice;
        cin >> choice;

        if (toLowerSTR(choice) == "b") {
            clearScreen();
            return;
        } else if (toLowerSTR(choice) == "s") {
            searchBookings(bookings, userName);
        } else {
            try {
                int index = stoi(choice);
                if (index >= 1 && index <= (int) bookings.size()) {
                    displayBookingDetail(bookings[index - 1], userName);
                } else {
                    cout << "Invalid choice!\n";
                    Sleep(1000);
                }
            } catch (...) {
                cout << "Invalid input!\n";
                Sleep(1000);
            }
        }
    }
}

// Unified complaint/comment function for both users and admins
void fileComplaint(const string &userName, const string &eventName, const string &userType) {
    clearScreen();

    if (userType == "User") {
        cout << "=== FILE COMPLAINT/FEEDBACK ===\n";
    } else {
        cout << "=== ADD ORGANISER COMMENT ===\n";
    }

    cout << "Event: " << eventName << endl;
    cout << userType << ": " << userName << endl << endl;

    string complaint;
    cin.ignore();

    if (userType == "User") {
        cout << "Enter your complaint/feedback (or type 'cancel' to cancel):\n";
    } else {
        cout << "Enter your comment/note (or type 'cancel' to cancel):\n";
    }

    getline(cin, complaint);

    if (toLowerSTR(complaint) == "cancel") {
        if (userType == "User") {
            cout << "Complaint cancelled.\n";
        } else {
            cout << "Comment cancelled.\n";
        }
        Sleep(1000);
        return;
    }

    if (complaint.empty()) {
        if (userType == "User") {
            cout << "Complaint cannot be empty!\n";
        } else {
            cout << "Comment cannot be empty!\n";
        }
        Sleep(1000);
        return;
    }

    // Save to issues file with appropriate prefix
    string issueFile = "issues_" + eventName + ".txt";
    replace(issueFile.begin(), issueFile.end(), ' ', '_');

    ofstream file(issueFile, ios::app);
    if (file.is_open()) {
        string timestamp = getCurrentDateTime();

        if (userType == "User") {
            file << "[User: " << userName << "] [" << timestamp << "] " << complaint << "\n";
        } else {
            file << "[Organiser: " << userName << "] [" << timestamp << "] " << complaint << "\n";
        }

        file.close();

        if (userType == "User") {
            cout << "\nComplaint/Feedback submitted successfully!\n";
            cout << "Your feedback has been recorded and will be reviewed by the event organizers.\n";
        } else {
            cout << "\nComment added successfully!\n";
            cout << "Your organiser comment has been recorded.\n";
        }

        cout << "Press Enter to continue...";
        cin.get();
    } else {
        cout << "Error: Could not save. Please try again later.\n";
        Sleep(3000);
    }
}