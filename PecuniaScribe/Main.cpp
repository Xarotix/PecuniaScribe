#include "PecuniaScribe.h" // Includes all necessary headers and declarations

int main() {
#ifdef _WIN32
    // Set console title for Windows
    SetConsoleTitleA(("PecuniaScribe " + APP_VERSION).c_str());

    // Enable ANSI escape codes for colors on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

    // --- Login System ---
    std::string username, password;
    int loginAttempts = 0;
    const int MAX_LOGIN_ATTEMPTS = 3;
    bool loggedIn = false;

    std::cout << MAGENTA_TEXT << "Welcome to PecuniaScribe (" << APP_VERSION << ")!" << RESET_COLOR << std::endl;
    std::cout << YELLOW_TEXT << "Please log in to continue." << RESET_COLOR << std::endl;

    while (loginAttempts < MAX_LOGIN_ATTEMPTS && !loggedIn) {
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        // Clear buffer after reading password.
        // This is important if the next operation after login is a getline,
        // or to ensure the menu choice input is clean.
        clearInputBuffer(); // Use the function from PecuniaScribe.h

        if (username == "optic" && password == "1337") {
            loggedIn = true;
            std::cout << GREEN_TEXT << "Login successful!" << RESET_COLOR << std::endl;
        }
        else {
            loginAttempts++;
            std::cout << RED_TEXT << "Invalid username or password. ";
            if (MAX_LOGIN_ATTEMPTS - loginAttempts > 0) {
                std::cout << (MAX_LOGIN_ATTEMPTS - loginAttempts) << " attempts remaining." << RESET_COLOR << std::endl;
            }
            else {
                std::cout << "No attempts remaining." << RESET_COLOR << std::endl;
            }

            if (loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                std::cout << RED_TEXT << "Maximum login attempts reached. Exiting." << RESET_COLOR << std::endl;
                return 1;
            }
        }
    }

    if (!loggedIn) {
        return 1; // Should ideally not be reached if MAX_LOGIN_ATTEMPTS logic is correct
    }
    // --- End Login System ---

    loadBillsFromFile(FILENAME); // Load bills after successful login

    int choice;
    do {
        displayMenu();
        while (!(std::cin >> choice)) {
            std::cout << RED_TEXT << "Invalid input. Please enter a number (1-4): " << RESET_COLOR;
            std::cin.clear();
            clearInputBuffer();
        }
        clearInputBuffer(); // CRUCIAL: Clear newline after reading choice, before next potential getline in addBill

        switch (choice) {
        case 1:
            addBill();
            break;
        case 2:
            viewBills();
            break;
        case 3:
            removeBill();
            break;
        case 4:
            std::cout << GREEN_TEXT << "Exiting PecuniaScribe. Your data is saved. Goodbye!" << RESET_COLOR << std::endl;
            break;
        default:
            std::cout << RED_TEXT << "Invalid choice. Please try again." << RESET_COLOR << std::endl;
        }
    } while (choice != 4);

    return 0;
}
