#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#include "../protection/lazy.h"
#include "../protection/skStr.h"


bool find(const char* name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    auto snapshot = LI_FN(CreateToolhelp32Snapshot).forwarded_safe_cached()(TH32CS_SNAPPROCESS, NULL);


    if (LI_FN(Process32First).forwarded_safe_cached()(snapshot, &entry) == TRUE)
    {
        while (LI_FN(Process32Next).forwarded_safe_cached()(snapshot, &entry) == TRUE)
        {
            if (!strcmp((const char*)entry.szExeFile, name))
            {
                return true;
            }
        }
    }

    LI_FN(CloseHandle).forwarded_safe_cached()(snapshot);
    return false;

}

int check_processes()
{
    std::vector<const char*> processes = {
    skCrypt("ollydbg.exe"),
    skCrypt("ProcessHacker.exe"),
    skCrypt("tcpview.exe"),
    skCrypt("autoruns.exe"),
    skCrypt("autorunsc.exe"),
    skCrypt("filemon.exe"),
    skCrypt("procmon.exe"),
    skCrypt("regmon.exe"),
    skCrypt("procexp.exe"),
    skCrypt("idaq.exe"),
    skCrypt("idaq64.exe"),
    skCrypt("ida.exe"),
    skCrypt("ida64.exe"),
    skCrypt("ImmunityDebugger.exe"),
    skCrypt("Wireshark.exe"),
    skCrypt("dumpcap.exe"),
    skCrypt("HookExplorer.exe"),
    skCrypt("ImportREC.exe"),
    skCrypt("PETools.exe"),
    skCrypt("LordPE.exe"),
    skCrypt("SysInspector.exe"),
    skCrypt("proc_analyzer.exe"),
    skCrypt("sysAnalyzer.exe"),
    skCrypt("sniff_hit.exe"),
    skCrypt("windbg.exe"),
    skCrypt("joeboxcontrol.exe"),
    skCrypt("joeboxserver.exe"),
    skCrypt("ResourceHacker.exe"),
    skCrypt("x32dbg.exe"),
    skCrypt("x64dbg.exe"),
    skCrypt("Fiddler.exe"),
    //skCrypt("httpdebugger.exe"),
    //skCrypt("HTTP Debugger Windows Service (32 bit).exe"),
    //skCrypt("HTTPDebuggerUI.exe"),
    //skCrypt("HTTPDebuggerSvc.exe"),
    skCrypt("cheatengine-x86_64.exe"),
    skCrypt("cheatengine-x86_64-SSE4-AVX2.exe"),
    };

    for (auto process : processes)
    {
        if (find(process)) *(uintptr_t*)(0) = 1;
    }
    return false;
}
