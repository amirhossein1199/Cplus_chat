#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
// #include <nlohmann/json.hpp> // will not be present; we emulate minimal JSON read/write manually

// Since we avoided external libs, we'll implement a very small ad-hoc JSON-like reader/writer
// for the specific file formats used: arrays of simple objects.

using namespace std;

static std::string trim(const std::string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a==string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b-a+1);
}

DataManager::DataManager(const std::string &dataDir): dir(dataDir) {
    // ensure trailing slash
    if (!dir.empty() && dir.back()!='/' && dir.back()!='\\') dir.push_back('/');
}

// ----- users -----
// Helper function to parse a number from the json string
static long long get_json_number(const string& content, size_t start_pos, const string& key) {
    size_t key_pos = content.find(key, start_pos);
    if (key_pos == string::npos) return 0;
    size_t colon_pos = content.find(':', key_pos);
    size_t num_start = content.find_first_of("0123456789", colon_pos);
    if (num_start == string::npos) return 0;
    size_t num_end = content.find_first_not_of("0123456789", num_start);
    return stoll(content.substr(num_start, num_end - num_start));
}

void DataManager::loadUsers() {
    users.clear();
    string path = dir + "users.json";
    ifstream f(path);
    if (!f.is_open()) return;
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    size_t pos = 0;
    while ((pos = content.find("\"username\"", pos))!=string::npos) {
        size_t colon = content.find(':', pos);
        size_t q1 = content.find('"', colon+1);
        size_t q2 = content.find('"', q1+1);
        string username = content.substr(q1+1, q2-q1-1);
        
        size_t ph = content.find("\"pass_hash\"", q2);
        size_t pc = content.find(':', ph);
        size_t pq1 = content.find('"', pc+1);
        size_t pq2 = content.find('"', pq1+1);
        string pass_hash = content.substr(pq1+1, pq2-pq1-1);

        User u;
        u.username = username;
        u.pass_hash = pass_hash;
        u.created_at = get_json_number(content, pq2, "\"created_at\"");
        u.last_login = get_json_number(content, pq2, "\"last_login\"");
        u.unread_count = get_json_number(content, pq2, "\"unread_count\"");
        u.sent_count = get_json_number(content, pq2, "\"sent_count\"");
        
        users.push_back(u);
        pos = q2; 
        size_t next_obj = content.find('{', pos);
        if (next_obj == string::npos) break;
        pos = next_obj;
    }
}

void DataManager::saveUsers() {
    string path = dir + "users.json";
    ofstream f(path);
    if (!f.is_open()) { cerr << "Could not open " << path << " to save users\n"; return; }
    f << "[\n";
    for (size_t i=0;i<users.size();++i) {
        f << "  {\n";
        f << "    \"username\": \"" << users[i].username << "\",\n";
        f << "    \"pass_hash\": \"" << users[i].pass_hash << "\",\n";
        f << "    \"created_at\": " << users[i].created_at << ",\n";
        f << "    \"last_login\": " << users[i].last_login << ",\n";
        f << "    \"unread_count\": " << users[i].unread_count << "\n";
        f << "    \"sent_count\": " << users[i].sent_count << "\n";
        f << "  }" << (i+1<users.size() ? "," : "") << "\n";
    }
    f << "]\n";
}

// ----- messages -----
void DataManager::loadMessages() {
    messages.clear();
    string path = dir + "messages.json";
    ifstream f(path);
    if (!f.is_open()) return;
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    size_t pos = 0;
    while ((pos = content.find("\"from\"", pos))!=string::npos) {
        size_t colon = content.find(':', pos);
        size_t q1 = content.find('"', colon+1);
        size_t q2 = content.find('"', q1+1);
        string from = content.substr(q1+1, q2-q1-1);
        // ... (code for parsing 'to' and 'content' remains the same)
        size_t topos = content.find("\"to\"", q2);
        size_t tcolon = content.find(':', topos);
        size_t tq1 = content.find('"', tcolon+1);
        size_t tq2 = content.find('"', tq1+1);
        string to = content.substr(tq1+1, tq2-tq1-1);
        
        time_t sent_at = get_json_number(content, tq2, "\"sent_at\"");

        size_t cpos = content.find("\"content\"", tq2);
        size_t ccolon = content.find(':', cpos);
        size_t cq1 = content.find('"', ccolon+1);
        size_t cq2 = content.find('"', cq1+1);
        string cont = content.substr(cq1+1, cq2-cq1-1);

        Message m;
        m.from = from; m.to = to; m.sent_at = sent_at; m.content = cont;
        messages.push_back(m);
        pos = cq2;
    }
}

void DataManager::saveMessages() {
    string path = dir + "messages.json";
    ofstream f(path);
    if (!f.is_open()) { cerr << "Could not open " << path << " to save messages\n"; return; }
    f << "[\n";
    for (size_t i=0;i<messages.size();++i) {
        f << "  {\n";
        f << "    \"from\": \"" << messages[i].from << "\",\n";
        f << "    \"to\": \"" << messages[i].to << "\",\n";
        f << "    \"sent_at\": " << messages[i].sent_at << ",\n";
        f << "    \"content\": \"" << messages[i].content << "\"\n";
        f << "  }" << (i+1<messages.size() ? "," : "") << "\n";
    }
    f << "]\n";
}

// ----- rooms -----
void DataManager::loadRooms() {
    rooms.clear();
    string path = dir + "rooms.json";
    ifstream f(path);
    if (!f.is_open()) return;
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    size_t pos = 0;
    while ((pos = content.find("\"id\"", pos))!=string::npos) {
        size_t colon = content.find(':', pos);
        size_t numstart = content.find_first_of("0123456789", colon);
        size_t numend = content.find_first_not_of("0123456789", numstart);
        int id = stoi(content.substr(numstart, numend-numstart));
        size_t namepos = content.find("\"name\"", pos);
        size_t ncolon = content.find(':', namepos);
        size_t nq1 = content.find('"', ncolon+1);
        size_t nq2 = content.find('"', nq1+1);
        string name = content.substr(nq1+1, nq2-nq1-1);
        ChatRoom r;
        r.id = id; r.name = name;
        rooms.push_back(r);
        pos = nq2;
    }
}

void DataManager::saveRooms() {
    string path = dir + "rooms.json";
    ofstream f(path);
    if (!f.is_open()) { cerr << "Could not open " << path << " to save rooms\n"; return; }
    f << "[\n";
    for (size_t i=0;i<rooms.size();++i) {
        f << "  {\n";
        f << "    \"id\": " << rooms[i].id << ",\n";
        f << "    \"name\": \"" << rooms[i].name << "\",\n";
        f << "    \"owner\": \"" << rooms[i].owner << "\"\n";
        f << "  }" << (i+1<rooms.size() ? "," : "") << "\n";
    }
    f << "]\n";
}

// Public methods
bool DataManager::loadAll() {
    loadUsers();
    loadMessages();
    loadRooms();
    return true;
}

bool DataManager::saveAll() {
    saveUsers();
    saveMessages();
    saveRooms();
    return true;
}

// user management
bool DataManager::addUser(const User &u) {
    if (find_if(users.begin(), users.end(), [&](const User &x){ return x.username==u.username; })!=users.end()) return false;
    users.push_back(u);
    saveUsers();
    return true;
}
bool DataManager::deleteUser(const std::string &username) {
    auto it = remove_if(users.begin(), users.end(), [&](const User &u){ return u.username==username; });
    if (it==users.end()) return false;
    users.erase(it, users.end());
    saveUsers();
    return true;
}
User* DataManager::findUser(const std::string &username) {
    for (auto &u : users) if (u.username==username) return &u;
    return nullptr;
}
std::vector<std::string> DataManager::getAllUsernames() {
    std::vector<std::string> out;
    for (auto &u : users) out.push_back(u.username);
    return out;
}

// messages
void DataManager::addMessage(const Message &m) {
    messages.push_back(m);
    // update sender's sent count
    if (auto sender = findUser(m.from)) {
        sender->sent_count++;
    }
    // mark unread for receiver
    if (auto receiver = findUser(m.to)) {
        receiver->unread_count++;
    }
    saveMessages();
    saveUsers(); // Need to save users to persist the new counts
}
std::vector<Message> DataManager::getInbox(const std::string &username, int limit) {
    std::vector<Message> out;
    for (auto &m : messages) if (m.to==username || (m.to.rfind("room:",0)==0 && false)) out.push_back(m);
    if (limit>0 && (int)out.size()>limit) out.resize(limit);
    return out;
}
std::vector<Message> DataManager::getRoomHistory(int roomId, int limit) {
    std::vector<Message> out;
    std::string rid = "room:" + std::to_string(roomId);
    for (auto &m : messages) if (m.to==rid) out.push_back(m);
    if (limit>0 && (int)out.size()>limit) out.resize(limit);
    return out;
}

// rooms
int DataManager::createRoom(const std::string &name, const std::string &owner) {
    int nid = 1;
    for (auto &r : rooms) nid = std::max(nid, r.id+1);
    ChatRoom c;
    c.id = nid;
    c.name = name;
    c.owner = owner;
    c.members.push_back(owner);
    rooms.push_back(c);
    saveRooms();
    return nid;
}
bool DataManager::inviteToRoom(int roomId, const std::string &username) {
    for (auto &r : rooms) {
        if (r.id==roomId) {
            if (std::find(r.members.begin(), r.members.end(), username)==r.members.end()) r.members.push_back(username);
            saveRooms();
            return true;
        }
    }
    return false;
}
bool DataManager::leaveRoom(int roomId, const std::string &username) {
    for (auto &r : rooms) {
        if (r.id==roomId) {
            r.members.erase(std::remove(r.members.begin(), r.members.end(), username), r.members.end());
            saveRooms();
            return true;
        }
    }
    return false;
}
