#include <iostream>
#include <fstream>
#include <vector>
#include "Message.h"

using namespace std;

// ذخیره در فایل txt
void exportMessagesToTxt(const vector<Message>& messages, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "Cannot open file: " << filename << endl;
        return;
    }
    for (const auto& msg : messages) {
        out << msg.getSender() << ": " 
            << msg.getContent() << " (" 
            << msg.getTimestamp() << ")" << endl;
    }
    out.close();
}

// ذخیره در JSON
void exportMessagesToJson(const vector<Message>& messages, const string& filename) {
    json jArray = json::array();
    for (const auto& msg : messages) {
        jArray.push_back(msg.toJson());
    }
    ofstream out(filename);
    out << jArray.dump(4); // فرمت مرتب
    out.close();
}

// لود از JSON
vector<Message> loadMessagesFromJson(const string& filename) {
    ifstream in(filename);
    vector<Message> messages;
    if (!in) {
        cerr << "Cannot open file: " << filename << endl;
        return messages;
    }
    json jLoad;
    in >> jLoad;
    for (const auto& j : jLoad) {
        messages.push_back(Message::fromJson(j));
    }
    return messages;
}

int main() {
    vector<Message> messages;
    messages.emplace_back("Taha", "Hello!", time(nullptr));
    messages.emplace_back("Ali", "Hi, how are you?", time(nullptr));

    // ذخیره در فایل‌ها
    exportMessagesToTxt(messages, "messages.txt");
    exportMessagesToJson(messages, "messages.json");

    // تست لود دوباره
    vector<Message> loaded = loadMessagesFromJson("messages.json");
    cout << "Loaded messages:" << endl;
    for (const auto& msg : loaded) {
        cout << msg.getSender() << " -> " << msg.getContent() << endl;
    }
    return 0;
}
