#include "Interface.h"
#include "Security.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

Interface::Interface(DataManager* dm): dm(dm) {}

static string input_line(const string &prompt="") {
    cout << prompt;
    string s;
    getline(cin >> ws, s);
    return s;
}

void Interface::runTests() {
    cout << "[Tests] Creating sample users and messages...\n";
    User a; a.username="alice"; a.pass_hash = Security::hash_password("alice"); dm->addUser(a);
    User b; b.username="bob"; b.pass_hash = Security::hash_password("bob"); dm->addUser(b);
    dm->addMessage(Message{"alice","bob",0,"hello bob",false});
    int rid = dm->createRoom("project", "alice");
    dm->addMessage(Message{"alice","room:"+to_string(rid),0,"room message",false});
    cout << "[Tests] Done. Check data/ folder.\n";
}

void Interface::startCLI() {
    string curUser="";
    cout << "Messenger CLI (built-in Interface)\nType 'help' for commands\n";
    while (true) {
        string cmd = input_line("> ");
        if (cmd=="help") {
            cout << "Commands: register, login, logout, delete, stats, send, inbox, users, createroom, inviteroom, leaveroom, roomhistory, export, save, tests, exit\n";
        } else if (cmd=="register") {
            string u = input_line("username: ");
            string p = input_line("password: ");
            User U; U.username=u; U.pass_hash = Security::hash_password(p); U.created_at = time(nullptr);
            if (dm->addUser(U)) cout << "Registered\n"; else cout << "Exists\n";
        } else if (cmd=="login") {
            string u = input_line("username: ");
            string p = input_line("password: ");
            User* uu = dm->findUser(u);
            if (!uu) { cout << "No such user\n"; continue; }
            if (uu->pass_hash == Security::hash_password(p)) {
                curUser = u;
                uu->last_login = time(nullptr);
                cout << "Logged in. Unread: " << uu->unread_count << "\n";
            } else cout << "Wrong password\n";
        } else if (cmd=="logout") {
            curUser="";
            cout << "Logged out\n";
        } else if (cmd=="delete") {
            if (curUser.empty()) { cout << "Not logged in\n"; continue; }
            string c = input_line("Type YES to confirm delete: ");
            if (c=="YES") { dm->deleteUser(curUser); curUser=""; cout << "Deleted\n"; }
        } else if (cmd=="stats") {
            if (curUser.empty()) { cout << "Login first\n"; continue; }
            User* u = dm->findUser(curUser);
            if (!u) { cout << "Error: Could not find current user data.\n"; continue; }
            
            cout << "--- Stats for " << u->username << " ---\n";
            time_t t = u->created_at;
            cout << "Account created: " << (t ? ctime(&t) : "N/A\n");
            t = u->last_login;
            cout << "Last login: " << (t ? ctime(&t) : "N/A\n");
            cout << "Total messages sent: " << u->sent_count << "\n";
            cout << "Unread messages: " << u->unread_count << "\n";
        } else if (cmd=="send") {
            if (curUser.empty()) { cout << "Login first\n"; continue; }
            string to = input_line("to (username or room:<id>): ");
            string content = input_line("content: ");
            // bot commands
            if (content.rfind("@echo ",0)==0) {
                string rest = content.substr(6);
                // echo: post same content to same target (here: just add another message)
                dm->addMessage(Message{curUser, to, time(nullptr), rest, false});
                cout << "[Bot echo] done\n";
            } else if (content.rfind("@all ",0)==0) {
                string rest = content.substr(5);
                auto names = dm->getAllUsernames();
                for (auto &n : names) if (n!=curUser) dm->addMessage(Message{curUser, n, time(nullptr), rest, false});
                cout << "[Bot all] done\n";
            } else {
                dm->addMessage(Message{curUser, to, time(nullptr), content, false});
                cout << "Sent\n";
            }
        } else if (cmd=="inbox") {
            if (curUser.empty()) { cout << "Login first\n"; continue; }
            auto inbox = dm->getInbox(curUser,50);
            cout << "Inbox: " << inbox.size() << " messages\n";
            for (auto &m : inbox) {
                cout << "From: " << m.from << " | " << m.content << "\n";
            }
        } else if (cmd=="users") {
            auto names = dm->getAllUsernames();
            cout << "Users:\n";
            for (auto &n: names) cout << " - " << n << "\n";
        } else if (cmd=="createroom") {
            if (curUser.empty()) { cout << "Login first\n"; continue; }
            string name = input_line("room name: ");
            int id = dm->createRoom(name, curUser);
            cout << "Room created id="<<id<<"\n";
        } else if (cmd=="inviteroom") {
            int rid = stoi(input_line("room id: "));
            string uname = input_line("username: ");
            if (dm->inviteToRoom(rid, uname)) cout << "Invited\n"; else cout << "Failed\n";
        } else if (cmd=="leaveroom") {
            if (curUser.empty()) { cout << "Login first\n"; continue; }
            int rid = stoi(input_line("room id to leave: "));
            if (dm->leaveRoom(rid, curUser)) cout << "You have left the room\n";
            else cout << "Failed to leave room (not a member?)\n";
        } else if (cmd=="roomhistory") {
            int rid = stoi(input_line("room id: "));
            auto h = dm->getRoomHistory(rid,50);
            cout << "History count: " << h.size() << "\n";
            for (auto &m : h) cout << "["<<m.from<<"] "<<m.content<<"\n";
        } else if (cmd=="export") {
            if (curUser.empty()) { cout << "Login first\n"; continue; }
            int rid = stoi(input_line("room id: "));
            string filename = input_line("filename (e.g. chat.txt): ");
            
            auto h = dm->getRoomHistory(rid, 1000); // export up to 1000 messages
            ofstream outfile(filename);
            if (!outfile.is_open()) {
                cout << "Error: Could not open file for writing.\n";
                continue;
            }
            
            outfile << "Chat history for room " << rid << "\n";
            outfile << "----------------------------------\n";
            for (auto &m : h) {
                time_t t = m.sent_at;
                char buf[80];
                strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
                outfile << "[" << buf << "] " << m.from << ": " << m.content << "\n";
            }
            cout << "History exported to " << filename << "\n";

        } else if (cmd=="save") {
            dm->saveAll();
            cout << "Saved\n";
        } else if (cmd=="tests") {
            runTests();
        } else if (cmd=="exit") {
            dm->saveAll();
            cout << "Bye\n";
            break;
        } else {
            cout << "Unknown command\n";
        }
    }
}
