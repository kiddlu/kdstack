@echo off

::export symble in src or def
cl /MT /c dll.c
link /dll dll.obj /def:dll.def

cl.exe main.c dll.lib