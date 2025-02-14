#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <windows.h>
#include <iostream>
#include <random>
#include <thread>

class MemoryManager
{
    public:
        explicit MemoryManager(HANDLE process);

        template <typename T>
        T ReadMemory(uintptr_t address);

        template <typename T>
        bool WriteMemory(uintptr_t address, T value);

    private:
        HANDLE processHandle;

        void RandomSleep();
        void CheckDebuggerPresence();
        uintptr_t ManipulatePointer(uintptr_t address);
        uintptr_t RestorePointer(uintptr_t adress);
};

#endif