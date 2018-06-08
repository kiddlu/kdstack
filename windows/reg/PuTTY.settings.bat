@echo off

reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "CloseOnExit" /t REG_DWORD /d 0x00000002 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "WarnOnClose" /t REG_DWORD /d 0x00000000 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "BlinkCur" /t REG_DWORD /d 0x00000001 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Font" /t REG_SZ /d "Consolas" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "FontCharSet" /t REG_DWORD /d 0x000003a8 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "MouseOverride" /t REG_DWORD /d 0x00000001 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "RectSelect" /t REG_DWORD /d 0x00000000 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "FontIsBold" /t REG_DWORD /d 0x00000000 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "FontHeight" /t REG_DWORD /d 0x0000000b /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "TermWidth" /t REG_DWORD /d 0x00000055 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "TermHeight" /t REG_DWORD /d 0x00000019 /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "ScrollbackLines" /t REG_DWORD /d 0x00004e20 /f >nul

reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour0" /t REG_SZ /d "131,148,150" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour1" /t REG_SZ /d "147,161,161" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour2" /t REG_SZ /d "0,43,54" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour3" /t REG_SZ /d "7,54,66" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour4" /t REG_SZ /d "0,43,54" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour5" /t REG_SZ /d "238,232,213" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour6" /t REG_SZ /d "7,54,66" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour7" /t REG_SZ /d "0,43,56" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour8" /t REG_SZ /d "220,50,47" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour9" /t REG_SZ /d "203,75,22" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour10" /t REG_SZ /d "133,153,0" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour11" /t REG_SZ /d "88,110,117" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour12" /t REG_SZ /d "181,137,0" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour13" /t REG_SZ /d "101,123,131" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour14" /t REG_SZ /d "38,139,210" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour15" /t REG_SZ /d "131,148,150" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour16" /t REG_SZ /d "211,54,130" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour17" /t REG_SZ /d "108,113,196" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour18" /t REG_SZ /d "42,161,152" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour19" /t REG_SZ /d "147,161,161" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour20" /t REG_SZ /d "238,232,213" /f >nul
reg add "HKCU\Software\SimonTatham\PuTTY\Sessions\Default%%20Settings" /v "Colour21" /t REG_SZ /d "253,246,227" /f >nul