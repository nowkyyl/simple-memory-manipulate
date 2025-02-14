#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <windows.h>
#include <TlHelp32.h>
#include <string>

class ProcessManager 
{
    public:
        explicit ProcessManager(const std::string& processName);
        ~ProcessManager();

        HANDLE GetProcessHandle() const;
        DWORD GetProcessId() const;

    private:
        DWORD processId;
        HANDLE processHandle;

        DWORD FindProcessId(const std::string& processName);
};

#endif