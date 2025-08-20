// User.h

// In dastoorat jelogiri mikonan az inke file "User.h" chand bar dar project include beshe
// va be moshkel bokhorim.
#ifndef USER_H
#define USER_H

// Ketabkhane "string" ro ezafe mikonim ta betavanim az dade-i be nam-e string estefade konim.
#include <string>

// Ta'rif-e class-e User
class User {
public: // Yani in moteghayerha va tavabe az birun-e class dastresipazir hastan

    // --- Moteghayerha (Variables) ---

    // yek shomare-ye monhaser be fard baraye har karbar
    int id;
    // nam-e karbari
    std::string username;
    // ramz-e oboor ke be soorat-e hash zakhire mishe
    std::string passwordHash;
    // zaman-e sakhte shodan-e account (be soorat-e timestamp)
    long creationTimestamp;
    long lastLoginTimestamp;


    // --- Tavabe (Functions) ---

    // Constructor: Tabe-i ke hamnam-e class hast va vaghti yek object az class sakhte mishe,
    // be soorat-e khodkar ejra mishe. Baraye meghdardehi-e avalie khube.
    User();

    // Tabe-i baraye sabt-e nam-e yek karbar-e jadid
    void registerUser();

    // Tabe-i baraye vorood-e karbar
    bool loginUser();
    // Tabe'i baraye hazf-e account-e karbar-e Feli
    void deleteUser();
    // Tabe'i baraye khorooj az account
    void logoutUser();
    void showStats();
};

#endif // USER_H