#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <unordered_map>

using namespace std;

const int MAX_OWNERS = 10;

struct MenuItem {
    int code;
    string name;
    double price;
    int quantity;
    MenuItem* next;
};

struct Owner {
    string username;
    string password;
    string restaurantName;
    MenuItem* menuHead;
};

struct CartItem {
    string restaurant;
    string name;
    double price;
    int quantity;
    int code;
};

struct PaymentRecord {
    string orderId;
    string customer;
    string restaurant;
    double totalAmount;
    double adminAmount;
    double ownerAmount;
    string date;
};



struct DeliveryPoint {
    string name;
    double price;
    int timeMinutes; // delivery time in minutes
};


vector<CartItem> cart;

Owner owners[MAX_OWNERS];
int uniqueCode = 100;


void addMenuItem(MenuItem* &head, string name, double price, int qty) {
    MenuItem* newItem = new MenuItem{uniqueCode++, name, price, qty, nullptr};
    if (head==nullptr) head = newItem;
    else {
        MenuItem* temp = head;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = newItem;
    }
}

void deleteMenuItem(MenuItem* &head, string itemName) {
    MenuItem* temp = head, *prev = nullptr;
    while (temp!= nullptr) {
        if (temp->name == itemName) {
            if (prev == nullptr) head = temp->next;
            else prev->next = temp->next;
            delete temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void showMenu(string restName) {
      HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,14);
        cout << "\n==============================================\n";
    cout << "\n        Menu of " << restName << "        \n";
        cout << "\n==============================================\n";
    for (int i = 0; i < MAX_OWNERS; i++) {
  SetConsoleTextAttribute(h,11);
        if (owners[i].restaurantName == restName) {
            MenuItem* temp = owners[i].menuHead;
            cout << left << setw(6)  << "Code"
                 << left << setw(20) << "Name"
                 << right << setw(12) << "Price"
                 << right << setw(8)  << "Qty" << "\n";
            cout << string(6+20+12+8, '-') << "\n";

            while (temp != nullptr) {
                string priceStr = to_string(temp->price);
                priceStr = priceStr.substr(0, priceStr.find(".") + 3) + " TK";
         SetConsoleTextAttribute(h, 15);
                cout << left << setw(6)  << temp->code
                     << left << setw(20) << temp->name
                     << right << setw(12) << priceStr
                     << right << setw(8)  << temp->quantity << "\n";

                temp = temp->next;
            }
SetConsoleTextAttribute(h,11);
            cout << string(6+20+12+8, '-') << "\n";
            return;
        }
    }

    cout << "Restaurant not found!\n";
           SetConsoleTextAttribute(h,7);
}

void firstTimeSetup() {
    ofstream fout("owners.txt");
    fout << "ow1,ps1,KacchiBhai\n";
    fout << "ow2,ps2,ShadhinBangla\n";
    fout << "ow3,ps3,ShobujBangla\n";
    fout << "ow4,ps4,TastyTreat\n";
    fout << "ow5,ps5,SUSTCafeteria\n";
    fout << "ow6,ps6,99Meals\n";
    fout << "ow7,ps7,PanshiRestaurant\n";
    fout << "ow8,ps8,PachBhaiRestaurant\n";
    fout << "ow9,ps9,TheBurgersPoint\n";
    fout << "ow10,ps10,FryBox\n"; // extra 1 restaurant for total 5

    fout.close();


    ofstream m("menu.txt");

    // ----- Kacchi Bhai (5 original + 5 new) -----
    m << "KacchiBhai,100,Kacchi Biriyani,250,20\n";
    m << "KacchiBhai,101,Tehari,150,25\n";
    m << "KacchiBhai,102,Beef Kala Bhuna,280,15\n";
    m << "KacchiBhai,103,Chicken Roast,120,10\n";
    m << "KacchiBhai,104,Borhani,60,40\n";

    m << "KacchiBhai,105,Mutton Bhuna,350,18\n";
    m << "KacchiBhai,106,Firni,40,50\n";
    m << "KacchiBhai,107,Beef Leg Roast,320,10\n";
    m << "KacchiBhai,108,Kala Vuna Special,300,12\n";
    m << "KacchiBhai,109,Soft Drinks,25,70\n";

    // ----- Shadhin Bangla -----
    m << "ShadhinBangla,110,Shutki Vorta,20,50\n";
    m << "ShadhinBangla,111,Chicken Curry,70,20\n";
    m << "ShadhinBangla,112,Luchi,10,50\n";
    m << "ShadhinBangla,113,Beef Rezala,270,32\n";
    m << "ShadhinBangla,114,Mishti Doi,40,40\n";

    m << "ShadhinBangla,115,Alu Vorta,15,60\n";
    m << "ShadhinBangla,116,Shingara,8,80\n";
    m << "ShadhinBangla,117,Beguni,12,45\n";
    m << "ShadhinBangla,118,Roshogolla,20,70\n";
    m << "ShadhinBangla,119,Jorda,25,55\n";

    // ----- Shobuj Bangla -----
    m << "ShobujBangla,120,Vegetable Khichuri,50,25\n";
    m << "ShobujBangla,121,Chicken Khichuri,90,30\n";
    m << "ShobujBangla,122,Dal Puri,80,30\n";
    m << "ShobujBangla,123,Tea,20,50\n";
    m << "ShobujBangla,124,Salad,10,20\n";

m << "ShobujBangla,125,Vat,60,30\n";
m << "ShobujBangla,126,Fish Curry,150,25\n";
m << "ShobujBangla,127,Vorta,25,40\n";
m << "ShobujBangla,128,Rice,40,50\n";
m << "ShobujBangla,129,Dal,30,45\n";

    // ----- Tasty Treat -----
    m << "TastyTreat,130,Chicken Burger,180,20\n";
    m << "TastyTreat,131,Beef Burger,220,18\n";
    m << "TastyTreat,132,French Fries,100,25\n";
    m << "TastyTreat,133,Chocolate Cake,350,20\n";
    m << "TastyTreat,134,Cold Coffee,130,30\n";

    m << "TastyTreat,135,Hot Coffee,90,25\n";
    m << "TastyTreat,136,Chicken Shawarma,170,18\n";
    m << "TastyTreat,137,Brownie,120,22\n";
    m << "TastyTreat,138,Chicken Cheese Wrap,160,20\n";
    m << "TastyTreat,139,Vanilla Shake,110,30\n";

    // ----- SUST Cafeteria -----
    m << "SUSTCafeteria,140,Paratha,10,50\n";
    m << "SUSTCafeteria,141,Rice,20,40\n";
    m << "SUSTCafeteria,142,Egg Curry,30,35\n";
    m << "SUSTCafeteria,143,Chicken Curry,60,25\n";
    m << "SUSTCafeteria,144,Tea,15,100\n";

    m << "SUSTCafeteria,145,Dim Puri,15,40\n";
    m << "SUSTCafeteria,146,Lemon Tea,15,60\n";
    m << "SUSTCafeteria,147,Khichuri,40,50\n";
    m << "SUSTCafeteria,148,Jilapi,8,80\n";
    m << "SUSTCafeteria,149,Mineral Water,20,100\n";

m << "99Meals,150,Beef Wrap,140,30\n";
m << "99Meals,151,Chicken Wrap,120,35\n";
m << "99Meals,152,Falafel,80,40\n";
m << "99Meals,153,Soft Drinks,25,60\n";
m << "99Meals,154,Shawarma Plate,180,20\n";
m << "99Meals,155,Cheese Pizza,200,25\n";
m << "99Meals,156,Chicken Nuggets,150,30\n";
m << "99Meals,157,French Fries,100,50\n";
m << "99Meals,158,Ice Cream,50,40\n";
m << "99Meals,159,Cola,30,60\n";

// ----- Panshi Restaurant -----
m << "PanshiRestaurant,160,Borhani,35,100\n";
m << "PanshiRestaurant,161,Kala Bhuna,300,25\n";
m << "PanshiRestaurant,162,Rice Special,40,60\n";
m << "PanshiRestaurant,163,Chicken Curry,90,40\n";
m << "PanshiRestaurant,164,Firni,35,80\n";
m << "PanshiRestaurant,165,Mutton Curry,150,30\n";
m << "PanshiRestaurant,166,Vegetable Curry,70,50\n";
m << "PanshiRestaurant,167,Fish Curry,120,25\n";
m << "PanshiRestaurant,168,Paratha,15,60\n";
m << "PanshiRestaurant,169,Lassi,50,45\n";

// ----- Pach Bhai Restaurant -----
m << "PachBhaiRestaurant,170,Mutton Curry,150,25\n";
m << "PachBhaiRestaurant,171,Beef Vuna,170,30\n";
m << "PachBhaiRestaurant,172,Paratha,12,50\n";
m << "PachBhaiRestaurant,173,Lassi,50,40\n";
m << "PachBhaiRestaurant,174,Chicken Soup,70,30\n";
m << "PachBhaiRestaurant,175,Fish Fry,200,20\n";
m << "PachBhaiRestaurant,176,Vegetable Pulao,90,35\n";
m << "PachBhaiRestaurant,177,Dal,30,50\n";
m << "PachBhaiRestaurant,178,Vorta,25,45\n";
m << "PachBhaiRestaurant,179,Biriyani,250,20\n";

// ----- The Burger's Point -----
m << "TheBurgersPoint,180,Beef Cheese Burger,250,20\n";
m << "TheBurgersPoint,181,Chicken Cheese Burger,220,22\n";
m << "TheBurgersPoint,182,Loaded Fries,180,25\n";
m << "TheBurgersPoint,183,Cold Coffee,150,30\n";
m << "TheBurgersPoint,184,Choco Shake,140,25\n";
m << "TheBurgersPoint,185,Grilled Sandwich,120,30\n";
m << "TheBurgersPoint,186,Chicken Wrap,160,25\n";
m << "TheBurgersPoint,187,Coke,30,60\n";
m << "TheBurgersPoint,188,French Fries,100,50\n";
m << "TheBurgersPoint,189,Ice Cream,50,40\n";

// ----- Fry Box -----
m << "FryBox,190,Fried Chicken,150,30\n";
m << "FryBox,191,Mojito,80,35\n";
m << "FryBox,192,Wedges,90,40\n";
m << "FryBox,193,Chicken Popcorn,120,45\n";
m << "FryBox,194,Ice Cream,50,50\n";
m << "FryBox,195,Soft Drinks,25,60\n";
m << "FryBox,196,Fish Fry,200,25\n";
m << "FryBox,197,Chicken Roll,150,35\n";
m << "FryBox,198,Cheese Sticks,100,40\n";
m << "FryBox,199,Burger Combo,250,20\n";

m.close();
}

void loadOwnersFromFile() {
    ifstream fin("owners.txt");
    string line; int idx = 0;
    while (getline(fin, line) && idx < MAX_OWNERS) {
        stringstream ss(line);
        string user, pass, rest;
        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, rest, ',');
        owners[idx] = {user, pass, rest, nullptr};
        idx++;
    }
    fin.close();
}


void loadMenuFromFile() {
    ifstream fin("menu.txt");
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string restname, item;
        int code, qty;
        double price;

        getline(ss, restname, ',');
        ss >> code; ss.ignore();
        getline(ss, item, ',');
        ss >> price; ss.ignore();
        ss >> qty;

        for (int i = 0; i < MAX_OWNERS; i++) {
            if (owners[i].restaurantName == restname) {
                MenuItem* newItem = new MenuItem{code, item, price, qty, nullptr};
                if (owners[i].menuHead == nullptr) owners[i].menuHead = newItem;
                else {
                    MenuItem* temp = owners[i].menuHead;
                    while (temp->next!=nullptr) temp = temp->next;
                    temp->next = newItem;
                }
                if (code >= uniqueCode) uniqueCode = code + 1;
                break;
            }
        }
    }
    fin.close();
}

void saveMenuToFile() {
    ofstream fout("menu.txt");
    for (int i = 0; i < MAX_OWNERS; i++) {
        MenuItem* temp = owners[i].menuHead;
        while (temp) {
            fout << owners[i].restaurantName << ","
                 << temp->code << ","
                 << temp->name << ","
                 << temp->price << ","
                 << temp->quantity << "\n";
            temp = temp->next;
        }
    }
    fout.close();
}

void addRequest(string restname, string action, string item, double price, int qty, double discount) {
    ofstream fout("requests.txt", ios::app);

    // Save in readable block format
    fout << "[Restaurant: " << restname << "]\n";
    fout << "Action   : " << action << "\n";
    fout << "Item     : " << item << "\n";
    fout << "Price    : " << fixed << setprecision(2) << price << "\n";
    fout << "Quantity : " << qty << "\n";
    fout << "Discount : " << discount << "%\n";
    fout << "Status   : PENDING\n";
    fout << "---------------------------------------------\n";

    fout.close();
}


void printRequestBlock(string restName, int code, string action, string item,
                       double price, int qty, double discount, string status)
{
    cout << "[Restaurant: " << restName << "]\n";
    if(code != 0) cout << "Code     : " << code << "\n";
    cout << "Action   : " << action << "\n";
    cout << "Item     : " << item << "\n";
    cout << "Price    : " << fixed << setprecision(2) << price << "\n";
    cout << "Quantity : " << qty << "\n";
    cout << "Discount : " << discount << "%\n";
    cout << "Status   : " << status << "\n";
    cout << "---------------------------------------------\n";
}



void saveRequest(string rest, int code, string action, string item,
                 double price, int qty, double discount, string status) {

    ofstream fout("request_history.txt", ios::app);

    fout << "[Restaurant: " << rest << "]\n";
    fout << "Code     : " << code << "\n";
    fout << "Action   : " << action << "\n";
    fout << "Item     : " << item << "\n";
    fout << "Price    : " << fixed << setprecision(2) << price << "\n";
    fout << "Quantity : " << qty << "\n";
    fout << "Discount : " << discount << "%\n";
    fout << "Status   : " << status << "\n";
    fout << "---------------------------------------------\n";

    fout.close();

    // Console print
    printRequestBlock(rest, code, action, item, price, qty, discount, status);
}


bool getItem(const string& restaurant, int code, string& name, double& price, int& qty) {
    ifstream fin("menu.txt");
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string rest, itemName;
        int c, quantity;
        double itemPrice;

        getline(ss, rest, ',');
        ss >> c;
        ss.ignore();
        getline(ss, itemName, ',');
        ss >> itemPrice;
        ss.ignore();
        ss >> quantity;

        if (rest == restaurant && c == code) {
            name = itemName;
            price = itemPrice;
            qty = quantity;
            return true;    // Found with full data
        }


    }
    return false; // Not found
}
void viewRequestHistory(string restName) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    ifstream fin("request_history.txt");
    if (!fin) {
        cout << "No request history found!\n";
        return;
    }

    SetConsoleTextAttribute(h, 14);
    cout << "\n==============================================\n";
    cout << "\n      Request History for " << restName << "      \n";
    cout << "\n==============================================\n";

    SetConsoleTextAttribute(h, 11);
    cout << left << setw(5)  << "No."
         << left << setw(12) << "Action"
         << left << setw(25) << "Item"
         << right << setw(10) << "Price"
         << right << setw(8)  << "Qty"
         << right << setw(10) << "Disc"
         << left << setw(12) << "   Status" << "\n";

    cout << string(5+12+25+10+8+10+12, '-') << "\n";

    string line;
    int count = 0;

    while (getline(fin, line)) {

        // Skip lines that do not start with restaurant block
        if (line.find("[Restaurant:") == string::npos)
            continue;

        // Extract restaurant name
        string rest = line.substr(line.find(":") + 2, line.find("]") - line.find(":") - 2);

        string codeLine, actionLine, itemLine, priceLine, qtyLine, discLine, statusLine;

        getline(fin, codeLine);
        getline(fin, actionLine);
        getline(fin, itemLine);
        getline(fin, priceLine);
        getline(fin, qtyLine);
        getline(fin, discLine);
        getline(fin, statusLine);

        // separator line
        getline(fin, line);

        // Extract values
        string action = actionLine.substr(actionLine.find(":") + 2);
        string item   = itemLine.substr(itemLine.find(":") + 2);
        double price  = stod(priceLine.substr(priceLine.find(":") + 2));
        int qty       = stoi(qtyLine.substr(qtyLine.find(":") + 2));

        string discStr = discLine.substr(discLine.find(":") + 2);
        discStr.erase(remove(discStr.begin(), discStr.end(), '%'), discStr.end());
        double discount = stod(discStr);

        string status = statusLine.substr(statusLine.find(":") + 2);

        // Filter by restaurant
        if (rest != restName) continue;

        count++;

        // Format discount with 2 decimal digits
        string discountStr = "-";
        if (discount != 0) {
            ostringstream dd;
            dd << fixed << setprecision(2) << discount;
            discountStr = dd.str() + "%";
        }

        // Print row
        SetConsoleTextAttribute(h, 15);
        cout << left << setw(5)  << count
             << left << setw(12) << action
             << left << setw(25) << item
             << right << setw(10) << fixed << setprecision(2) << price
             << right << setw(8)  << qty
             << right << setw(10) << discountStr
             << "  " << left << setw(12) << status << "\n";
    }

    if (count == 0) {
        cout << "No requests found for your restaurant.\n";
    }

    SetConsoleTextAttribute(h,11);
    cout << string(5+12+25+10+8+10+12, '-') << "\n";
    SetConsoleTextAttribute(h, 7);

    fin.close();
}

void requestDiscount(Owner &owner) {
     HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    int code;
    double discount;

    cout << "Enter the item code to apply discount: ";
    cin >> code;

    string name;
    double price;
    int qty;

    // Check if item exists using your getItem() function
    if (!getItem(owner.restaurantName, code, name, price, qty)) {
        cout << "Item code not found in your menu!\n";
        return;
    }

    cout << "Enter discount percentage: ";
    cin >> discount;

    if (discount <= 0 || discount >= 100) {
        cout << "Invalid discount percentage!\n";
        return;
    }

    // Save discount request (full data)
    addRequest(owner.restaurantName, "DISCOUNT", name, code, qty, discount);
   SetConsoleTextAttribute(h,3);
    cout << "Discount request submitted for approval.\n";
     SetConsoleTextAttribute(h,7);
}
void accept_proposal() {

    ifstream fin("requests.txt");
    if (!fin) {
        cout << "No proposals found.\n";
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    string line;
    vector<string> remaining;
    int reqCount = 0;

    while (getline(fin, line)) {

        if (line.find("[Restaurant:") == string::npos)
            continue;

        reqCount++;

        // -------------------------
        // Parse the block
        // -------------------------
        string rest, action, item;
        double price = 0, discount = 0;
        int qty = 0, code = 0;

        // Restaurant
        rest = line.substr(line.find(":") + 2, line.find("]") - line.find(":") - 2);

        // Action
        getline(fin, line);
        action = line.substr(line.find(":") + 2);

        // Item
        getline(fin, line);
        item = line.substr(line.find(":") + 2);

        // Price
        getline(fin, line);
        price = stod(line.substr(line.find(":") + 2));

        // Quantity
        getline(fin, line);
        qty = stoi(line.substr(line.find(":") + 2));

        // Discount
        getline(fin, line);
        discount = stod(line.substr(line.find(":") + 2));

        // Status line
        getline(fin, line);
        // Separator line
        getline(fin, line);

        // -------------------------
        // SHOW PARSED REQUEST
        // -------------------------
        cout << "\n===== Pending Request #" << reqCount << " =====\n";

        SetConsoleTextAttribute(hConsole, 14);
        printRequestBlock(rest, code, action, item, price, qty, discount, "PENDING");
        SetConsoleTextAttribute(hConsole, 7);

        // -------------------------
        // Approval option
        // -------------------------
        int choice;
        cout << "Approve this request? (1=Yes / 0=No): ";
        cin >> choice;

        string finalStatus = (choice == 1 ? "ACCEPTED" : "REJECTED");

        saveRequest(rest, code, action, item, price, qty, discount, finalStatus);

        if (choice == 1) {
            // Apply menu change
            for (int i = 0; i < MAX_OWNERS; i++) {

                if (owners[i].restaurantName == rest) {

                    if (action == "ADD") {
                        MenuItem* newItem = new MenuItem{ uniqueCode++, item, price, qty, nullptr };

                        if (owners[i].menuHead == nullptr)
                            owners[i].menuHead = newItem;
                        else {
                            MenuItem* temp = owners[i].menuHead;
                            while (temp->next) temp = temp->next;
                            temp->next = newItem;
                        }
                    }

                    else if (action == "DELETE") {
                        deleteMenuItem(owners[i].menuHead, item);
                    }

                    else if (action == "EDIT") {
                        deleteMenuItem(owners[i].menuHead, item);
                        MenuItem* newItem = new MenuItem{ uniqueCode++, item, price, qty, nullptr };
                        if (owners[i].menuHead == nullptr)
                            owners[i].menuHead = newItem;
                        else {
                            MenuItem* temp = owners[i].menuHead;
                            while (temp->next) temp = temp->next;
                            temp->next = newItem;
                        }
                    }

                    else if (action == "DISCOUNT") {
                        MenuItem* temp = owners[i].menuHead;
                        while (temp) {
                            if (temp->name == item) {
                                temp->price = temp->price * (1 - discount / 100.0);
                                break;
                            }
                            temp = temp->next;
                        }
                    }
                }
            }
        }
        else {
            // Keep unapproved request
            // Re-save full block text
            remaining.push_back("[Restaurant: " + rest + "]");
            remaining.push_back("Action   : " + action);
            remaining.push_back("Item     : " + item);
            remaining.push_back("Price    : " + to_string(price));
            remaining.push_back("Quantity : " + to_string(qty));
            remaining.push_back("Discount : " + to_string(discount));
            remaining.push_back("Status   : PENDING");
            remaining.push_back("---------------------------------------------");
        }
    }

    fin.close();

    // Write remaining requests
    ofstream fout("requests.txt", ios::trunc);
    for (auto& r : remaining) fout << r << "\n";
    fout.close();

    saveMenuToFile();

    cout << "\nAll pending requests processed.\n";
}

// ---------------- Admin ----------------
struct Admin {
    string username;
    string password;
} adminInfo;

string gethiddenPassword() {
    string pass = "";
    char ch;

    while ((ch = _getch()) != '\r') {  // Enter চাপলে শেষ
        if (ch == '\b') {              // Backspace
            if (!pass.empty()) {
                cout << "\b \b";
                pass.pop_back();
            }
        } else {
            pass += ch;
            cout << '*';
        }
    }
    return pass;
}

bool adminLogin() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    string user, pass;

    cout << "\n===== Admin Login =====\n";

    // Username (Light Green)
    SetConsoleTextAttribute(h, 11);
    cout << "Enter username: ";
    SetConsoleTextAttribute(h, 7);
    cin >> user;

    // Password (Light Green)
    SetConsoleTextAttribute(h, 11);
    cout << "Enter password: ";
    SetConsoleTextAttribute(h, 7);
    pass = gethiddenPassword();

    cout << "\n";

    // Validation
    if (user == "nw1" && pass == "pss") {
        SetConsoleTextAttribute(h, 10);
        cout << "\nLogin successful! Welcome, Admin.\n";
        SetConsoleTextAttribute(h, 7);
        return true;
    } else {
        SetConsoleTextAttribute(h, 12);
        cout << "\nInvalid username or password.\n";
        SetConsoleTextAttribute(h, 7);
        return false;
    }
}
void showTotalOwners() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ifstream fin("owners.txt");
    if (!fin) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "Owner file not found!\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    SetConsoleTextAttribute(hConsole, 14); // Cream shade
    cout << "\n--- Registered Owners ---\n";

    string line;
    int count = 0;
    SetConsoleTextAttribute(hConsole, 15);
    while (getline(fin, line)) {
        stringstream ss(line);
        string user, pass, rest;
        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, rest, ',');

        count++;
        cout << count << ". " << user << " (" << rest << ")\n";
    }
    SetConsoleTextAttribute(hConsole, 3);
    fin.close();
    cout << "Total Owners: " << count << "\n";

    SetConsoleTextAttribute(hConsole, 7); // reset color
}

void showTotalCustomers() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ifstream fin("customers.txt");
    if (!fin) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "Customer file not found!\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    SetConsoleTextAttribute(hConsole, 6);
    cout << "\n=== Registered Customers ===\n\n";

    SetConsoleTextAttribute(hConsole, 7);

    // HEADER with elegant dotted design
    cout << "SL.. "
         << "Name........................ "
         << "Phone............... "
         << "Address................................................\n";

    cout << "------------------------------------------------------------------------------------------------------\n";

    string line;
    int count = 0;

    while (getline(fin, line)) {
        stringstream ss(line);
        string user, pass, phone, address;

        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, phone, ',');
        getline(ss, address, ',');

        count++;

        cout << left
             << setw(4) << count << " "
             << setw(26) << user << " "
             << setw(17) << phone << " "
             << setw(50) << address
             << "\n";
    }

    cout << "------------------------------------------------------------------------------------------------------\n";

    SetConsoleTextAttribute(hConsole, 6);
    cout << "Total Customers: " << count << "\n";

    SetConsoleTextAttribute(hConsole, 7);
}


void viewFoodItems() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Console handle
    cout << "\n--- All Foods ---\n";

    for (int i = 0; i < MAX_OWNERS; i++) {
        // Set red color for restaurant name header
        SetConsoleTextAttribute(hConsole, 12); // 12 = Red
        cout << "\n--- Menu of " << owners[i].restaurantName << " ---\n";

        // Reset to default color for items
        SetConsoleTextAttribute(hConsole, 7);

        showMenu(owners[i].restaurantName);
    }

    // Reset to default color at the end
    SetConsoleTextAttribute(hConsole, 7);
}



 //customer//

struct Customer {
    string username;
    string password;
    string phone;
    string gmail;     // NEW FIELD
    string address;
    bool loggedIn;
};

struct food {
    int id;
    string name;
    double price;
};

string getHiddenPassword() {
    string pass = "";
    char ch;

    while (true) {
        ch = getch();   // Safe Windows version

        if (ch == '\r') break;

        if (ch == '\b') {
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        } else {
            pass += ch;
            cout << "*";
        }
    }

    cout << endl;
    return pass;
}
Customer registerCustomer() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    Customer customer;

    // Username
    cout << "\033[93mEnter username: \033[0m";
    cin >> customer.username;

    // Password
    cout << "\033[93mEnter password: \033[0m";
    customer.password = getHiddenPassword();

    // Phone
    cout << "\033[93mEnter phone number: \033[0m";
    cin >> customer.phone;

    cin.ignore(); // clear input buffer

    // Address
    cout << "\033[93mEnter address: \033[0m";
    getline(cin, customer.address);

    // Save to file (without Gmail)
    ofstream fout("customers.txt", ios::app);
    if (!fout) {
        SetConsoleTextAttribute(h, 12);
        cout << "Error opening file!\n";
        SetConsoleTextAttribute(h, 7);
        return customer;
    }

    fout << customer.username << ","
         << customer.password << ","
         << customer.phone << ","
         << customer.address << endl;
    fout.close();

    // Confirmation
    SetConsoleTextAttribute(h, 10); // Bright Green
    cout << "\nCustomer registered successfully!\n";
    SetConsoleTextAttribute(h, 7);

    return customer;
}



void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);}
bool loginCustomer(Customer &customer) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    string uname, pass;

    setColor(11);  // light blue
    cout << "Enter username: ";
    setColor(7);
    cin >> uname;

    setColor(11);
    cout << "Enter password: ";
    setColor(7);
    pass = getHiddenPassword();

    ifstream fin("customers.txt");
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string fileUser, filePass, filePhone, fileAddress;

        getline(ss, fileUser, ',');
        getline(ss, filePass, ',');
        getline(ss, filePhone, ',');
        getline(ss, fileAddress, ',');

        if (fileUser == uname && filePass == pass) {
            customer.username = fileUser;
            customer.password = filePass;
            customer.phone = filePhone;
            customer.address = fileAddress;

            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}


void viewRestaurants() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    setColor(11);  // light blue
    cout << "\nAvailable Restaurants:\n";
    setColor(7);   // reset to default

    for (int i = 0; i < MAX_OWNERS; i++) {
        cout << (i+1) << ". " << owners[i].restaurantName << "\n";
    }

    setColor(11);
    cout << "Enter restaurant number: ";
    setColor(7);
}

void viewRestaurantMenu(int index){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (index < 1 || index > MAX_OWNERS) {
        setColor(6);  // light yellow/orange
        cout << "Invalid restaurant ID!\n";
        setColor(7);
        return;
    }

    Owner &rest = owners[index - 1];

    setColor(6);
    cout << "\n--- Menu of " << rest.restaurantName << " ---\n";
    setColor(7);

    if (rest.menuHead == nullptr) {
        setColor(6);
        cout << "No items available!\n";
        setColor(7);
        return;
    }

    MenuItem* temp = rest.menuHead;
    int serial = 1;

    setColor(6);
    cout << left << setw(5)  << "No."
         << left << setw(20) << "Name"
         << right << setw(12) << "Price"
         << right << setw(8)  << "Qty" << "\n";
    cout << string(5+20+12+8, '-') << "\n";
    setColor(7);

    while (temp != nullptr) {
        cout << left << setw(5)  << serial
             << left << setw(20) << temp->name
             << right << setw(12) << fixed << setprecision(2) << temp->price
             << right << setw(8)  << temp->quantity << "\n";

        temp = temp->next;
        serial++;
    }

    setColor(6);
    cout << string(5+20+12+8, '-') << "\n";
    setColor(7);
}
void searchFoodAll(string query) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    bool found = false;

    // হেডার হলুদ
    setColor(6); // light yellow
    cout << "\n--- Search Results for \"" << query << "\" ---\n";
    cout << left << setw(20) << "Restaurant"
         << left << setw(20) << "Food Name"
         << right << setw(12) << "Price"
         << right << setw(8)  << "Qty" << "\n";
    cout << string(20+20+12+8, '-') << "\n";
    setColor(7); // reset to default

    for (int i = 0; i < MAX_OWNERS; i++) {
        MenuItem* temp = owners[i].menuHead;
        while (temp != nullptr) {
            string itemName = temp->name;
            string searchQ = query;
            transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
            transform(searchQ.begin(), searchQ.end(), searchQ.begin(), ::tolower);

            if (itemName.find(searchQ) != string::npos) {
                // খাবারের রঙ ডিফল্ট
                cout << left << setw(20) << owners[i].restaurantName
                     << left << setw(20) << temp->name
                     << right << setw(12) << fixed << setprecision(2) << temp->price
                     << right << setw(8)  << temp->quantity << "\n";
                found = true;
            }
            temp = temp->next;
        }
    }

    if (!found) {
        cout << "No matching items found!\n";
    }

    // footer হলুদ
    setColor(6);
    cout << string(20+20+12+8, '-') << "\n";
    setColor(7); // reset again
}

void searchFoodByRange(int minPrice, int maxPrice) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    bool found = false;

    setColor(6); // light yellow / কমলা
    cout << "\n--- Foods Available Between " << minPrice << " and " << maxPrice << " Taka ---\n";
    cout << left << setw(20) << "Restaurant"
         << left << setw(20) << "Food Name"
         << right << setw(12) << "Price"
         << right << setw(8)  << "Qty" << "\n";
    cout << string(20 + 20 + 12 + 8, '-') << "\n";
    setColor(7); // reset to default

    for (int i = 0; i < MAX_OWNERS; i++) {
        MenuItem* temp = owners[i].menuHead;

        while (temp != nullptr) {
            if (temp->price >= minPrice && temp->price <= maxPrice) {
                cout << left << setw(20) << owners[i].restaurantName
                     << left << setw(20) << temp->name
                     << right << setw(12) << fixed << setprecision(2) << temp->price
                     << right << setw(8)  << temp->quantity << "\n";
                found = true;
            }
            temp = temp->next;
        }
    }

    if (!found) {
        setColor(6);
        cout << "No foods found in this price range.\n";
        setColor(7);
    }

    setColor(6);
    cout << string(20 + 20 + 12 + 8, '-') << "\n";
    setColor(7);
}

void addToCart(int restIndex, int serial, int qty) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    setColor(11);  // light blue
    cout << qty << " x ";
    setColor(7);

    // ↓ নিচের কোড আগের মতোই রাখো
    if (restIndex < 1 || restIndex > MAX_OWNERS) {
        cout << "Invalid restaurant!\n";
        return;
    }

    Owner &rest = owners[restIndex - 1];
    MenuItem* temp = rest.menuHead;
    int currentSerial = 1;

    while (temp != nullptr && currentSerial < serial) {
        temp = temp->next;
        currentSerial++;
    }

    if (temp == nullptr) {
        cout << "Invalid serial number!\n";
        return;
    }

    if (qty > temp->quantity) {
        cout << "Not enough stock available!\n";
        return;
    }

    bool itemExists = false;
    for (auto &item : cart) {
        if (item.name == temp->name && item.restaurant == rest.restaurantName) {
            item.quantity += qty;
            itemExists = true;
            break;
        }
    }

    if (!itemExists) {
        CartItem c;
        c.restaurant = rest.restaurantName;
        c.name = temp->name;
        c.price = temp->price;
        c.quantity = qty;
        c.code = temp->code;
        cart.push_back(c);
    }

    setColor(11);  // light blue message
    cout << qty << " x " << temp->name
         << " from " << rest.restaurantName << " added to cart!\n";
    setColor(7);
}vector<int> indexMap;

void viewCart() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    indexMap.clear();   // IMPORTANT

    setColor(6);
    cout << "\n--- Your Cart (Items from Multiple Restaurants) ---\n";
    setColor(7);

    if (cart.empty()) {
        setColor(6);
        cout << "Cart is empty!\n";
        setColor(7);
        return;
    }

    double total = 0;

    const int w0 = 5;
    const int w1 = 20, w2 = 25, w3 = 10, w4 = 8, w5 = 12;

    setColor(6);
    cout << left << setw(w0) << "No."
         << left << setw(w1) << "Restaurant"
         << left << setw(w2) << "Item Name"
         << right << setw(w3) << "Price"
         << right << setw(w4) << "Qty"
         << right << setw(w5) << "Subtotal" << "\n";

    cout << string(w0 + w1 + w2 + w3 + w4 + w5, '-') << "\n";
    setColor(7);

    // Group but also keep original index
    map<string, vector<int>> groups;

    for (int i = 0; i < cart.size(); i++)
        groups[cart[i].restaurant].push_back(i);

    int itemNumber = 1;

    for (auto &[restaurant, idxList] : groups) {

        setColor(6);
        cout << left << setw(w0) << ""
             << left << setw(w1) << restaurant << "\n";
        setColor(7);

        for (int idx : idxList) {
            auto &c = cart[idx];
            double subtotal = c.price * c.quantity;
            total += subtotal;

            cout << left << setw(w0) << itemNumber
                 << left << setw(w1) << ""
                 << left << setw(w2) << c.name
                 << right << setw(w3) << c.price
                 << right << setw(w4) << c.quantity
                 << right << setw(w5) << subtotal << "\n";

            indexMap.push_back(idx);  // store original index
            itemNumber++;
        }

        cout << string(w0 + w1 + w2 + w3 + w4 + w5, '-') << "\n";
    }

    setColor(6);
    cout << left << setw(w0 + w1 + w2 + w3 + w4) << "Total:"
         << right << setw(w5) << total << " TK\n";
    setColor(7);
}


vector<DeliveryPoint> deliveryPoints = {
    {"Bijoy 24 hall", 0, 5},
    {"Syed Mujtaba Ali Hall", 0, 5},
    {"Shah Poran Hall", 0, 5},
    {"Ayesha Siddika  Hall", 0, 5},
    {"Dr.Sirajunnesa Chowdhury Hall", 0, 5},
    {"Fatema Tuz Zahra Hall", 0, 5},
    {"Samad 1 Sub Hall", 5, 5},
    {"Samad 2 Sub Hall", 5, 5},
    {"Amir Complex Sub Hall", 5, 5},
    {"Modina Market Point",10, 10},
    {"Ambarkhana Point", 50, 15},
    {"Zindabazar Point", 50, 15},
    {"Subidbazar Point", 40, 12},
    {"Tilagor Point", 60, 20},
    {"Akhalia Point", 30, 10},
    {"University Gate", 5, 8}
};


void manageDeliveryPoints() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;
    do {
        SetConsoleTextAttribute(hConsole, 10);
        cout << "\n===== Delivery Point Management =====\n";
        SetConsoleTextAttribute(hConsole, 7);

        cout << "1. View all delivery points\n";
        cout << "2. Add a delivery point\n";
        cout << "3. Update a delivery point\n";
        cout << "4. Delete a delivery point\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); // clear newline

        if (choice == 1) {
    system("cls");

    // Header white
    SetConsoleTextAttribute(hConsole, 15);
    cout << "\nDelivery Point                 Delivery (TK)     Time (min)\n";
    cout << "---------------------------------------------------------------\n";
    SetConsoleTextAttribute(hConsole, 6); // Light brown/yellow

    for (int i = 0; i < deliveryPoints.size(); i++) {
        cout << left << setw(3) << (to_string(i+1) + ".")      // No.
             << setw(30) << deliveryPoints[i].name             // Name
             << setw(14) << fixed << setprecision(2) << deliveryPoints[i].price  // Price
             << setw(10) << deliveryPoints[i].timeMinutes      // Time
             << "\n";
    }

    SetConsoleTextAttribute(hConsole, 15);
    cout << "---------------------------------------------------------------\n";
    SetConsoleTextAttribute(hConsole, 7);
}
        else if (choice == 2) { // Add
            DeliveryPoint dp;
            cout << "Enter delivery place name: ";
            getline(cin, dp.name);
            cout << "Enter delivery price: ";
            cin >> dp.price;
            cout << "Enter delivery time (minutes): ";
            cin >> dp.timeMinutes;
            cin.ignore();
            deliveryPoints.push_back(dp);
            cout << "Delivery point added successfully!\n";
        }
        else if (choice == 3) { // Update
            int idx;
            cout << "Enter delivery point number to update: ";
            cin >> idx;
            cin.ignore();
            if (idx < 1 || idx > deliveryPoints.size()) {
                cout << "Invalid number!\n";
                continue;
            }
            idx--; // zero-based index
            cout << "Enter new name (current: " << deliveryPoints[idx].name << "): ";
            string newName; getline(cin, newName);
            if (!newName.empty()) deliveryPoints[idx].name = newName;
            cout << "Enter new price (current: " << deliveryPoints[idx].price << "): ";
            double newPrice; cin >> newPrice;
            deliveryPoints[idx].price = newPrice;
            cout << "Enter new delivery time in minutes (current: " << deliveryPoints[idx].timeMinutes << "): ";
            int newTime; cin >> newTime;
            deliveryPoints[idx].timeMinutes = newTime;
            cin.ignore();
            cout << "Delivery point updated!\n";
        }
        else if (choice == 4) { // Delete
            int idx;
            cout << "Enter delivery point number to delete: ";
            cin >> idx;
            cin.ignore();
            if (idx < 1 || idx > deliveryPoints.size()) {
                cout << "Invalid number!\n";
                continue;
            }
            deliveryPoints.erase(deliveryPoints.begin() + idx - 1);
            cout << "Delivery point deleted!\n";
        }

    } while (choice != 0);
}


// ====================
// Check First Order
// ====================



void editProfile(Customer &c) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "\n===== Edit Profile =====\n";

    // Username
    SetConsoleTextAttribute(h, 11);  // 🔵 হালকা নীল
    cout << "Enter new username (old: " << c.username << "): ";
    SetConsoleTextAttribute(h, 7);   // ⚪ ডিফল্ট
    cin >> c.username;

    // Phone
    SetConsoleTextAttribute(h, 11);
    cout << "Enter new phone (old: " << c.phone << "): ";
    SetConsoleTextAttribute(h, 7);
    cin >> c.phone;

    // Address
    cin.ignore();
    SetConsoleTextAttribute(h, 11);
    cout << "Enter new address (old: " << c.address << "): ";
    SetConsoleTextAttribute(h, 7);
    getline(cin, c.address);

    // Success message
    SetConsoleTextAttribute(h, 11);
    cout << "\nProfile updated successfully!\n";
    SetConsoleTextAttribute(h, 7);
}


    // ----------------------------
void submitReview(Customer &c) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. Restaurant List
    SetConsoleTextAttribute(h, 11);
    cout << "\n--- Restaurant List ---\n";
    SetConsoleTextAttribute(h, 7);

    vector<int> restMap;
    for (int i = 0; i < MAX_OWNERS; i++) {
        if (owners[i].restaurantName != "") {
            restMap.push_back(i);
            cout << restMap.size() << ". " << owners[i].restaurantName << "\n"; // NO COLOR
        }
    }

    if (restMap.empty()) {
        SetConsoleTextAttribute(h, 11);
        cout << "No restaurants available!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    SetConsoleTextAttribute(h, 11);
    cout << "Select restaurant number: ";
    SetConsoleTextAttribute(h, 7);
    int rChoice;
    cin >> rChoice;

    if (rChoice < 1 || rChoice > restMap.size()) {
        SetConsoleTextAttribute(h, 11);
        cout << "Invalid selection!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    int restIndex = restMap[rChoice - 1];
    Owner &rest = owners[restIndex];

    // 2. Menu
    if (rest.menuHead == nullptr) {
        SetConsoleTextAttribute(h, 11);
        cout << "This restaurant has no menu!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    SetConsoleTextAttribute(h, 11); // Bright cyan for header
cout << "\n--- Menu of " << rest.restaurantName << " ---\n";
SetConsoleTextAttribute(h, 7);

// Table Header
int w1 = 5, w2 = 25, w3 = 10, w4 = 10; // column widths
SetConsoleTextAttribute(h, 14); // Light brown/yellow for header
cout << left << setw(w1) << "No"
     << left << setw(w2) << "Food Name"
     << right << setw(w3) << "Price"
     << right << setw(w4) << "Qty" << "\n";
cout << string(w1 + w2 + w3 + w4, '-') << "\n";
SetConsoleTextAttribute(h, 7); // reset for rows

// Print menu items
MenuItem *temp = rest.menuHead;
int serial = 1;
while (temp != nullptr) {
    cout << left << setw(w1) << serial
         << left << setw(w2) << temp->name
         << right << setw(w3) << fixed << setprecision(2) << temp->price
         << right << setw(w4) << temp->quantity
         << "\n";
    temp = temp->next;
    serial++;
}

// 3. Choose Food
SetConsoleTextAttribute(h, 11);
cout << "\nSelect food number to review: ";
SetConsoleTextAttribute(h, 7);
int fChoice;
cin >> fChoice;

// Navigate to the chosen item
temp = rest.menuHead;
serial = 1;
while (temp != nullptr && serial < fChoice) {
    temp = temp->next;
    serial++;
}

if (temp == nullptr) {
    SetConsoleTextAttribute(h, 11);
    cout << "Invalid food selection!\n";
    SetConsoleTextAttribute(h, 7);
    return;
}

    string foodName = temp->name;

    // 4. Rating
    int rating;
    SetConsoleTextAttribute(h, 11);
    cout << "Enter rating (1-5): ";
    SetConsoleTextAttribute(h, 7);
    cin >> rating;

    if (rating < 1 || rating > 5) {
        SetConsoleTextAttribute(h, 11);
        cout << "Invalid rating!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    cin.ignore();

    // 5. Comment
    string comment;
    SetConsoleTextAttribute(h, 11);
    cout << "Enter your comment: ";
    SetConsoleTextAttribute(h, 7);
    getline(cin, comment);

    // Save Review
    ofstream fout("reviews.txt", ios::app);
    fout << rest.restaurantName << ","
         << foodName << ","
         << c.username << ","
         << rating << ","
         << comment << "\n";
    fout.close();

    SetConsoleTextAttribute(h, 11);
    cout << "Review submitted successfully!\n";
    SetConsoleTextAttribute(h, 7);
}


void viewReviews() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(h, 6); // light orange
    cout << "\n--- Restaurant List ---\n";
    SetConsoleTextAttribute(h, 7);

    vector<int> restMap;

    for (int i = 0; i < MAX_OWNERS; i++) {
        if (owners[i].restaurantName != "") {
            restMap.push_back(i);
            cout << restMap.size() << ". " << owners[i].restaurantName << "\n";
        }
    }

    if (restMap.empty()) {
        SetConsoleTextAttribute(h, 6);
        cout << "No restaurants available!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    SetConsoleTextAttribute(h, 6);
    cout << "Select restaurant number: ";
    SetConsoleTextAttribute(h, 7);

    int rChoice;
    cin >> rChoice;

    if (rChoice < 1 || rChoice > restMap.size()) {
        SetConsoleTextAttribute(h, 6);
        cout << "Invalid selection!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    int restIndex = restMap[rChoice - 1];
    string restName = owners[restIndex].restaurantName;

    ifstream fin("reviews.txt");
    if (!fin) {
        SetConsoleTextAttribute(h, 6);
        cout << "No reviews found!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    SetConsoleTextAttribute(h, 6);
    cout << "\n--- Reviews for " << restName << " ---\n";
    SetConsoleTextAttribute(h, 7);

    string line;
    bool found = false;

    while (getline(fin, line)) {
        stringstream ss(line);
        string rName, food, user, ratingStr, comment;

        getline(ss, rName, ',');
        getline(ss, food, ',');
        getline(ss, user, ',');
        getline(ss, ratingStr, ',');
        getline(ss, comment);

        if (rName == restName) {
            cout << "Food: " << food << " | "
                 << "User: " << user << " | "
                 << "Rating: " << ratingStr << "/5 | "
                 << comment << "\n";
            found = true;
        }
    }

    if (!found) {
        SetConsoleTextAttribute(h, 6);
        cout << "No reviews yet.\n";
        SetConsoleTextAttribute(h, 7);
    }

    fin.close();
}

void viewPendingOrders(string restName) {
     HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    ifstream fin("pendingorders.txt");
    if (!fin || fin.peek() == EOF) {
             SetConsoleTextAttribute(h, 3);
        cout << "\n\n  ====No pending orders found!===   \n\n";
        SetConsoleTextAttribute(h,7);
        return;
    }

    string line;
    int count = 0;
      SetConsoleTextAttribute(h, 14);
       cout << "\n==============================================\n";
    cout << "\n      Pending Orders for " << restName << "      \n";
   cout << "\n==============================================\n";
    while (getline(fin, line)) {
        stringstream ss(line);
        string restaurant, customer, location, item;
        int qty;
        double price;

        getline(ss, restaurant, ',');
        getline(ss, customer, ',');
        getline(ss, location, ',');
        getline(ss, item, ',');
        ss >> qty; ss.ignore(); // ignore comma after qty
        ss >> price;
  SetConsoleTextAttribute(h,15);
        if (restaurant == restName) {
            count++;
            cout << count << ".\n";
            cout << "   Customer: " << customer << "\n";
            cout << "   Location: " << location << "\n";
            cout << "   Item: " << item << "\n";
            cout << "   Qty: " << qty << "\n";
            cout << "   Price: " << price << " TK\n";
              SetConsoleTextAttribute(h,6);

            cout << "----------------------------\n";
        }
    }
          SetConsoleTextAttribute(h,7);
    if (count == 0) cout << "No orders found for your restaurant.\n";
    fin.close();
}
int getDeliveryMinutes(string location) {
    for (auto &d : deliveryPoints) {
        if (d.name == location) {
            return d.timeMinutes;
        }
    }
    return 5; // default fallback
}
unordered_map<string, int> loadPreparationTimes() {
    unordered_map<string, int> prepTimes;
    ifstream fin("menu.txt");
    if (!fin) return prepTimes;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string rest, item, prepStr;
        int code;
        double price;
        int quantity;

        getline(ss, rest, ',');
        getline(ss, item, ',');
        ss >> code; ss.ignore();
        ss >> price; ss.ignore();
        ss >> quantity; ss.ignore();
        getline(ss, prepStr);

        size_t pos = prepStr.find("secs");
        int prepSeconds = 0;
        if (pos != string::npos) {
            prepSeconds = stoi(prepStr.substr(0, pos - 1));
        }

        string key = rest + "_" + to_string(code);
        prepTimes[key] = prepSeconds;
    }
    fin.close();
    return prepTimes;
}
void setPreparationTime(Owner &owner) {
     HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream fin("pendingorders.txt");
    if (!fin || fin.peek() == EOF) {
        cout << "\nNo pending orders.\n";
        return;
    }

    vector<string> allLines;
    string line;
    while (getline(fin, line)) allLines.push_back(line);
    fin.close();

    vector<int> ownerIndexes;
    for (int i = 0; i < allLines.size(); i++) {
        stringstream ss(allLines[i]);
        string rest; getline(ss, rest, ',');
        if (rest == owner.restaurantName) ownerIndexes.push_back(i);
    }

    if (ownerIndexes.empty()) {
        cout << "\nNo pending orders for your restaurant.\n";
        return;
    }

    // ********* FIX: Load existing prep_times.txt first *********
    vector<string> prepTimesLines;
    ifstream oldPrep("prep_times.txt");
    string oldLine;
    while (getline(oldPrep, oldLine)) {
        if (!oldLine.empty())
            prepTimesLines.push_back(oldLine);
    }
    oldPrep.close();
    // ************************************************************

    while (!ownerIndexes.empty()) {

      SetConsoleTextAttribute(h, 14);
        cout << "\n=== Pending Orders for " << owner.restaurantName << " ===\n";
        for (int i = 0; i < ownerIndexes.size(); i++) {
            stringstream ss(allLines[ownerIndexes[i]]);
            string rest, customer, location, item, qtyStr, priceStr;
            getline(ss, rest, ','); getline(ss, customer, ','); getline(ss, location, ',');
            getline(ss, item, ','); getline(ss, qtyStr, ','); getline(ss, priceStr, ',');
             SetConsoleTextAttribute(h,7);
            cout << i + 1 << ". Customer: " << customer
                 << " | Location: " << location
                 << " | Item: " << item
                 << " | Qty: " << qtyStr
                 << " | Price: " << priceStr << " TK\n";
        }
            SetConsoleTextAttribute(h,11);
        int choice;
        cout << "\nSelect order number to set preparation time: ";
        cin >> choice;
        if (choice < 1 || choice > ownerIndexes.size()) {
            cout << "Invalid choice!\n";
            continue;
        }

        int prepMinutes;
        cout << "Enter preparation time (minutes): ";
        cin >> prepMinutes;
        int prepSeconds = prepMinutes * 60;

        int idx = ownerIndexes[choice - 1];
        stringstream ss(allLines[idx]);
        string rest, customer, location, item, qtyStr, priceStr;
        getline(ss, rest, ','); getline(ss, customer, ','); getline(ss, location, ',');
        getline(ss, item, ','); getline(ss, qtyStr, ','); getline(ss, priceStr, ',');

        // Add new prep time line (existing ones already loaded above)
        prepTimesLines.push_back(rest + "," + customer + "," + location + "," + item + "," +
                                 qtyStr + "," + priceStr + "," + to_string(prepSeconds) + " secs");

        cout << "Preparation time set for " << item << "\n";

        // Remove processed order
        allLines.erase(allLines.begin() + idx);

        // Refresh ownerIndexes
        ownerIndexes.clear();
        for (int i = 0; i < allLines.size(); i++) {
            stringstream ss2(allLines[i]);
            string r; getline(ss2, r, ',');
            if (r == owner.restaurantName) ownerIndexes.push_back(i);
        }

        if (ownerIndexes.empty()) break;
              SetConsoleTextAttribute(h, 15);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        char again;
        cout << "\nProcess another order? (Y/N): ";
        cin >> again;
        if (again == 'N' || again == 'n') break;
    }

    // Write all prep times (old + new)
    ofstream foutPrep("prep_times.txt");
    for (auto &l : prepTimesLines) foutPrep << l << "\n";
    foutPrep.close();

    // Rewrite remaining pending orders
    ofstream foutPending("pendingorders.txt");
    for (auto &l : allLines) foutPending << l << "\n";
    foutPending.close();

          SetConsoleTextAttribute(h,3);
    cout << "\nAll preparation times updated successfully!\n";
          SetConsoleTextAttribute(h,7);
}

void showEstimatedDeliveryTime(const Customer& c) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    ifstream fin("prep_times.txt");
    if (!fin || fin.peek() == EOF) {
        SetConsoleTextAttribute(h, 12); // Red
        cout << "No preparation data found!\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }

    SetConsoleTextAttribute(h, 10); // Bright Green for header
    cout << "\n=== Estimated Delivery Time ===\n";
    SetConsoleTextAttribute(h, 7);

    string line;
    bool found = false;

    while (getline(fin, line)) {
        stringstream ss(line);
        string rest, customer, location, item, qtyStr, priceStr, prepStr;
        getline(ss, rest, ',');
        getline(ss, customer, ',');
        getline(ss, location, ',');
        getline(ss, item, ',');
        getline(ss, qtyStr, ',');
        getline(ss, priceStr, ',');
        getline(ss, prepStr);

        if (customer != c.username) continue;
        found = true;

        int prepSeconds = 0;
        size_t pos = prepStr.find(" ");
        if (pos != string::npos) prepSeconds = stoi(prepStr.substr(0, pos));
        else prepSeconds = stoi(prepStr);

        int prepMinutes = prepSeconds / 60;
        int deliveryMinutes = getDeliveryMinutes(location);
        int totalMinutes = prepMinutes + deliveryMinutes;

        // Print info with labels bright green
        SetConsoleTextAttribute(h, 10); // Bright Green
        cout << "\nRestaurant: ";
        SetConsoleTextAttribute(h, 7);
        cout << rest << "\n";

        SetConsoleTextAttribute(h, 10);
        cout << "Item: ";
        SetConsoleTextAttribute(h, 7);
        cout << item << "\n";

        SetConsoleTextAttribute(h, 10);
        cout << "Qty: ";
        SetConsoleTextAttribute(h, 7);
        cout << qtyStr << "\n";

        SetConsoleTextAttribute(h, 10);
        cout << "Preparation Time: ";
        SetConsoleTextAttribute(h, 7);
        cout << prepMinutes << " minutes\n";

        SetConsoleTextAttribute(h, 10);
        cout << "Delivery Time: ";
        SetConsoleTextAttribute(h, 7);
        cout << deliveryMinutes << " minutes\n";

        SetConsoleTextAttribute(h, 10);
        cout << "Total Delivery Time: ";
        SetConsoleTextAttribute(h, 7);
        cout << totalMinutes << " minutes\n";

        SetConsoleTextAttribute(h, 10);
        cout << "-----------------------------------------\n";
        SetConsoleTextAttribute(h, 7);
    }

    if (!found) {
        SetConsoleTextAttribute(h, 12); // Red
        cout << "\nNo orders found for your account!\n";
        SetConsoleTextAttribute(h, 7);
    }
}

void saveOrderToHistory(string restaurant, string customer, string location,
                      string item, int qty, double price)
{
    ofstream fout("order_history.txt", ios::app);

    fout << restaurant << ","
         << customer << ","
         << location << ","
         << item << ","
         << qty << ","     // no conversion needed
         << price << "\n"; // no conversion needed

    fout.close();
}


void viewAllPreviousOrders(string restName) {
     HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream fin("order_history.txt");
    if (!fin || fin.peek() == EOF) {
        cout << "\nNo previous orders found!\n";
        SetConsoleTextAttribute(h,7);
        return;

    }

    string line;
    int count = 0;
          SetConsoleTextAttribute(h, 14);
cout << "\n==============================================\n";
cout << "\n     All Previous Orders for " << restName << "       \n";
    cout << "\n==============================================\n";

    while (getline(fin, line)) {
        stringstream ss(line);
        string restaurant, customer, location, item, prepStr;
        int qty;
        double price;

        getline(ss, restaurant, ',');
        getline(ss, customer, ',');
        getline(ss, location, ',');
        getline(ss, item, ',');
        ss >> qty; ss.ignore();
        ss >> price; ss.ignore();
        getline(ss, prepStr);
       SetConsoleTextAttribute(h, 15);
        if (restaurant == restName) {
            count++;
            cout << count << ".\n";
            cout << "   Customer: " << customer << "\n";
            cout << "   Location: " << location << "\n";
            cout << "   Item: " << item << "\n";
            cout << "   Qty: " << qty << "\n";
            cout << "   Price: " << price << " TK\n";
                      SetConsoleTextAttribute(h,6);
            cout << "----------------------------\n";
        }
    }

    if (count == 0) cout << "No previous orders found for your restaurant.\n";
    fin.close();
           SetConsoleTextAttribute(h,7);
}

void ViewReviews(Owner &owner) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream fin("reviews.txt");
    if (!fin) {
        cout << "No reviews found!\n";
        return;
    }
    SetConsoleTextAttribute(h, 14);
    cout << "\n==============================================\n";
    cout << "\n        Reviews for " << owner.restaurantName << "    \n";
cout << "\n==============================================\n";
    string line;
    bool found = false;

    while (getline(fin, line)) {
        stringstream ss(line);
        string rName, food, user, ratingStr, comment;

        getline(ss, rName, ',');
        getline(ss, food, ',');
        getline(ss, user, ',');
        getline(ss, ratingStr, ',');
        getline(ss, comment);
    SetConsoleTextAttribute(h,7);
        if (rName == owner.restaurantName) {
           cout << "Food: " << food << "\n"
     << "User: " << user << "\n"
     << "Rating: " << ratingStr << "/5\n"
     << "Comment: " << comment << "\n";
         SetConsoleTextAttribute(h,6);
     cout<< "----------------------------------------------\n";

            found = true;
        }
    }
    SetConsoleTextAttribute(h, 7);
    if (!found)
        cout << "No reviews yet.\n";

    fin.close();
}
void updateItemStock(string restaurant) {
     HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int code;
    SetConsoleTextAttribute(h, 7);
    cout << "Enter item code: ";
    cin >> code;

    ifstream fin("menu.txt");
    if (!fin) { cout << "menu.txt not found!\n"; return; }

    vector<string> allLines;
    string line;
    bool found = false;

    int updatedQuantity = -1;   // store updated quantity globally for linked list sync

    while (getline(fin, line)) {

        string rest, name;
        int c, quantity;
        double price;

        stringstream ss(line);
        getline(ss, rest, ',');
        ss >> c; ss.ignore();
        getline(ss, name, ',');
        ss >> price; ss.ignore();
        ss >> quantity;
       SetConsoleTextAttribute(h,15);
        if (rest == restaurant && c == code) {
            found = true;
            cout << "\nItem Found!\nName: " << name << "\nCurrent Quantity: " << quantity << "\n";
            cout << "1. Add Quantity\n2. Remove Quantity\nChoice: ";
            int ch; cin >> ch;
                SetConsoleTextAttribute(h,11);
            int change;
            cout << "Enter amount: ";
            cin >> change;

            if (ch == 1) quantity += change;
            else if (ch == 2) {
                if (change > quantity) {
                    cout << "Cannot remove more than current stock!\n";
                } else {
                    quantity -= change;
                }
            }

            updatedQuantity = quantity;   // save to update linked list later
    SetConsoleTextAttribute(h, 7);
            cout << "Updated Quantity: " << quantity << endl;
        }

        string updated = rest + "," + to_string(c) + "," + name + "," + to_string(price) + "," + to_string(quantity);
        allLines.push_back(updated);
    }
    fin.close();

    if (!found) {
        cout << "Item code not found!\n";
        return;
    }

    ofstream fout("menu.txt");
    for (auto &l : allLines) fout << l << "\n";
    fout.close();

    for (int i = 0; i < MAX_OWNERS; i++) {
        if (owners[i].restaurantName == restaurant) {

            MenuItem* t = owners[i].menuHead;

            while (t != nullptr) {
                if (t->code == code) {
                    t->quantity = updatedQuantity;   // sync value
                    break;
                }
                t = t->next;
            }

            break;
        }
    }
        SetConsoleTextAttribute(h,3);
    cout << "Stock updated successfully!\n";
        SetConsoleTextAttribute(h, 7);
}

void deductStock(string restaurant, int code, int qtyOrdered) {
    ifstream fin("menu.txt");
    if (!fin) return;

    vector<string> allLines;
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string rest, name;
        int c, quantity;
        double price;

        getline(ss, rest, ',');
        ss >> c; ss.ignore();
        getline(ss, name, ',');
        ss >> price; ss.ignore();
        ss >> quantity;

        if (rest == restaurant && c == code) {
            if (quantity >= qtyOrdered) {
                quantity -= qtyOrdered;
                // cout << "Stock updated: " << name << " - New quantity: " << quantity << "\n"; // remove this
            } else {
                // cout << "Not enough stock for " << name << "! Available: " << quantity << ", Ordered: " << qtyOrdered << "\n"; // optional: remove
            }
            line = rest + "," + to_string(c) + "," + name + "," + to_string(price) + "," + to_string(quantity);
        }

        allLines.push_back(line);
    }
    fin.close();

    ofstream fout("menu.txt");
    for (auto &l : allLines) fout << l << "\n";
    fout.close();

    // Update linked list
    for (int i = 0; i < MAX_OWNERS; i++) {
        if (owners[i].restaurantName == restaurant) {
            MenuItem* temp = owners[i].menuHead;
            while (temp != nullptr) {
                if (temp->code == code) {
                    if (temp->quantity >= qtyOrdered) temp->quantity -= qtyOrdered;
                    break;
                }
                temp = temp->next;
            }
            break;
        }
    }
}

void removeFromCart() {
    if (cart.empty()) {
        cout << "Cart is empty!\n";
        return;
    }

    viewCart();

    cout << "\nEnter item number to remove (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice == 0) return;

    if (choice < 1 || choice > indexMap.size()) {
        cout << "Invalid number!\n";
        return;
    }

    int realIndex = indexMap[choice - 1];  // mapped index

    cout << "Removing \"" << cart[realIndex].name << "\"...\n";

    cart.erase(cart.begin() + realIndex);

    cout << "Item removed successfully!\n";
}


void distributePayment(const string& customer, const string& restaurant, double totalAmount) {
    double adminAmount = totalAmount * 0.20;
    double ownerAmount = totalAmount * 0.80;

    string orderId = "ORD" + to_string(time(0)) + customer.substr(0, 3);

    time_t now = time(0);
    tm* localTime = localtime(&now);
    string date = to_string(localTime->tm_year + 1900) + "-" +
                  to_string(localTime->tm_mon + 1) + "-" +
                  to_string(localTime->tm_mday);

    ofstream fout("payments.txt", ios::app);
    if(!fout) {
        cout << "Error opening payments.txt for writing!\n";
        return;
    }

    fout << orderId << ","
         << customer << ","
         << restaurant << ","
         << fixed << setprecision(2) << totalAmount << ","
         << adminAmount << ","
         << ownerAmount << ","
         << date << "\n";
    fout.close();

    cout << "\n=== Payment Distribution ===\n";
    cout << "Order ID    : " << orderId << "\n";
    cout << "Total Amount: " << totalAmount << " TK\n";
    cout << "Date        : " << date << "\n";
    cout << "============================\n";
}

// Function to calculate total earnings for admin
double getAdminTotalEarnings() {
    ifstream fin("payments.txt");
    if (!fin) return 0.0;

    double total = 0.0;
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;

        getline(ss, orderId, ',');
        getline(ss, customer, ',');
        getline(ss, restaurant, ',');
        getline(ss, totalStr, ',');
        getline(ss, adminStr, ',');
        getline(ss, ownerStr, ',');
        getline(ss, date);

        total += stod(adminStr);
    }
    fin.close();
    return total;
}

// Function to calculate total earnings for a specific owner
double getOwnerTotalEarnings(const string& restaurantName) {
    ifstream fin("payments.txt");
    if (!fin) return 0.0;

    double total = 0.0;
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;

        getline(ss, orderId, ',');
        getline(ss, customer, ',');
        getline(ss, restaurant, ',');
        getline(ss, totalStr, ',');
        getline(ss, adminStr, ',');
        getline(ss, ownerStr, ',');
        getline(ss, date);

        if (restaurant == restaurantName) {
            total += stod(ownerStr);
        }
    }
    fin.close();
    return total;
}

// Function to view all payment records for admin
void viewAllPaymentRecords() {
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);


ifstream fin("payments.txt");
if (!fin || fin.peek() == EOF) {
    SetConsoleTextAttribute(h, 12); // Red
    cout << "\nNo payment records found!\n";
    SetConsoleTextAttribute(h, 7);
    return;
}

SetConsoleTextAttribute(h, 14); // Yellow Headline
cout << "\n===== All Payment Records =====\n";
SetConsoleTextAttribute(h, 7);

string line;
int count = 0;
double grandTotal = 0, grandAdmin = 0, grandOwner = 0;

while (getline(fin, line)) {
    if (line.empty()) continue;
    stringstream ss(line);
    string orderId, custId, custName, restaurant, totalStr, adminStr, ownerStr, date;

    // Read first 7 fields
    vector<string> fields;
    string temp;
    while (getline(ss, temp, ',')) {
        fields.push_back(temp);
    }

    // Adjust parsing if file has 7 fields (customerID missing)
    if (fields.size() == 7) {
        orderId = fields[0];
        custId = "";              // Missing customer ID
        custName = fields[1];
        restaurant = fields[2];
        totalStr = fields[3];
        adminStr = fields[4];
        ownerStr = fields[5];
        date = fields[6];
    } else if (fields.size() >= 8) {
        orderId = fields[0];
        custId = fields[1];
        custName = fields[2];
        restaurant = fields[3];
        totalStr = fields[4];
        adminStr = fields[5];
        ownerStr = fields[6];
        date = fields[7];
    } else {
        continue; // invalid line
    }

    // Trim whitespace
    auto trim = [](string &s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
    };
    trim(orderId); trim(custId); trim(custName); trim(restaurant);
    trim(totalStr); trim(adminStr); trim(ownerStr); trim(date);

    double total = 0, admin = 0, owner = 0;
    try {
        total = stod(totalStr);
        admin = stod(adminStr);
        owner = stod(ownerStr);
    } catch(...) {
        continue;
    }

    grandTotal += total;
    grandAdmin += admin;
    grandOwner += owner;
    count++;

    // Print neatly
    cout << count << ". Order ID: " << orderId << "\n";
    if (!custId.empty()) cout << "   Customer ID: " << custId << "\n";
    cout << "   Name       : " << custName << "\n";
    cout << "   Restaurant : " << restaurant << "\n";
    cout << "   Total      : " << fixed << setprecision(2) << total << "\n";
    cout << "   Admin      : " << fixed << setprecision(2) << admin << "\n";
    cout << "   Owner      : " << fixed << setprecision(2) << owner << "\n";
    cout << "   Date       : " << date << "\n";
    cout << "----------------------------------------\n";
}

// Grand Total summary
SetConsoleTextAttribute(h, 14); // Yellow
cout << "GRAND TOTAL:\n";
cout << "Total      : " << fixed << setprecision(2) << grandTotal << "\n";
cout << "Admin Total: " << fixed << setprecision(2) << grandAdmin << "\n";
cout << "Owner Total: " << fixed << setprecision(2) << grandOwner << "\n";
SetConsoleTextAttribute(h, 7);

fin.close();


}


void viewOwnerPaymentRecords(const string& restaurantName) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream fin("payments.txt");
    if (!fin || fin.peek() == EOF) {
        cout << "\nNo payment records found for your restaurant!\n";
        return;
    }
    SetConsoleTextAttribute(h, 14);
    cout << "\n===== Payment Records for " << restaurantName << " =====\n";
    SetConsoleTextAttribute(h,7);
    // Column headers with | separator
    cout << left << setw(15) << "Order ID"
         << "| " << left << setw(20) << "Customer"
         << "| " << right << setw(10) << "Total"
         << "| " << right << setw(12) << "Your Share"
         << "| " << left << setw(12) << "Date" << "\n";

    cout << string(15+20+10+12+12+6, '-') << "\n";

    string line;
    double ownerTotal = 0.0;
    int count = 0;

    while (getline(fin, line)) {
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;

        if (!getline(ss, orderId, ',')) continue;
        if (!getline(ss, customer, ',')) continue;
        if (!getline(ss, restaurant, ',')) continue;
        if (!getline(ss, totalStr, ',')) continue;
        if (!getline(ss, adminStr, ',')) continue;
        if (!getline(ss, ownerStr, ',')) continue;
        if (!getline(ss, date)) continue;

        restaurant.erase(restaurant.find_last_not_of(" \n\r\t")+1);

        if (restaurant == restaurantName) {
            double total = 0.0, ownerAmount = 0.0;
            try {
                total = stod(totalStr);
                ownerAmount = stod(ownerStr);
            } catch(...) { continue; }

            ownerTotal += ownerAmount;
            count++;
    SetConsoleTextAttribute(h, 15);
            cout << left << setw(15) << orderId
                 << "| " << left << setw(20) << (customer.length() > 18 ? customer.substr(0,18) + "..." : customer)
                 << "| " << right << setw(10) << fixed << setprecision(2) << total
                 << "| " << right << setw(12) << fixed << setprecision(2) << ownerAmount
                 << "| " << left << setw(12) << date << "\n";
        }
    }
        SetConsoleTextAttribute(h, 7);

    cout << string(15+20+10+12+12+6, '-') << "\n";
    SetConsoleTextAttribute(h,11);
    // Summary line
    cout << left << setw(37) << ("Total Orders: " + to_string(count))
         << right << setw(29) << "Total Earnings (80%): " << ownerTotal << " TK\n";

    fin.close();
}


void checkout_delayed(Customer &c, vector<CartItem> &cart) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    auto color = [&](int code) {
        SetConsoleTextAttribute(h, code);
    };

    if (cart.empty()) {
        color(12);
        cout << "\n[!] Cart is empty!\n";
        color(7);
        return;
    }

    color(11);
    cout << "\n============================================================\n";
    cout << "                  FINAL ORDER SUMMARY\n";
    cout << "============================================================\n\n";
    color(7);

    int w1 = 35;   // Delivery Point column width
int w2 = 15;   // Delivery (TK)
int w3 = 12;   // Time (min)

// Header white
color(15);
cout << left << setw(w1) << "Delivery Point"
     << right << setw(w2) << "Delivery (TK)"
     << right << setw(w3) << "Time (min)" << "\n";

cout << string(w1 + w2 + w3, '-') << "\n";

color(6); // body light brown

for (int i = 0; i < (int)deliveryPoints.size(); ++i) {
    string name = to_string(i+1) + ". " + deliveryPoints[i].name;

    cout << left << setw(w1) << name
         << right << setw(w2) << fixed << setprecision(2) << deliveryPoints[i].price
         << right << setw(w3) << deliveryPoints[i].timeMinutes
         << "\n";
}

color(15);
cout << string(w1 + w2 + w3, '-') << "\n";
color(7);

    int choice;
    cout << "\nEnter your choice: ";
    cin >> choice;

    if (choice < 1 || choice > (int)deliveryPoints.size()) {
        color(12);
        cout << "\nInvalid choice!\n";
        color(7);
        return;
    }

    string location     = deliveryPoints[choice-1].name;
    double deliveryCost = deliveryPoints[choice-1].price;
    int deliveryTime    = deliveryPoints[choice-1].timeMinutes;

    double total = 0;

    map<string, vector<CartItem>> group;
    for (auto &item : cart)
        group[item.restaurant].push_back(item);

    for (auto &[restaurant, items] : group) {
        color(14);
        cout << "\n------------------------------------------------------------\n";
        cout << "                  ORDER FROM \"" << restaurant << "\"\n";
        cout << "------------------------------------------------------------\n";
        color(7);

        double sub = 0;

        for (auto &item : items) {
            double cost = item.price * item.quantity;
            sub += cost;
            total += cost;

            cout << "  " << left << setw(22) << item.name
                 << " x" << item.quantity
                 << "    = " << fixed << setprecision(2) << cost << " TK\n";
        }

        cout << "  Restaurant Subtotal       = " << sub << " TK\n";
    }

    color(10);
    cout << "\n------------------------------------------------------------\n";
    cout << "Delivery Charge             : " << deliveryCost << " TK\n";
    cout << "------------------------------------------------------------\n";
    cout << "Total Payable               : " << total + deliveryCost << " TK\n";
    cout << "------------------------------------------------------------\n\n";
    color(7);

    cout << "Confirm order? (1 = Yes, 0 = No): ";
    int confirm;
    cin >> confirm;

    if (confirm != 1) {
        color(12);
        cout << "\nOrder cancelled.\n";
        color(7);
        return;
    }

    //===========================
    // SAVE pendingorders.txt
    //===========================
    ofstream fout("pendingorders.txt", ios::app);
    for (auto &item : cart) {
        fout << item.restaurant << "," << c.username << "," << location << ","
             << item.name << "," << item.quantity << "," << item.price << "\n";
    }
    fout.close();

    //===========================
    // SAVE order_history.txt  ⭐ ADDED ⭐
    //===========================
    for (auto &item : cart) {
        saveOrderToHistory(
            item.restaurant,
            c.username,
            location,
            item.name,
            item.quantity,
            item.price
        );
    }
    ofstream foutHistory("order_history.txt", ios::app);
foutHistory << "\n";
foutHistory.close();

    // Stock update
    for (auto &item : cart)
        deductStock(item.restaurant, item.code, item.quantity);

    // Payments
    map<string, double> totals;
    for (auto &item : cart)
        totals[item.restaurant] += item.price * item.quantity;

    for (auto &[rest, amount] : totals)
        distributePayment(c.username, rest, amount);

    cart.clear();

    color(10);
    cout << "\n============================================================\n";
    cout << "              YOUR ORDER HAS BEEN PLACED!\n";
    cout << "============================================================\n";
    color(7);

    cout << " Your Order has been confirmed.\n";


    this_thread::sleep_for(chrono::seconds(3));
    c.loggedIn = false;

    color(11);

    color(7);
}


// FINAL CHECKOUT FUNCTION
void viewCustomerOrders(const string& username) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    ifstream fin("order_history.txt");
    if (!fin || fin.peek() == EOF) {
        cout << "\nNo orders found!\n";
        return;
    }

    string line;
    int count = 0;
    bool insideOrder = false;  // এখন কি current order-এর মধ্যে আছি?

    SetConsoleTextAttribute(h, 11);
    cout << "\n===== Your Orders =====\n\n";
    SetConsoleTextAttribute(h, 7);

    while (getline(fin, line)) {

        // নতুন order detect
        if (line.empty()) {
            insideOrder = false;   // next non-empty line = new order
            continue;
        }

        string restaurant, customer, location, item, qty, price;
        stringstream ss(line);

        getline(ss, restaurant, ',');
        getline(ss, customer, ',');
        getline(ss, location, ',');
        getline(ss, item, ',');
        getline(ss, qty, ',');
        getline(ss, price, ',');

        if (customer == username) {

            if (!insideOrder) {
                count++;
                insideOrder = true;

                cout << "------------------------------\n";
                SetConsoleTextAttribute(h, 11);
                cout << "Order " << count << ":\n";
                SetConsoleTextAttribute(h, 7);
            }

            SetConsoleTextAttribute(h, 11); cout << "Restaurant : ";
            SetConsoleTextAttribute(h, 7); cout << restaurant << "\n";

            SetConsoleTextAttribute(h, 11); cout << "Item       : ";
            SetConsoleTextAttribute(h, 7); cout << item << "\n";

            SetConsoleTextAttribute(h, 11); cout << "Quantity   : ";
            SetConsoleTextAttribute(h, 7); cout << qty << "\n";

            SetConsoleTextAttribute(h, 11); cout << "Price      : ";
            SetConsoleTextAttribute(h, 7); cout << price << " TK\n";

            SetConsoleTextAttribute(h, 11); cout << "Location   : ";
            SetConsoleTextAttribute(h, 7); cout << location << "\n";

            SetConsoleTextAttribute(h, 11);
            cout << "Payment    : ";
            SetConsoleTextAttribute(h, 10);
            cout << "Cash On Delivery\n";
            SetConsoleTextAttribute(h, 7);
        }
    }

    cout << "------------------------------\n";

    if (count == 0)
        cout << "No orders found for your account.\n";

    fin.close();
}



// ======= Box Drawing Helpers =======
void printTopBorder(int width) {
    cout << char(201); // ╔
    for (int i = 0; i < width-2; ++i) cout << char(205); // ═
    cout << char(187) << "\n"; // ╗
}

void printBottomBorder(int width) {
    cout << char(200); // ╚
    for (int i = 0; i < width-2; ++i) cout << char(205); // ═
    cout << char(188) << "\n"; // ╝
}

void printSeparator(int width) {
    cout << char(204); // ╠
    for (int i = 0; i < width-2; ++i) cout << char(205); // ═
    cout << char(185) << "\n"; // ╣
}

// ======= Sales Report =======
void generateSalesReport() {
    ifstream fin("payments.txt");
    if (!fin) {
        setColor(15);
        cout << "No sales data found!\n";
        return;
    }

    map<string, double> restaurantEarnings;
    map<string, int> restaurantOrders;
    double totalRevenue = 0;
    int totalOrders = 0;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;
        getline(ss, orderId, ',');
        getline(ss, customer, ',');
        getline(ss, restaurant, ',');
        getline(ss, totalStr, ',');
        getline(ss, adminStr, ',');
        getline(ss, ownerStr, ',');
        getline(ss, date);

        double total = stod(totalStr);
        restaurantEarnings[restaurant] += total;
        restaurantOrders[restaurant]++;
        totalRevenue += total;
        totalOrders++;
    }
    fin.close();

    int width = 75;

 setColor(14);
    printTopBorder(width);
    cout << char(186) << setw(width/2 + 10) << right << " SALES REPORT " << setw(width/2 - 11) << "" << char(186) << "\n";
    printSeparator(width);
 setColor(7);
    cout << char(186) << " Total Revenue: " << totalRevenue << " TK"
         << setw(width - 28) << char(186) << "\n";
    cout << char(186) << " Total Orders : " << totalOrders
         << setw(width - 28) << char(186) << "\n";
    cout << char(186) << " Average Order: " << (totalOrders > 0 ? totalRevenue/totalOrders : 0) << " TK"
         << setw(width - 31) << char(186) << "\n";
 setColor(3);;
    printSeparator(width);
    cout << char(186) << setw(width/2 + 12) << right << " Restaurant Performance " << setw(width/2 - 14) << "" << char(186) << "\n";
    printSeparator(width);
 setColor(7);
    cout << char(186) << left << setw(25) << " Restaurant"
         << right << setw(10) << "Orders"
         << right << setw(15) << "Revenue"
         << right << setw(15) << "Avg/Order" << char(186) << "\n";

    printSeparator(width);

    int row = 0;
    for (auto& [restaurant, earnings] : restaurantEarnings) {
        int orders = restaurantOrders[restaurant];
        double avg = orders > 0 ? earnings/orders : 0;
        setColor(15);
        cout << char(186)
             << left << setw(25) << restaurant
             << right << setw(10) << orders
             << right << setw(15) << fixed << setprecision(2) << earnings
             << right << setw(15) << fixed << setprecision(2) << avg
             << char(186) << "\n";
        row++;
    }
 setColor(7);

    printBottomBorder(width);
}

// ======= Daily Report =======
void generateDailyReport() {
    cout << "Enter date (YYYY-MM-DD): ";
    string targetDate;
    cin >> targetDate;

    ifstream fin("payments.txt");
    if (!fin) {
        setColor(15);
        cout << "No sales data found!\n";
        return;
    }

    map<string, double> dailyRestaurantEarnings;
    double dailyRevenue = 0;
    int dailyOrders = 0;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;
        getline(ss, orderId, ',');
        getline(ss, customer, ',');
        getline(ss, restaurant, ',');
        getline(ss, totalStr, ',');
        getline(ss, adminStr, ',');
        getline(ss, ownerStr, ',');
        getline(ss, date);

        if (date == targetDate) {
            double total = stod(totalStr);
            dailyRevenue += total;
            dailyOrders++;
            dailyRestaurantEarnings[restaurant] += total;
        }
    }
    fin.close();

    int width = 60;
    setColor(14);
    printTopBorder(width);
    cout << char(186) << setw(width/2 + 10) << right << " DAILY REPORT " << setw(width/2 - 10) << "" << char(186) << "\n";
    printSeparator(width);
 setColor(7);
    cout << char(186) << " Date: " << targetDate
         << setw(width - 14 - targetDate.length()) << char(186) << "\n";
    cout << char(186) << " Daily Revenue: " << dailyRevenue << " TK"
         << setw(width - 24) << char(186) << "\n";
    cout << char(186) << " Daily Orders : " << dailyOrders
         << setw(width - 22) << char(186) << "\n";

    printSeparator(width);
    cout << char(186) << left << setw(25) << "Restaurant"
         << right << setw(15) << "Revenue" << char(186) << "\n";
    printSeparator(width);

    int row = 0;
    for (auto& [restaurant, earnings] : dailyRestaurantEarnings) {
        setColor(11);
        cout << char(186) << left << setw(25) << restaurant
             << right << setw(15) << fixed << setprecision(2) << earnings
             << char(186) << "\n";
        row++;
    }
    setColor(7);
    printBottomBorder(width);
}

 // For SetConsoleTextAttribute







// ======= Restaurant Performance Ranking =======
void viewRestaurantPerformance() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole,14); // Gray
    int width = 80;
    printTopBorder(width);
    cout << char(186) << setw(width/2 + 16) << right << " RESTAURANT PERFORMANCE " << setw(width/2 - 17) << "" << char(186) << "\n";
    printSeparator(width);
    // Reset

    // Collect data
    map<string, pair<double,double>> restaurantStats;
    map<string,int> orderCount, reviewCount;
    map<string,double> totalRatings;

    ifstream payFile("payments.txt");
    string line;
    while(getline(payFile,line)){
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;
        getline(ss, orderId, ','); getline(ss, customer, ','); getline(ss, restaurant, ',');
        getline(ss, totalStr, ','); getline(ss, adminStr, ','); getline(ss, ownerStr, ',');
        getline(ss, date);
        double total = stod(totalStr);
        restaurantStats[restaurant].first += total;
        orderCount[restaurant]++;
    }
    payFile.close();

    ifstream revFile("reviews.txt");
    while(getline(revFile,line)){
        stringstream ss(line);
        string restaurant, food, user, ratingStr, comment;
        getline(ss, restaurant, ','); getline(ss, food, ','); getline(ss, user, ',');
        getline(ss, ratingStr, ','); getline(ss, comment);
        double rating = stod(ratingStr);
        totalRatings[restaurant] += rating;
        reviewCount[restaurant]++;
    }
    revFile.close();

    for(auto& [r,total]: totalRatings){
        int count = reviewCount[r];
        restaurantStats[r].second = count>0 ? total/count : 0;
    }
    SetConsoleTextAttribute(hConsole,6);

    cout << char(186) << left << setw(3) << "No"
         << left << setw(25) << "Restaurant"
         << right << setw(8) << "Orders"
         << right << setw(15) << "Revenue"
         << right << setw(12) << "Rating"
         << right << setw(15) << "Performance" << char(186) << "\n";
    printSeparator(width);
 SetConsoleTextAttribute(hConsole,7);
    vector<tuple<string,int,double,double>> rankings;
    for(auto& [r,stats]: restaurantStats){
        double performance = (orderCount[r]*0.3) + (stats.first*0.0001) + (stats.second*20);
        rankings.push_back({r, orderCount[r], stats.first, performance});
    }
    sort(rankings.begin(),rankings.end(),[](auto &a, auto &b){return get<3>(a) > get<3>(b);});

    int rank=1,row=0;
    for(auto& [restaurant, orders, revenue, performance]: rankings){
        double rating = restaurantStats[restaurant].second;
        SetConsoleTextAttribute(hConsole,15); // Gray or default for rows
        cout << char(186)
             << left << setw(3) << rank++
             << left << setw(25) << (restaurant.length()>22 ? restaurant.substr(0,22)+"..." : restaurant)
             << right << setw(8) << orders
             << right << setw(15) << fixed << setprecision(2) << revenue
             << right << setw(12) << fixed << setprecision(1) << rating
             << right << setw(15) << fixed << setprecision(2) << performance
             << char(186) << "\n";
        row++;
    }
    SetConsoleTextAttribute(hConsole,7);
    printBottomBorder(width);
}

string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

void analyzeCustomerBehavior() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    map<string, vector<string>> customerPreferences;
    map<string, double> customerSpending;
    map<string, int> customerOrderCount;

    ifstream orderFile("order_history.txt");
    string line;

    while (getline(orderFile, line)) {

        // Skip blank lines
        if (trim(line).empty()) continue;

        stringstream ss(line);
        string restaurant, customer, location, item, qtyStr, priceStr;

        getline(ss, restaurant, ',');
        getline(ss, customer, ',');
        getline(ss, location, ',');
        getline(ss, item, ',');
        getline(ss, qtyStr, ',');
        getline(ss, priceStr, ',');

        restaurant = trim(restaurant);
        customer   = trim(customer);
        location   = trim(location);
        item       = trim(item);
        qtyStr     = trim(qtyStr);
        priceStr   = trim(priceStr);

        int qty = stoi(qtyStr);
        double price = stod(priceStr);

        customerPreferences[customer].push_back(restaurant);
        customerSpending[customer] += qty * price;
        customerOrderCount[customer]++;
    }
    orderFile.close();


    int width = 95;

    // Header
    SetConsoleTextAttribute(hConsole, 10);
    cout << string(width, '=') << "\n";
    cout << setw((width + 30) / 2) << right << "CUSTOMER BEHAVIOR ANALYSIS" << "\n";
    cout << string(width, '=') << "\n";

    // Column titles
    SetConsoleTextAttribute(hConsole, 10);
    cout << left << setw(15) << "Customer"
         << right << setw(8) << "Orders"
         << right << setw(15) << "Total Spend"
         << right << setw(12) << "Avg/Order"
         << left << setw(30) << " Restaurant"
         << "\n";
    cout << string(width, '-') << "\n";

    // Data rows
    SetConsoleTextAttribute(hConsole, 15);

    for (auto &p : customerPreferences) {
        string customer = p.first;
        auto &restaurants = p.second;

        int orders = customerOrderCount[customer];
        double spending = customerSpending[customer];
        double avgSpending = orders > 0 ? spending / orders : 0;

        // Find favorite restaurant
        map<string, int> freq;
        for (auto &r : restaurants) freq[r]++;

        string favorite = "None";
        int maxFreq = 0;

        for (auto &f : freq)
            if (f.second > maxFreq) {
                maxFreq = f.second;
                favorite = f.first;
            }

        cout << left << setw(15)
             << (customer.length() > 12 ? customer.substr(0, 12) + "..." : customer)
             << right << setw(8) << orders
             << right << setw(15) << fixed << setprecision(2) << spending
             << right << setw(12) << fixed << setprecision(2) << avgSpending
             << left << setw(30)
             << (favorite.length() > 28 ? favorite.substr(0, 28) + "..." : favorite)
             << "\n";
    }

    SetConsoleTextAttribute(hConsole, 7);
    cout << string(width, '=') << "\n";
}

// ======= System Health Check =======
void systemHealthCheck() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int width=70;
    printTopBorder(width);
    cout << char(186) << setw(width/2 + 10) << right << " SYSTEM HEALTH CHECK " << setw(width/2 - 10) << "" << char(186) << "\n";
    printSeparator(width);

    vector<string> essentialFiles={"owners.txt","customers.txt","menu.txt","payments.txt"};
    vector<string> optionalFiles={"reviews.txt","order_history.txt","pendingorders.txt"};

    cout << char(186) << " Essential Files: " << setw(width-21) << char(186) << "\n";
    for(auto& file: essentialFiles){
        ifstream f(file);
        SetConsoleTextAttribute(hConsole, f.good()?10:12); // green/red
        cout << char(186) << (f.good()?"  ":"  ") << file
             << setw(width-6-file.length()) << char(186) << "\n";
    }
    SetConsoleTextAttribute(hConsole,7);

    cout << char(186) << " Optional Files: " << setw(width-21) << char(186) << "\n";
    for(auto& file: optionalFiles){
        ifstream f(file);
        SetConsoleTextAttribute(hConsole, f.good()?10:12);
        cout << char(186) << (f.good()?"  ":"  ") << file
             << setw(width-6-file.length()) << char(186) << "\n";
    }
    SetConsoleTextAttribute(hConsole,7);
    printBottomBorder(width);
}

// ======= Create Backup =======
void createBackup() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    string timestamp=to_string(time(0));
    string backupFolder="backup_"+timestamp;
    system(("mkdir "+backupFolder).c_str());

    vector<string> filesToBackup={"owners.txt","customers.txt","menu.txt","payments.txt","reviews.txt","order_history.txt"};

    int width=60;
    printTopBorder(width);
    cout << char(186) << setw(width/2 + 8) << right << " CREATING BACKUP " << setw(width/2 - 9) << "" << char(186) << "\n";
    printSeparator(width);

    for(auto& file: filesToBackup){
        ifstream src(file, ios::binary);
        ofstream dst(backupFolder+"/"+file, ios::binary);
        dst << src.rdbuf();
        SetConsoleTextAttribute(hConsole,10); // Green
        cout << char(186) << "  " << file << " backed up" << setw(width-6-file.length()) << char(186) << "\n";
    }
    SetConsoleTextAttribute(hConsole,7);
    printSeparator(width);
    cout << char(186) << " Backup folder: " << backupFolder << setw(width-21-backupFolder.length()) << char(186) << "\n";
    printBottomBorder(width);
}

void listBackups() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // 🌿 Light-Green Box Heading
    SetConsoleTextAttribute(h, 10);
    cout << "\n==============================================\n";
    cout << "               BACKUP LIST\n";
    cout << "==============================================\n";
    SetConsoleTextAttribute(h, 7);

    cout << "\nAvailable Backups:\n";
    system("dir /b backup_*");
}


void viewPopularItems(Owner &owner) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    map<string, int> itemSales;
    ifstream fin("order_history.txt");
    string line;

    while (getline(fin, line)) {
        vector<string> parts;
        string temp = "";

        // Split by comma manually
        for (char c : line) {
            if (c == ',') {
                parts.push_back(temp);
                temp = "";
            } else {
                temp += c;
            }
        }
        parts.push_back(temp);

        // If line doesn't have minimum 5 fields → skip
        if (parts.size() < 5) continue;

        string restaurant = parts[0];
        string item = parts[3];
        string qtyStr = parts[4];

        // If qty contains non-digit → skip safely
        if (!isdigit(qtyStr[0])) continue;

        int qty = stoi(qtyStr);

        if (restaurant == owner.restaurantName) {
            itemSales[item] += qty;
        }
    }
    fin.close();
    SetConsoleTextAttribute(h,14);
    cout << "\n==============================================\n";
    cout << "\n    Most Popular Items    \n";
cout << "\n==============================================\n";
    vector<pair<int, string>> sortedItems;
    for (auto &p : itemSales) {
        sortedItems.push_back({p.second, p.first});
    }

    sort(sortedItems.rbegin(), sortedItems.rend());
    SetConsoleTextAttribute(h,11);
    for (int i = 0; i < min(5, (int)sortedItems.size()); i++) {
        cout << i+1 << ". " << sortedItems[i].second
             << " - " << sortedItems[i].first << " sold\n";
    }
        SetConsoleTextAttribute(h,14);
}




void checkLowStock(Owner &owner) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h,14);
        cout << "\n==============================================\n";
    cout << "\n       Low Stock Alert     \n";
    cout << "\n==============================================\n";
    MenuItem* temp = owner.menuHead;
    bool lowStockFound = false;
       SetConsoleTextAttribute(h,7);
    while (temp != nullptr) {
        if (temp->quantity < 10) { // Threshold: 10 items
            cout << temp->name << " - Only " << temp->quantity << " left!\n";
            lowStockFound = true;
        }
        temp = temp->next;
    }

    if (!lowStockFound) {
                   SetConsoleTextAttribute(h,3);
        cout << "All items have sufficient stock.\n";
    }
       SetConsoleTextAttribute(h, 7);
}
void viewFeedbackSummary(Owner &owner) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream fin("reviews.txt");
    string line;
    double totalRating = 0;
    int reviewCount = 0;
    map<int, int> ratingDistribution; // 1-5 stars

    while (getline(fin, line)) {
        stringstream ss(line);
        string restaurant, food, user, ratingStr, comment;

        getline(ss, restaurant, ',');
        getline(ss, food, ',');
        getline(ss, user, ',');
        getline(ss, ratingStr, ',');
        getline(ss, comment);

        if (restaurant == owner.restaurantName) {
            int rating = stoi(ratingStr);
            totalRating += rating;
            reviewCount++;
            ratingDistribution[rating]++;
        }
    }
    fin.close();
       SetConsoleTextAttribute(h,14);
       cout << "\n==============================================\n";
    cout << "\n        Feedback Summary     \n";
    cout << "\n==============================================\n";
           SetConsoleTextAttribute(h,7);
    cout << "Total Reviews: " << reviewCount << "\n";
    if (reviewCount > 0) {
        cout << "Average Rating: " << fixed << setprecision(1)
             << totalRating/reviewCount << "/5\n";
       SetConsoleTextAttribute(h,15);
        cout << "Rating Distribution:\n";
        for (int i = 5; i >= 1; i--) {
            cout << i << " stars: " << ratingDistribution[i] << " reviews\n";
        }
    }
}

void viewMonthlyEarnings(Owner &owner) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
               SetConsoleTextAttribute(h,7);
    cout << "Enter month and year (MM-YYYY): ";
    string monthYear;
    cin >> monthYear;

    double monthlyEarnings = 0;
    ifstream fin("payments.txt");
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string orderId, customer, restaurant, totalStr, adminStr, ownerStr, date;

        getline(ss, orderId, ',');
        getline(ss, customer, ',');
        getline(ss, restaurant, ',');
        getline(ss, totalStr, ',');
        getline(ss, adminStr, ',');
        getline(ss, ownerStr, ',');
        getline(ss, date);

        string orderMonthYear = date.substr(5, 2) + "-" + date.substr(0, 4);

        if (restaurant == owner.restaurantName && orderMonthYear == monthYear) {
            monthlyEarnings += stod(ownerStr);
        }
    }
    fin.close();
       SetConsoleTextAttribute(h,11);
       cout << "\n==============================================\n";
    cout << "Monthly Earnings for " << monthYear << ": " << monthlyEarnings << " TK\n";
    cout << "\n==============================================\n";
       SetConsoleTextAttribute(h, 7);
}


void AdminDashboard() {
    if (!adminLogin()) return;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Light Sky Blue Header
    SetConsoleTextAttribute(h, 11);
    cout << "\n===== Admin Dashboard =====\n";
    SetConsoleTextAttribute(h, 7);

    int choice;
    do {
        cout << "1. View Pending Requests\n";
        cout << "2. Approve/Reject Food Requests\n";
        cout << "3. Show All Foods\n";
        cout << "4. Show Owners\n";
        cout << "5. Show Customers\n";
        cout << "6. View Payment Records\n";        // NEW
        cout << "7. View Total Earnings\n";
        cout << "8. Sales Report\n";                    // NEW
        cout << "9. Daily Report\n";                   // NEW
        cout << "10. Restaurant Performance\n";        // NEW
        cout << "11. Customer Analysis\n";             // NEW
        cout << "12. System Health Check\n";           // NEW
        cout << "13. Create Backup\n";                 // NEW
        cout << "14. manageDeliveryPoints\n";             // NEW
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
          case 1: {
    ifstream fin("requests.txt");
    if (!fin) {
        cout << "No pending requests found.\n";
        break;
    }

    string line;
    bool anyRequest = false;
        SetConsoleTextAttribute(h, 14);
    cout << "\n==============================================\n";
    cout << "       Pending Requests \n";
    cout << "\n==============================================\n";
    SetConsoleTextAttribute(h, 7);
    string rest, action, item;
    double price, discount;
    int qty;

    while (getline(fin, line)) {

        if (line.find("[Restaurant:") != string::npos) {
            anyRequest = true;

            // Restaurant
            rest = line.substr(line.find(":") + 2, line.find("]") - line.find(":") - 2);

            // Action
            getline(fin, line);
            action = line.substr(line.find(":") + 2);

            // Item
            getline(fin, line);
            item = line.substr(line.find(":") + 2);

            // Price
            getline(fin, line);
            price = stod(line.substr(line.find(":") + 2));

            // Quantity
            getline(fin, line);
            qty = stoi(line.substr(line.find(":") + 2));

            // Discount
            getline(fin, line);
            discount = stod(line.substr(line.find(":") + 2));

            // Status line
            getline(fin, line);

            // --------------------------- OUTPUT ---------------------------
            cout << "---------------------------------------------\n";
            cout << "Restaurant : " << rest << "\n";
            cout << "Action     : " << action << "\n";
            cout << "Item       : " << item << "\n";
            cout << "Price      : " << price << "\n";
            cout << "Quantity   : " << qty << "\n";
            cout << "Discount   : " << discount << "%\n";
            cout << "Status     : PENDING\n";
        }
    }

    if (!anyRequest)
        cout << "No pending requests found.\n";
    SetConsoleTextAttribute(h, 3);
    cout << "---------------------------------------------\n";
    SetConsoleTextAttribute(h, 7);
    fin.close();
    break;
}

            case 2: accept_proposal(); break;
            case 3: viewFoodItems(); break;
            case 4: showTotalOwners(); break;
            case 5: showTotalCustomers(); break;
            case 6: viewAllPaymentRecords(); break;  // NEW
            case 7: {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    double totalEarnings = getAdminTotalEarnings();

    const int boxWidth = 45; // Box width

    // Top border - Bright White
    SetConsoleTextAttribute(hConsole, 15); // Bright White
    cout << "\n+" << string(boxWidth,'=') << "+\n";

    // Headline centered - Bright White
    string headline = "ADMIN TOTAL EARNINGS";
    int padding = (boxWidth - headline.length()) / 2;
    cout << "|" << string(padding, ' ') << headline << string(boxWidth - headline.length() - padding, ' ') << "|\n";

    // Bottom border of headline
    cout << "+" << string(boxWidth,'=') << "+\n";

    // Earnings info - Bright Green
    SetConsoleTextAttribute(hConsole, 10); // Bright Green
    cout << "| Total Earnings: " << setw(boxWidth - 17) << fixed << setprecision(2) << totalEarnings << " TK |\n";
    cout << "| From all orders: 20% commission" << string(boxWidth - 34,' ') << "|\n";

    // Bottom border
    SetConsoleTextAttribute(hConsole, 15); // Bright White
    cout << "+" << string(boxWidth,'=') << "+\n";

    // Reset color
    SetConsoleTextAttribute(hConsole, 7);

    break;
}

            case 8: generateSalesReport(); break;         // NEW
            case 9: generateDailyReport(); break;         // NEW
            case 10: viewRestaurantPerformance(); break;  // NEW
            case 11: analyzeCustomerBehavior(); break;    // NEW
            case 12: systemHealthCheck(); break;          // NEW
            case 13: createBackup(); break;               // NEW
              case 14: manageDeliveryPoints(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 0);
}

// ---------------- Owner ----------------
void OwnerDashboard() {
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

SetConsoleTextAttribute(h, 11);
cout << "\n--- Owner Panel ---\n";
SetConsoleTextAttribute(h, 7);

cout << "Enter username: ";
string user, pass;
cin >> user;

cout << "Enter password: ";
pass = gethiddenPassword(); // <-- Hidden password here

cout << "\n";

    for (int i = 0; i < MAX_OWNERS; i++) {
        if (owners[i].username == user && owners[i].password == pass) {
                       SetConsoleTextAttribute(h, 14);
                           cout << "\n==============================================\n";
            cout << "\n           Welcome, Owner of " << owners[i].restaurantName << "!\n";
                cout << "\n==============================================\n";

            ifstream fin("pendingorders.txt");
            string line;
            bool hasPending = false;
            while (getline(fin, line)) {
                stringstream ss(line);
                string restaurant;
                getline(ss, restaurant, ',');
                if (restaurant == owners[i].restaurantName) {
                    hasPending = true;
                    break;
                }
            }
            fin.close();

            if (hasPending) {
                SetConsoleTextAttribute(h, 11);
                cout << "\n----You have new pending orders!----\n";
            }

            int ch;
            do {                SetConsoleTextAttribute(h, 7);
                cout << "\n1. View Menu\n2. Request Add Item to Admin\n3. Request Delete Item to Admin\n4. Request Edit Item to Admin\n5. Request Discount to Admin\n6. View Request History\n7. View Pending Order from customer\n8. Set Food Preparation Time\n9. View All History Orders from customer\n10. Restock Item\n11. View Reviews\n12. View My Earnings\n13. View Popular Items\n14. Check Low Stock\n15. Revenue Report\n16. Monthly Earnings\n0. Logout\n";
                SetConsoleTextAttribute(h, 15); cout<<"\nChoice: ";
                cin >> ch;
                cin.ignore(); // flush newline

                if (ch == 1) {
                    showMenu(owners[i].restaurantName);
                }
                else if (ch == 2) {
                          HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                     SetConsoleTextAttribute(h, 7);
                    string item;
                    double p;
                    int q;
                    cout << "Item name: "; getline(cin, item);
                    cout << "Price: "; cin >> p;
                    cout << "Quantity: "; cin >> q;
                    cin.ignore();
                    addRequest(owners[i].restaurantName, "ADD", item, p, q, 0);
                       SetConsoleTextAttribute(h, 3);
                    cout << "Add request sent successfully!\n";
                }
                else if (ch == 3) {
                         SetConsoleTextAttribute(h, 7);
                    int code;
                    cout << "Item code to delete: "; cin >> code; cin.ignore();

                    string n;
                    double p;
                    int q;

                    if (!getItem(owners[i].restaurantName, code, n, p, q)) {
                        cout << "There is no food of this code.\n";
                    } else {
                        addRequest(owners[i].restaurantName, "DELETE", n, p, q, 0);
                        SetConsoleTextAttribute(h, 3);
                        cout << "Delete request sent successfully!\n";
                    }
                }
                else if (ch == 4) {
                        SetConsoleTextAttribute(h,7);
                    int code;
                    cout << "Item code to edit: "; cin >> code; cin.ignore();

                    string oldName;
                    double oldPrice;
                    int oldQty;

                    if (!getItem(owners[i].restaurantName, code, oldName, oldPrice, oldQty)) {
                        cout << "There is no food of this code.\n";
                    } else {
                        SetConsoleTextAttribute(h,6);
                        cout << "Found item:\n";
                        cout << "Name: " << oldName << "\nPrice: " << oldPrice << "\nQuantity: " << oldQty << "\n";

                        double newPrice;
                        int newQty;
                        cout << "New price: "; cin >> newPrice;
                        cout << "New quantity: "; cin >> newQty;
                        cin.ignore();

                        addRequest(owners[i].restaurantName, "EDIT", oldName, newPrice, newQty, 0);
                        SetConsoleTextAttribute(h,3);
                        cout << "Edit request sent successfully!\n";
                        SetConsoleTextAttribute(h, 7);
                    }
                }
                else if (ch == 5) {
                    requestDiscount(owners[i]);
                }
                else if (ch == 6) {
                    viewRequestHistory(owners[i].restaurantName);
                }

                else if (ch == 7) {
                    viewPendingOrders(owners[i].restaurantName);
                }
                else if (ch == 8) {
                    setPreparationTime(owners[i]);
                }
                else if (ch == 9) {
                    viewAllPreviousOrders(owners[i].restaurantName);
                }
                else if (ch == 10) {
                    updateItemStock(owners[i].restaurantName);
                }
                else if (ch == 11) {
                    ViewReviews(owners[i]);
                }
       else if (ch == 12) {  // Owner chooses to view earnings
    viewOwnerPaymentRecords(owners[i].restaurantName);
}              else if (ch == 13) viewPopularItems(owners[i]);

             else if (ch == 14) checkLowStock(owners[i]);
            else if (ch == 15) viewFeedbackSummary(owners[i]);
              else if (ch == 16) viewMonthlyEarnings(owners[i]);
            } while (ch != 0);
            return;
        }
    }
    cout << "Invalid login!\n";
}

void CustomerDashboard() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 7);
    Customer c;
    bool loggedIn = false;  // track login status

    while (true) {
            SetConsoleTextAttribute(h, 14);
        cout << "\n===== FoodFly Customer Panel =====\n\n";
    SetConsoleTextAttribute(h, 7);
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. View Restaurants & Menu\n";
        cout << "4. Search Food Item\n";
        cout << "5. Search Food By Range\n";

        cout << "6. Add Item to Cart\n";
        cout << "7. View Cart\n";
        cout << "8. Remove Item from Cart\n";
        cout << "9. Order Confirmation\n";
        cout << "10. View Estimated Delivery Time (Seconds)\n"; // নতুন option        // ✔ NEW
        cout << "11. My Order History\n";         // ✔ NEW
        cout << "12. Edit Profile\n";

        cout << "13. Submit Review\n";   // নতুন
        cout << "14. View Reviews\n";    // নতুন


        cout << "0. Back to Main Menu\n";
        cout << "Choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            c = registerCustomer();
        }
        else if (choice == 2) {
            if (loginCustomer(c)) {
                cout << " Login successful! Welcome " << c.username << "\n";
                loggedIn = true;
            } else {
                cout << " Invalid username/password! Returning to main menu...\n";
                break;
            }
        }
        else if (choice == 3) {
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            viewRestaurants();

            int rid; cin >> rid;
            viewRestaurantMenu(rid);
        }


        else if (choice == 4) {
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            cin.ignore();
            cout << "Enter food name to search: ";
            string query; getline(cin, query);
            searchFoodAll(query);

        }
        else if (choice == 5) {
           int minP, maxP;
           cout << "Enter minimum price: ";
           cin >> minP;
           cout << "Enter maximum price: ";
           cin >> maxP;
           searchFoodByRange(minP, maxP);
       }



        else if (choice == 6) {
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            viewRestaurants();
            int rid; cin >> rid;
            viewRestaurantMenu(rid);
            cout << "Enter serial number of item to add: ";
            int s; cin >> s;
            cout << "Enter quantity: ";
            int q; cin >> q;
            addToCart(rid, s, q);
        }
        else if (choice == 7) {
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            viewCart();
        }
        else if (choice == 8) {  // NEW: Remove from cart
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            removeFromCart();
        }
        else if (choice == 9) {   // ✔ CHECKOUT ADDED
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            checkout_delayed(c,cart);
        }
        else if (choice == 10) {  // নতুন option
            if(!loggedIn) { cout << " Please login first!\n"; continue; }
            showEstimatedDeliveryTime(c);
        }

        else if (choice == 11) {   // ✔ ORDER HISTORY ADDED
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            viewCustomerOrders(c.username);
        }

        else if (choice == 12) {   // ✔ EDIT PROFILE ADDED
            if (!loggedIn) { cout << " Please login first!\n"; continue; }
            editProfile(c);
        }

        else if (choice == 13) {
            if(!loggedIn) { cout << " Please login first!\n"; continue; }
            submitReview(c);
        }
        else if (choice == 14) {
            if(!loggedIn) { cout << " Please login first!\n"; continue; }
             viewReviews();
        }




        else if (choice == 0) {
            cout << "Returning to main menu...\n";
            break;
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
}

int showRoleMenu() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Center the heading (approximate spacing)
    cout << "\n\n";

    // === 1. Big Yellow Centered Heading ===
    SetConsoleTextAttribute(h, 14); // Yellow
    cout << "                 ==== WELCOME TO FOODFLY ====\n";

    // === 2. Orange (Light Red) Sub-heading ===
    SetConsoleTextAttribute(h, 11); // Light Red (close to orange)
    cout << "Who are you?\n";

    // Reset color to white
    SetConsoleTextAttribute(h, 7);

    cout << "1. Admin\n";
    cout << "2. Restaurant Owner\n";
    cout << "3. Customer\n";
    cout << "0. Exit\n";
     SetConsoleTextAttribute(h, 15);
    cout << "Enter your role: ";

    int c;
    cin >> c;
    return c;
    SetConsoleTextAttribute(h, 7);
}

int main() {
    firstTimeSetup();
    loadOwnersFromFile();
    loadMenuFromFile();

    while (true) {
        int role = showRoleMenu();
        if (role == 1) AdminDashboard();
        else if (role == 2) OwnerDashboard();
        else if (role == 3) CustomerDashboard();
        else if (role == 0) {
            cout << "Exiting FoodFly. Goodbye!\n";
            break;
        }
        else cout << "Invalid choice!\n";
    }
    return 0;
}
