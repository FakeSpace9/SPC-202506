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
#include <fstream>
#include <limits>
#include <ctype.h>
#include <ctime>

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
    string time;
};

vector<Concert> loadConcerts(const string &filename);

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

void eventRegistration(const string &userName);

bool checkoutAndPayment(const string &userName, const Concert &concert, const vector<Seat> &selectedSeats,
                        double totalPrice);

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
        cout << "Enter Password: ";
        cin >> newUser.password;
        cout << "Confirm Password: ";
        cin >> confirmPassword;

        if (newUser.password != confirmPassword) {
            cout << "Passwords do not match. Try again.\n";
        } else if (newUser.password.length() < 8) {
            cout << "Password must be at least 8 characters long.\n";
        } else {
            break;
        }
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

void userLogin() {
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

    if (!readFile) {
        cout << "File cannot be open.";
        return;
    }
    string line;

    cout << "User Login\n";

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
        cin >> password;


        if (toLowerSTR(userName) == "admin" && password == "admin") {
            adminMenu(userName);
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

void adminMenu(string userName) {
    clearScreen();
    int choice;
    while (true) {
        cout << "Welcome " << userName << "!" << endl;
        cout << "1. Event Creation\n2. Event Monitoring\n3. Event Reporting\n4. Logout";
        cout << "\nEnter your choice :";
        cin >> choice;
        switch (choice) {
            case 1:
                createEventSeats();
                break;
            case 2:
                cout << "\nEvent Monitoring is not implemented yet.\n";
                break;
            case 3:
                cout << "\nEvent Monitoring is not implemented yet.\n";
                break;
            case 4:
                clearScreen();
                cout << "Logout Successful";
                for (int i = 0; i < 3; i++) {
                    cout << ".";
                    cout.flush();
                    Sleep(700);
                }
                mainMenu();
                break;
            default:
                clearScreen();
                cout << "Invalid option. Try again.\n";
        }
    }
}

void userMenu(string userName) {
    int choice;
    while (true) {
        cout << "\nWelcome " << userName << "!" << endl;
        cout << "1. Event Register\n2. Profile\n3. Logout";
        // profile is to view past ticket also with update email and password
        cout << "\nEnter your choice :";
        cin >> choice;
        switch (choice) {
            case 1:
                eventRegistration(userName);
                break;
            case 2:
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
                break;
            default:
                clearScreen();
                cout << "Invalid option. Try again.";
                choice = 0;
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
    cout << "Time:   " << c.time << "\n";
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

// event registration function
void eventRegistration(const string &userName) {
    size_t consumed = 0;
    int number;
    vector<Concert> concerts = loadConcerts("events.txt");
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
                << " on " << concerts[i].date << " " << concerts[i].time << "\n";
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
    if (choice == 0) return; // back to previous menu
    if (choice < 1 || choice > (int) concerts.size()) {
        cout << "Invalid choice.\n";
        return;
    }

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
        if (seatLower == "back")
            return;
        if (seatLower == "done")
            break;
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
    if (checkoutAndPayment(userName, selected, selectedSeats, totalPrice)) {
        saveSeats(seatFileName, seats);
        cout << "Booking confirmed!\n";
        cout << "Returning to user menu in 3 second\n";
        for (int i = 0; i < 3; i++) {
            cout << ".";
            cout.flush();
            Sleep(700);
        }

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
                << "3. E‑Wallet\n"
                << "Enter your choice: ";

        int method;
        if (!(cin >> method) || method < 0 || method > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                cout << "Enter e‑wallet ID/number (or type 'back'): ";
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