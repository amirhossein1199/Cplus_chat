#include <iostream>
#include "User.h"

int main() {
    User currentUser;
    int choice;
    // Yek moteghayer baraye negahdari-e vaziat-e vorood-e karbar
    bool isLoggedIn = false;

    while (true) {
        if (!isLoggedIn) {
            // --- MENU-YE VOROOD NAKARDE (LOGGED OUT) ---
            std::cout << "\n--- Welcome to Messenger ---\n";
            std::cout << "1. Register a new account\n";
            std::cout << "2. Login\n";
            std::cout << "3. Exit\n";
            std::cout << "Please enter your choice: ";
            std::cin >> choice;
            std::cin.ignore(); // Baraye pishgiri az bug-e getline

            switch (choice) {
                case 1:
                    currentUser.registerUser();
                    break;
                case 2:
                    // natije-ye loginUser() ro dar isLoggedIn zakhire mikonim.
                    // Agar login movaffaghiat amiz bood, true mishe.
                    isLoggedIn = currentUser.loginUser();
                    break;
                case 3:
                    std::cout << "Goodbye!\n";
                    return 0;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } else {
            // --- MENU-YE VOROOD KARDE (LOGGED IN) ---
            std::cout << "\n--- Logged in as: " << currentUser.username << " ---\n";
            std::cout << "1. Show My Stats\n"; 
            std::cout << "2. Delete Account\n";
            std::cout << "3. Logout\n";
            std::cout << "4. Exit\n";
            std::cout << "Please enter your choice: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    currentUser.showStats(); 
                    break;
                case 2:
                    currentUser.deleteUser();
                    isLoggedIn = false;
                    break;
                case 3:
                    currentUser.logoutUser();
                    isLoggedIn = false;
                    break;
                case 4:
                    std::cout << "Goodbye!\n";
                    return 0;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
    }
    return 0;
}