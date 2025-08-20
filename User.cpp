// User.cpp

#include "User.h"
#include <iostream>
#include <ctime>
#include <fstream> // Baraye kar kardan ba file-ha (khundan va neveshtan)
#include "json.hpp" // Ketabkhane-ye JSON ke ezafe kardim
#include <ctime> // Baraye tabdil-e timestamp be tarikh-e khana


// Baraye kar-e rahat-tar, yek nam-e mosta'ar baraye nlohmann::json misazim
using json = nlohmann::json;

User::User() {
    id = -1;
    username = "";
    passwordHash = "";
    creationTimestamp = 0;
}

#include "picosha2.h" // Ketabkhane-ye jadid baraye hashing


void User::registerUser() {

    std::cout << "Please enter a username: ";
    std::getline(std::cin >> std::ws, this->username);

    json allUsersJson;
    std::ifstream userFile_read("users.json");
    if (userFile_read.is_open()) {
        if (userFile_read.peek() != std::ifstream::traits_type::eof()) {
            userFile_read >> allUsersJson;
        }
        userFile_read.close();
    }
    
    if (!allUsersJson.is_array()) {
        allUsersJson = json::array();
    }

    for (const auto& user : allUsersJson) {
        if (user["username"] == this->username) {
            std::cout << "Error: This username is already taken. Please try another one.\n";
            return;
        }
    }

    std::cout << "Please enter a password: ";
    std::string password;
    std::getline(std::cin, password);

    // --- GHESMAT-E JADID: HASH KARDAN-E PASSWORD ---
    // Yek reshte (string) khali baraye negahdari-e natije-ye hash misazim.
    std::string hash_hex_str;
    // Tabe'e hash256_hex_string az ketabkhane-ye picosha2, password-e voroodi ro migire
    // va natije-ye hash ro dar moteghayer-e hash_hex_str mirize.
    picosha2::hash256_hex_string(password, hash_hex_str);

    // Be jaye zakhire-ye password-e sade, natije-ye HASH SHODE ro zakhire mikonim.
    this->passwordHash = hash_hex_str;
    // ----------------------------------------------

    this->creationTimestamp = time(0);
    this->id = this->creationTimestamp % 1000;

    json newUserJson;
    newUserJson["id"] = this->id;
    newUserJson["username"] = this->username;
    newUserJson["passwordHash"] = this->passwordHash; // Inja meghdar-e hash shode zakhire mishe
    newUserJson["creationTimestamp"] = this->creationTimestamp;

    allUsersJson.push_back(newUserJson);

    std::ofstream userFile_write("users.json");
    userFile_write << allUsersJson.dump(4);
    userFile_write.close();

    std::cout << "User '" << this->username << "' registered successfully!\n";
}

// --- Piasazi-e Tabe'e loginUser ---
bool User::loginUser() {
    // 1. Gereftan-e etela'at az karbar
    std::cout << "Please enter your username: ";
    std::getline(std::cin >> std::ws, this->username);

    // --- GHESMAT-E JA OFTADE ---
    // In bakhsh dar code-e ghabli ja oftade bood
    std::cout << "Please enter your password: ";
    std::string password;
    std::getline(std::cin, password);
    // ----------------------------

    // 2. Khundan-e file-e users.json
    json allUsersJson;
    std::ifstream userFile_read("users.json");
    if (!userFile_read.is_open()) {
        std::cout << "Error: Could not open user database.\n";
        return false;
    }
    if (userFile_read.peek() != std::ifstream::traits_type::eof()) {
        userFile_read >> allUsersJson;
    }
    userFile_read.close();
    
    // --- GHESMAT-E JA OFTADE ---
    // Hash kardan-e password-e vared shode
    std::string inputPasswordHash;
    picosha2::hash256_hex_string(password, inputPasswordHash);
    // ----------------------------

    // 3. Jostojoo baraye peyda kardan-e index-e karbar
    int userIndex = -1;
    for (int i = 0; i < allUsersJson.size(); ++i) {
        if (allUsersJson[i]["username"] == this->username && allUsersJson[i]["passwordHash"] == inputPasswordHash) {
            userIndex = i;
            break;
        }
    }

    // 4. Barresi-e natije
    if (userIndex != -1) {
        // Agar karbar peyda shod:
        auto& user = allUsersJson[userIndex];
        
        std::cout << "Welcome, " << user["username"].get<std::string>() << "!\n";

        // Zakhire-ye etela'at dar object-e feli
        this->id = user["id"];
        this->passwordHash = user["passwordHash"];
        this->creationTimestamp = user["creationTimestamp"];
        this->lastLoginTimestamp = time(0);

        // Update kardan-e file-e JSON ba zaman-e jadid-e vorood
        user["lastLoginTimestamp"] = this->lastLoginTimestamp;

        std::ofstream userFile_write("users.json");
        userFile_write << allUsersJson.dump(4);
        userFile_write.close();

        return true;
    } else {
        // Agar karbar peyda nashod:
        std::cout << "Login failed. Invalid username or password.\n";
        return false;
    }
}
// --- Piasazi-e Tabe'e deleteUser ---
void User::deleteUser() {
    // 1. Khundan-e hame-ye karbar-ha az file
    json allUsersJson;
    std::ifstream userFile_read("users.json");
    if (userFile_read.is_open()) {
        if (userFile_read.peek() != std::ifstream::traits_type::eof()) {
            userFile_read >> allUsersJson;
        }
        userFile_read.close();
    }

    if (!allUsersJson.is_array() || allUsersJson.empty()) {
        std::cout << "Error: No users found to delete.\n";
        return;
    }

    // 2. Sakhtan-e yek list-e jadid az karbar-ha (bedoone karbar-e feli)
    json updatedUsersJson = json::array();
    bool userFound = false;

    for (const auto& user : allUsersJson) {
        // Agar id-e karbar-e dakhel-e file, ba id-e karbari ke mikhaim hazf konim (this->id)
        // barabar NABOOD, oon ro be list-e jadid ezafe mikonim.
        if (user["id"] != this->id) {
            updatedUsersJson.push_back(user);
        } else {
            userFound = true;
        }
    }

    if (!userFound) {
        std::cout << "Error: Could not find your account to delete.\n";
        return;
    }

    // 3. Neveshtan-e list-e jadid dar file (overwrite kardan)
    std::ofstream userFile_write("users.json");
    userFile_write << updatedUsersJson.dump(4);
    userFile_write.close();

    std::cout << "Account '" << this->username << "' has been deleted successfully.\n";
}


// --- Piasazi-e Tabe'e logoutUser ---
void User::logoutUser() {
    std::cout << "You have been logged out.\n";
    // Dar inja code-e خاصی nist. asli-e kar taghir-e vaziat-e isLoggedIn dar main.cpp ast.
    // Dar ayande mitoonim inja etela'at-e currentUser ro reset konim.
    this->id = -1;
    this->username = "";
}

// --- Piasazi-e Tabe'e showStats ---
void User::showStats() {
    // time_t yek noe dade monaseb baraye timestamp ast
    time_t creation_time = this->creationTimestamp;
    time_t login_time = this->lastLoginTimestamp;

    // ctime tarikh ro be soorat-e khana barmigardoone ama dar entehash yek 'Enter' ezafi dare
    std::string creation_str = ctime(&creation_time);
    creation_str.pop_back(); // Hazf-e 'Enter'-e ezafi az akhar-e reshte

    std::string login_str = ctime(&login_time);
    login_str.pop_back(); // Hazf-e 'Enter'-e ezafi

    std::cout << "\n--- User Stats ---\n";
    std::cout << "Username: " << this->username << std::endl;
    std::cout << "Account Created On: " << creation_str << std::endl;
    
    // Check mikonim ke aya karbar ta be hal login karde ya na
    if (this->lastLoginTimestamp == 0) {
        std::cout << "Last Login Time: Never logged in before\n";
    } else {
        std::cout << "Last Login Time: " << login_str << std::endl;
    }
    std::cout << "--------------------\n";
}