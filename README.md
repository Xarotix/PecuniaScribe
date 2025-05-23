
# PecuniaScribe - Bill Management CLI

**Version:** 1.1.2 (as per the latest code updates)

## Description

PecuniaScribe is a simple command-line interface (CLI) application written in C++ designed to help you track your personal bills. It allows you to add, view, and remove bills, and stores your data persistently in an "encrypted" file.

## Features

-   **Add Bills:**
    
    -   Specify title, due date, and amount.
        
    -   Mark bills as "Crucial" or "Optional".
        
    -   Crucial bills are highlighted with an asterisk (`*`) in the list.
        
    -   Option to cancel bill addition.
        
-   **View Bills:**
    
    -   Lists all added bills with their details (ID, Title, Due Date, Amount, Status).
        
    -   Formatted table display.
        
-   **Remove Bills:**
    
    -   Remove bills by their unique ID.
        
    -   Option to cancel removal.
        
-   **Data Persistence:**
    
    -   Bill data is saved to a local file (`bills.dat`).
        
    -   The data in the file undergoes a simple XOR "encryption" (Note: This is for obfuscation, not strong security).
        
-   **Login System:**
    
    -   Basic username and password protection to access the application.
        
-   **User-Friendly Interface:**
    
    -   Color-coded output for better readability (requires a terminal that supports ANSI escape codes).
        
    -   Screen clearing after operations to reduce clutter.
        
    -   "Press Enter to continue..." prompts for better user flow.
        

## Requirements

-   A C++ compiler that supports C++11 or newer (e.g., G++, Clang, MSVC).
    
-   A terminal that supports ANSI escape codes for colors to display correctly (most modern terminals do, like Windows Terminal, PowerShell (newer versions), Linux terminals, macOS Terminal).
    

## How to Compile

The project is structured into three main files:

-   `PecuniaScribe.h` (Header file with declarations)
    
-   `PecuniaScribe_lib.cpp` (Source file with function definitions)
    
-   `main.cpp` (Source file with the main application logic)
    

To compile using G++ (ensure all three files are in the same directory):

```
g++ main.cpp PecuniaScribe_lib.cpp -o PecuniaScribe -std=c++17

```

-   Replace `-std=c++17` with `-std=c++11` or `-std=c++14` if needed, though C++17 is recommended.
    
-   This will create an executable named `PecuniaScribe` (or `PecuniaScribe.exe` on Windows).
    

## How to Run

Once compiled, execute the program from your terminal:

**On Linux/macOS:**

```
./PecuniaScribe

```

**On Windows:**

```
.\PecuniaScribe.exe

```

or simply:

```
PecuniaScribe

```

## Login Credentials

Upon starting the application, you will be prompted for a username and password:

-   **Username:**  `optic`
    
-   **Password:**  `1337`
    

You have 3 attempts to log in.

## Data Storage

-   Bill information is stored in a file named `bills.dat` in the same directory where the executable is run.
    
-   **Encryption Note:** The data in `bills.dat` is "encrypted" using a simple XOR cipher with a hardcoded key. This is **not** cryptographically secure and is only intended to deter casual viewing of the file's contents. Do not rely on this for protecting highly sensitive information.
    
-   The pipe character (`|`) is used as a delimiter in the data file. Therefore, bill titles and due dates **should** not **contain the `|` character**.
    

## Known Limitations / Potential Future Improvements

-   **Basic Encryption:** The current XOR encryption is very weak. For real security, a proper encryption library should be used.
    
-   **No Bill Editing:** Currently, you cannot edit an existing bill; you must remove and re-add it.
    
-   **Simple Date Handling:** Due dates are stored as strings and not validated or used for sorting/reminders.
    
-   **Error Handling:** While basic input validation exists, error handling for file operations and data parsing could be more robust.
    
-   **Single User:** Designed for a single user with hardcoded credentials.
    
-   **No Data Backup/Recovery:** If `bills.dat` is corrupted or deleted, data is lost.
    

Created by: Xarotix
