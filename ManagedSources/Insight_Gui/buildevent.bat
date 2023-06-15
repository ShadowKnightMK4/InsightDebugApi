
@echo off
REM TO use this macro put "call "
REM For projects being made in this solution. Set Post build to above.
REM copy /Y  "$(SolutionDir)code\Native\InsightApi\$(Configuration)\$(PlatformTarget)\InsightAPI.*"  $(OutDir)
copy /Y %1 %3

copy /y %2 %3

if errorlevel 1 (
    echo Error occurred
    exit /b 1
) else (
    echo Success
    exit /b 0
)