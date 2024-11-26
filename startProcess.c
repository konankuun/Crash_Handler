#include <windows.h>
#include <stdio.h>
#include "crashHandler.h"

void startProcess() {
    WCHAR wProcessName[256];
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi;
    
    MultiByteToWideChar(CP_UTF8, 0, PROCESS_NAME, -1, wProcessName, 256);
    
    printf("🔍 Attempting to start: %s\n", PROCESS_NAME);
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    if (CreateProcessW(
        NULL,           // Pas de chemin spécifique
        wProcessName,   // Nom du processus comme ligne de commande
        NULL,           // Sécurité processus par défaut
        NULL,           // Sécurité thread par défaut
        FALSE,          // Pas d'héritage
        CREATE_NEW_CONSOLE, // Nouvelle console
        NULL,           // Environnement parent
        NULL,           // Dossier courant
        &si,            // Info démarrage
        &pi            // Info processus
    )) {
        printf("✅ %s restarté avec succès.\n", PROCESS_NAME);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        DWORD error = GetLastError();
        printf("❌ Erreur lors du redémarrage de %s (Code: %lu)\n", PROCESS_NAME, error);
        
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0,
            NULL
        );
        printf("Message d'erreur : %s\n", (char*)lpMsgBuf);
        LocalFree(lpMsgBuf);
    }
}