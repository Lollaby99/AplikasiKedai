#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
using namespace std;

struct Stock {
    string name;
    int quantity;
    int price;
    int sold = 0; // Track sold quantity
    int used = 0; // Track used quantity
};

const int MAX_INVENTORY = 100; // Maximum number of items in the inventory
Stock inventory[MAX_INVENTORY];
int inventoryCount = 0;
int dailySales = 0;
int dailyExpenses = 0;
const int MIN_STOCK_WARNING = 5;

// Clear screen function
void clearScreen() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/Unix
    #endif
}

// Sleep Functions
void delay(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000); // Convert seconds to milliseconds
#else
    sleep(seconds);
#endif
}

// Owner Functions
void inputStock() {
    clearScreen();
    if (inventoryCount >= MAX_INVENTORY) {
        cout << "Inventory is full! Cannot add more items.\n";
        delay(2);
        return;
    }

    string stockName;
    int quantity;
    int price;
    cout << "Enter stock name: ";
    cin >> stockName;
    transform(stockName.begin(), stockName.end(), stockName.begin(), ::tolower);

    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter price: ";
    cin >> price;

    bool found = false;

    // Check if the stock already exists
    for (int i = 0; i < inventoryCount; ++i) {
        if (inventory[i].name == stockName) {
            inventory[i].quantity += quantity; // Add quantity to the existing stock
            cout << "Stock updated successfully! New quantity: " << inventory[i].quantity << "\n";
            found = true;
            break;
        }
    }

    // If the stock does not exist, add it as a new entry
    if (!found) {
        Stock newStock;
        newStock.name = stockName;
        newStock.quantity = quantity;
        newStock.price = price;
        inventory[inventoryCount++] = newStock;
        cout << "Stock added successfully!\n";
    }
    delay(2);
}


void checkStockWarning() {
    clearScreen();
    cout << "\nStocks with low inventory:\n";
    for (int i = 0; i < inventoryCount; ++i) {
        if (inventory[i].quantity < MIN_STOCK_WARNING) {
            cout << inventory[i].name << " - Quantity: " << inventory[i].quantity << "\n";
        }
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void viewDailyExpenses() {
    clearScreen();
    cout << fixed << setprecision(2);
    cout << "\nDaily Expenses Summary:\n";
    for (int i = 0; i < inventoryCount; ++i) {
        if (inventory[i].used > 0) {
            cout << "Item: " << inventory[i].name << " - Quantity Used: " << inventory[i].used << "\n";
        }
    }
    cout << "Total Expenses: " << dailyExpenses << "\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void viewSalesSummary() {
    clearScreen();
    cout << fixed << setprecision(2);
    cout << "\nDaily Sales Summary:\n";
    for (int i = 0; i < inventoryCount; ++i) {
        if (inventory[i].sold > 0) {
            cout << "Item: " << inventory[i].name << " - Quantity Sold: " << inventory[i].sold << "\n";
        }
    }
    cout << "Total Sales: Rp. " << dailySales << "\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// Staff Functions
void recordSale() {
    clearScreen();
    string itemName;
    int quantity;
    int totalPrice = 0;
    char confirm;

    do {
        clearScreen();
        cout << "Enter item name: ";
        cin >> itemName;
        transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
        cout << "Enter quantity sold: ";
        cin >> quantity;

        bool found = false;
        for (int i = 0; i < inventoryCount; ++i) {
            if (inventory[i].name == itemName) {
                if (inventory[i].quantity >= quantity) {
                    int price = inventory[i].price * quantity;
                    inventory[i].quantity -= quantity;
                    inventory[i].sold += quantity;
                    totalPrice += price;
                    cout << "Item added to sale! Subtotal: Rp. " << price << "\n";
                } else {
                    cout << "Insufficient stock for " << inventory[i].name << "!\n";
                }
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Item not found!\n";
        }

        cout << "\nDo you want to add another item to this sale? (y/n): ";
        cin >> confirm;

    } while (tolower(confirm) == 'y');

    dailySales += totalPrice; // Add total price to daily sales
    cout << fixed << setprecision(2);
    cout << "\nTotal price for this sale: Rp. " << totalPrice << "\n";
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void recordUsage() {
    clearScreen();
    string itemName;
    int quantity;

    cout << "Enter item name: ";
    cin >> itemName;
    transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
    cout << "Enter quantity used: ";
    cin >> quantity;

    bool found = false;
    for (int i = 0; i < inventoryCount; ++i) {
        if (inventory[i].name == itemName) {
            if (inventory[i].quantity >= quantity) {
                inventory[i].quantity -= quantity;
                inventory[i].used += quantity;
                dailyExpenses += quantity;
                cout << "Stock updated successfully!\n";
            } else {
                cout << "Insufficient stock!\n";
            }
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Item not found!\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void checkStock() {
    clearScreen();
    cout << "\nAvailable Stock:\n";
    for (int i = 0; i < inventoryCount; ++i) {
        cout << inventory[i].name << " - Quantity: " << inventory[i].quantity << "\n";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// Menu Functions
void ownerMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\nOwner Menu:\n";
        cout << "1. Input Stock\n";
        cout << "2. Check Stock Warnings\n";
        cout << "3. View Daily Expenses\n";
        cout << "4. View Sales Summary\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: inputStock(); break;
            case 2: checkStockWarning(); break;
            case 3: viewDailyExpenses(); break;
            case 4: viewSalesSummary(); break;
            case 5: cout << "Exiting Owner Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

void staffMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\nStaff Menu:\n";
        cout << "1. Record Sale\n";
        cout << "2. Record Expense\n";
        cout << "3. Check Stock\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: recordSale(); break;
            case 2: recordUsage(); break;
            case 3: checkStock(); break;
            case 4: cout << "Exiting Staff Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

int main() {
    int role;
    do {
        clearScreen();
        cout << "\nMain Menu:\n";
        cout << "1. Owner\n";
        cout << "2. Staff\n";
        cout << "3. Exit\n";
        cout << "Enter your role: ";
        cin >> role;

        switch (role) {
            case 1: ownerMenu(); break;
            case 2: staffMenu(); break;
            case 3: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (role != 3);

    return 0;
}
