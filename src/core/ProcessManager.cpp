#include <core/ProcessManager.h>

ProcessManager::ProcessManager(const std::string& processName)
{
    processId = FindProcessId(processName);
    if (processId)
    {
        processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    }
}

ProcessManager::~ProcessManager()
{
    if (processHandle)
    {
        CloseHandle(processHandle);
    }
}

HANDLE ProcessManager::GetProcessHandle() const {
    return processHandle;
}

DWORD ProcessManager::GetProcessId() const {
    return processId;
}

std::wstring StringToWString(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstrTo[0], size_needed);
}

DWORD ProcessManager::FindProcessId(const std::string& processName)
{
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &entry))
    {
        do {
            if (!_wcsicmp(entry.szExeFile, StringToWString(processName).c_str()))
            {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}