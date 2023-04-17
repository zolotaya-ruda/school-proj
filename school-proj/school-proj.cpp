#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


class Reader
{
    std::vector<std::string> path;
    Reader(std::vector<std::string> p_path) : path(p_path){}

    STATUS go_to(char p_path)
    {
        std::string formatted_str(path.begin(), path.end());
        formatted_str.push_back(p_path);

        for (const auto& entry : fs::directory_iterator(formatted_str))
            std::cout << entry.path() << std::endl;
    }
    
};


enum class STATUS{
    ERROR,
    OK
};

std::vector<std::string> split(std::string path)
{
    std::vector<std::string> out;
    std::string buff;
    std::string last_symb;

    for (auto symb : path)
    {
        if (symb == '/')
        {
            out.push_back(buff);
            buff = "";
            continue;
        }
        buff.push_back(symb);
        last_symb = buff;
    }
    out.push_back(last_symb);

    return out;
}


int main()
{

    std::string path = "D:/";

    Reader reader(path);

    std::vector<std::string> splited = split(path);
    splited.pop_back();
    for (auto str : splited)
    {
        std::cout << str << '\n';
    }
   // std::string path = "C:/";
   // for (const auto& entry : fs::directory_iterator(path))
     //   std::cout << entry.path() << std::endl;
}