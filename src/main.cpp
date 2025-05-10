#include "MainPage.h"
#include <filesystem>

namespace fs = std::filesystem;

bool FileExists(const std::string& file_path)
{
    return fs::exists(file_path);
}


int main()
{

    std::vector<std::string> list_file_path=
    {
        "E:/University2/3.2_course/cursach/ModelPanel/model/model_panel_STL.stl",
        "F:/University2/3.2_course/cursach/ModelPanel/model/model_panel_STL.stl",
        "G:/University2/3.2_course/cursach/ModelPanel/model/model_panel_STL.stl",
        "D:/model_panel/model_panel_STL.stl",
        "C:/model_panel/model_panel_STL.stl"
    };

    for(std::string file_path : list_file_path)
    {
        if (FileExists(file_path))
        {
            std::cout << "The file was found at the path: " << file_path << std::endl;
            MainPage main_page(file_path);
            break;
        }
        std::cout << "The file is missing from the path: " << file_path << std::endl;
    }


    return 0;
}