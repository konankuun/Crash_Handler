#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <conio.h>
#include "crashHandler.h"

char PROCESS_NAME[256] = "";

void startProcess();

int isProcessRunning(const char *processName){
    int found = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(Process32First(hSnapshot, &pe32)){
        do{
            if(_stricmp(pe32.szExeFile, processName) == 0){
                found = 1;
                break;
            }
        }while(Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return found;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (!IsWindowVisible(hwnd)) return TRUE;
    
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    
    char title[256];
    GetWindowTextA(hwnd, title, sizeof(title));
    
    if (strlen(title) == 0) return TRUE;
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (Process32First(snapshot, &pe32)) {
        do {
            if (pe32.th32ProcessID == processId) {
                printf("%d - %s (%s)\n", *((int*)lParam), pe32.szExeFile, title);
                (*((int*)lParam))++;
                break;
            }
        } while (Process32Next(snapshot, &pe32));
    }
    return TRUE;
}

void listRunningProcesses() {
    system("cls");
    printf("\n📋 List of open windows:\n\n");
    int counter = 1;
    EnumWindows(EnumWindowsProc, (LPARAM)&counter);
    printf("\nType _exit to quit.\n");
    printf("Type _analyse to refresh the list.\n\n");
    printf("Choose a process (Type the exact name with full path if needed): ");
    scanf("%259[^\n]", PROCESS_NAME);
    getchar();

    if (strstr(PROCESS_NAME, ".exe") == NULL) {
        strcat(PROCESS_NAME, ".exe");
    }
    printf("\nProcess selected : %s\n", PROCESS_NAME);
}

void exitProgram(){
    system("cls");
    printf("Exiting...\n");
    Sleep(1000);
    exit(0);
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);

    while(1) {
        listRunningProcesses();
        
        if (strcmp(PROCESS_NAME, "_exit") == 0) {
            exitProgram();
        }
        else if (strcmp(PROCESS_NAME, "_analyse") == 0) {
            system("cls");
            printf("Reanalysing process...\n");
            Sleep(1000);
            continue;
        }
        else {
            printf("🔎 Checking process %s...\n", PROCESS_NAME);
            while(1) {
                if(!isProcessRunning(PROCESS_NAME)) {
                    printf("⚠️ %s crashed. Restarting...\n", PROCESS_NAME);
                    startProcess();
                    Sleep(6000);
                }
                Sleep(CHECK_DELAY);
            }
        }
    }
    return 0;
}