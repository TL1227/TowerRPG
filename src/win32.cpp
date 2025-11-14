#ifdef _WIN32
#include <windows.h>
#include <vector>
#include <string>

int main(int argc, char** argv);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    int argc = 0;
    LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);

    std::vector<std::string> argv_str;
    argv_str.reserve(argc);

    for (int i = 0; i < argc; ++i) {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, nullptr, 0, nullptr, nullptr);
        std::string arg(size_needed - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, arg.data(), size_needed, nullptr, nullptr);
        argv_str.push_back(std::move(arg));
    }

    std::vector<char*> argv;
    argv.reserve(argc);
    for (auto& s : argv_str)
        argv.push_back(s.data());

    LocalFree(argvW);

    return main(argc, argv.data());
}
#endif

