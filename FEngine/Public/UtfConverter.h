#pragma once

#include<string>


extern std::wstring ConvertUtf(const std::string& utf8);


extern void ConvertUtf(const std::wstring& src, std::string& utf8);


extern bool FileExists(const wchar_t* path);
static std::string GLOBAL_PATH = "D://GitProject//FEngine//FRenderer//";
extern bool FindGlobalPath();
class File
{
    File(const std::string& file)
    {

    }
    bool Exist()
    {

    }
private:
    std::basic_string<wchar_t> fileName;
    std::basic_string<wchar_t> fileExt;
    std::basic_string<wchar_t> absolutePath;
    std::basic_string<wchar_t> relativePath;
};
