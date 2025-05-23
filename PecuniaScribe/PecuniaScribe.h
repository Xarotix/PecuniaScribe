#ifndef PECUNIA_SCRIBE_H
#define PECUNIA_SCRIBE_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>      // For std::fixed and std::setprecision
#include <limits>       // For std::numeric_limits
#include <algorithm>    // For std::transform
#include <fstream>      // For file input/output
#include <sstream>      // For string stream parsing
#include <cstdlib>      // For system()

// Required for SetConsoleTitleA and other Windows-specific functions
#ifdef _WIN32
// Define NOMINMAX to prevent Windows headers from defining min and max macros,
// which can conflict with std::numeric_limits::max() and std::min/max.
#define NOMINMAX 
#include <windows.h>
#endif

// --- Constants ---

// ANSI Escape Codes for Colors
const std::string RESET_COLOR = "\033[0m";
const std::string RED_TEXT = "\033[31m";
const std::string GREEN_TEXT = "\033[32m";
const std::string YELLOW_TEXT = "\033[33m";
const std::string BLUE_TEXT = "\033[34m";
const std::string MAGENTA_TEXT = "\033[35m";
const std::string CYAN_TEXT = "\033[36m";
const std::string WHITE_TEXT = "\033[37m";
const std::string BRIGHT_RED_TEXT = "\033[91m";
const std::string BRIGHT_YELLOW_TEXT = "\033[93m";

// Application Version and File Configuration
const std::string APP_VERSION = "v1.1.2"; // Updated version for clear screen feature
const std::string FILENAME = "bills.dat";
const char ENCRYPTION_KEY = 'K'; // A simple XOR encryption key

// --- Structure Definition ---
struct Bill {
    std::string title;
    std::string dueDate;
    double amount;
    bool isCrucial;
    int id; // Unique ID for easier removal
};

// --- Global Variable Declarations (defined in PecuniaScribe_lib.cpp) ---
extern std::vector<Bill> bills;
extern int nextBillId;

// --- Function Declarations ---

// System Utilities
void clearScreen();

// Encryption/Decryption
std::string encryptDecrypt(const std::string& data, char key);

// File Operations
void saveBillsToFile(const std::string& filename);
void loadBillsFromFile(const std::string& filename);

// UI and Input Handling
void displayMenu();
void clearInputBuffer();

// Bill Management
void addBill();
void viewBills();
void removeBill();

#endif // PECUNIA_SCRIBE_H
