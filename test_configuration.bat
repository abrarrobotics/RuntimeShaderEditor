@echo off

echo Testing NW_SDK variable
if not exist %NW_SDK%\nw.exe (
	echo You must set the environment variable NW_SDK pointing to the NW.js SDK folder
) else (
    echo OK
)

echo Testing NW_DIR variable
if not exist %NW_DIR%\nw.exe (
	echo You must set the environment variable NW_DIR pointing to the NW.js release folder
) else (
    echo OK
)

pause
