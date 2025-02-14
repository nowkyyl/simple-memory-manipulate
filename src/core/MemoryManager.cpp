#include <core/MemoryManager.h>

MemoryManager::MemoryManager(HANDLE process) : processHandle(process) {}

void MemoryManager::RandomSleep()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(1, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen)));
}

uintptr_t MemoryManager::ManipulatePointer(uintptr_t address)
{
    uintptr_t result = address;
    for (int i = 0; i < 10; ++i)
    {
        result = result * 3 + 0x1337;
    }
    return result;
}

uintptr_t MemoryManager::RestorePointer(uintptr_t manipulatedAddress)
{
    uintptr_t result = manipulatedAddress;
    for (int i = 0; i < 10; ++i)
    {
        if (result % 3 != 0) {
            std::cerr << "invalid manipulated address" << std::endl;
            exit(EXIT_FAILURE);
        }
        result = (result - 0x1337) / 3;
    }
    return result;
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

    address = ManipulatePointer(address);
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

    address = ManipulatePointer(address);
    if (!WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), nullptr)) {
        std::cerr << "failed to write memory at address: " << std::hex << address << std::dec << std::endl;
        return false;
    }

    return true;
}