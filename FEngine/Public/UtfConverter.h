#pragma once

#include<string>


extern std::wstring ConvertUtf(const std::string& utf8);


extern void ConvertUtf(const std::wstring& src, std::string& utf8);


extern bool FileExists(const wchar_t* path);
static std::string GLOBAL_PATH = "D://GitProject//FEngine//";
extern bool FindGlobalPath();

static const char* const PATH_SEPARATORS = "/\\";
class File
{
public:
    File() = default;
    File(const std::string& file)
    {
        InitFile(file);
    }
    File(const File& relative, const std::string& filename)
    {

    }

    bool InitFile(const std::string& file)
    {
        if (isFilePath(file))
        {
            absolutePath = file;
        }
        else if (_isAbsoleteFile(file))
        {
            fileExt = getFileExtension(file);
            fileName = getSimpleFileName(file);
            absolutePath = getFilePath(file);
        }
        else {

        }
        return true;
    }



   
    bool exist()const
    {

    }
    bool isDirectory() const{
        return fileName.empty();
    }

    bool _isAbsoleteFile(const std::string& file)
    {
        return true;
    }

    File GetRelativeFile() const 
    {

    }

    std::string GetFilePath() const
    {
        return absolutePath;
    }

    std::string getFilePath(const std::string& file)const
    {
        std::string::size_type slash = file.find_last_of(PATH_SEPARATORS);
        if (slash == std::string::npos) return std::string();
        else return std::string(file, 0, slash+1);
    }

    std::string getFileExtension(const std::string& file)const
    {

        std::string::size_type dot = file.find_last_of('.');
        std::string::size_type slash = file.find_last_of(PATH_SEPARATORS);
        if (dot == std::string::npos || (slash != std::string::npos && dot < slash)) return std::string("");
        return std::string(file.begin() + dot + 1, file.end());
    }
    std::string getSimpleFileName(const std::string& fileName)
    {
        std::string::size_type slash = fileName.find_last_of(PATH_SEPARATORS);
        if (slash == std::string::npos) return fileName;
        else return std::string(fileName.begin() + slash + 1, fileName.end());
    }
    bool isFilePath(const std::string& file) const {
        return getFileExtension(file).empty();
    }

   
private:
  /*  std::basic_string<wchar_t> fileName;
    std::basic_string<wchar_t> fileExt;
    std::basic_string<wchar_t> absolutePath;
    std::basic_string<wchar_t> relativePath;*/

    std::string fileName;
    std::string fileExt;
    std::string absolutePath;
    std::string relativePath;
};
