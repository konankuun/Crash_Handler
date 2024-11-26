#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#define PROCESS_NAME "RobloxPlayerBeta.exe" // Program name here

#define CHECK_DELAY 4000

#define ROBLOX_URI "roblox://placeID=13379208636/" // Roblox URI here

int isProcessRunning(const char *precessName){
    int found = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(Process32First(hSnapshot, &pe32)){
        do{
            if(_stricmp(pe32.szExeFile, precessName) == 0){
                found = 1;
                break;
            }
        }while(Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return found;
}

void startRoblox(){
    ShellExecute(NULL, "open", ROBLOX_URI, NULL, NULL, SW_SHOWNORMAL); // Replace ROBLOX_URI with NULL if you don't want a roblox place ID.
    printf("✅ Roblox restarted successfuly.\n");
}

int main(){
    printf("🔎 Processus checking %s...\n", PROCESS_NAME);
    while(1){
        if(!isProcessRunning(PROCESS_NAME)){
            printf("⚠️ Roblox crashed. Restarting...");
            Sleep(5);
            startRoblox();
        }
    }
    return 0;
}