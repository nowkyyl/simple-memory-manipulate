#include <core/MemoryManager.h>

MemoryManager::MemoryManager(HANDLE process) : processHandle(process) {}

void MemoryManager::RandomSleep()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(1, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen)));
}

void MemoryManager::CheckDebuggerPresence()
{
    __asm
    {
        mov eax, 30h
        call eax

        test al, al
        jz NoDebuggerFound
        
        mov ecx, 1000000h
    
    delay:
        dec ecx
        jnz delay

        NoDebuggerFound:
        nop
    }
}

template <typename T>
T MemoryManager::ReadMemory(uintptr_t address)
{
    CheckDebuggerPresence();

    T buffer;
    if (!ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, sizeof(T), nullptr)) {
        std::cerr << "failed to read memory at address: " << std::hex << address << std::dec << std::endl;
        return T();
    }

    return buffer;
}

template <typename T>
bool MemoryManager::WriteMemory(uintptr_t address, T value)
{
    CheckDebuggerPresence();

    if (!WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), nullptr)) {
        std::cerr << "failed to write memory at address: " << std::hex << address << std::dec << std::endl;
        return false;
    }

    return true;
}
