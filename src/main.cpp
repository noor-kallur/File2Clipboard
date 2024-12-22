#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>

void copyToClipboard(const std::string& file_Content) {
    if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard" << std::endl;
        system("pause");
        return;
    }

    if (!EmptyClipboard()) {
        std::cerr << "Failed to empty clipboard" << std::endl;
        CloseClipboard();
        system("pause");
        return;
    }

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (file_Content.size() + 1) * sizeof(char));
    if (hGlobal == nullptr) {
        std::cerr << "Failed to allocate memory" << std::endl;
        CloseClipboard();
        system("pause");
        return;
    }

    char* pGlobal = static_cast<char*>(GlobalLock(hGlobal));
    if (pGlobal != nullptr) {
        memcpy(pGlobal, file_Content.c_str(), file_Content.size() + 1);
        GlobalUnlock(hGlobal);
    }
    else 
    {
        std::cerr << "Failed to lock global memory" << std::endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        system("pause");
        return;
    }

    if (SetClipboardData(CF_TEXT, hGlobal) == nullptr) {
        std::cerr << "Failed to set clipboard data" << std::endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        system("pause");
        return;
    }

    //std::cout << "File copied to clipboard!" << std::endl;
    CloseClipboard();
}

std::string ReadFileContents(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        system("pause");
        return "";
    }

    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return contents;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: FileCopyUtility <source_file>" << std::endl;
        system("pause");
        return 1;
    }

    //std::cout << "File: " << argv[1] << std::endl;
    std::string file_Content = ReadFileContents(argv[1]);
    //std::cout << "File content: " << file_Content << std::endl;

    copyToClipboard(file_Content);
    //system("pause");
    return 0;
}
