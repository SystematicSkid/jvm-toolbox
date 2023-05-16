@echo off
setlocal enabledelayedexpansion

for %%i in (*.fbs) do (
    flatc.exe --cpp --java -o ../ %%i
)

endlocal