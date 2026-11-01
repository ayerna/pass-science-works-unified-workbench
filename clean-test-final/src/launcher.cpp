// Windows-only launcher. Double-clicking Launch.exe must open the workbench
// with no terminal, admin rights, install or internet.
// Steps: run workbench.exe all (regenerates results/), wait for it to finish,
// then open frontend/index.html in the default browser.
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>

static std::string exe_dir() {
    char buf[MAX_PATH] = {0};
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    std::string dir(buf);
    std::size_t slash = dir.find_last_of("\\/");
    return (slash == std::string::npos) ? std::string(".\\") : dir.substr(0, slash + 1);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const std::string dir = exe_dir();
    CreateDirectoryA((dir + "results").c_str(), nullptr);
    std::string core = "\"" + dir + "workbench.exe\" all";
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    // CreateProcess needs a writable command-line buffer.
    std::vector<char> cmd(core.begin(), core.end());
    cmd.push_back('\0');
    if (CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, FALSE,
                       CREATE_NO_WINDOW, nullptr, dir.c_str(), &si, &pi)) {
        WaitForSingleObject(pi.hProcess, 30000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        MessageBoxA(nullptr,
            "workbench.exe was not found next to Launch.exe. "
            "Keep both files in the same folder and try again.",
            "Unified Engineering Workbench", MB_OK | MB_ICONERROR);
        return 1;
    }
    std::string page = dir + "frontend\\index.html";
    HINSTANCE opened = ShellExecuteA(nullptr, "open", page.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    if ((INT_PTR)opened <= 32) {
        MessageBoxA(nullptr, "Could not open frontend/index.html in the default browser.",
                    "Unified Engineering Workbench", MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}
#endif
