@ECHO OFF

echo Converting documentation to a html document...


grip documentation.md --quiet --title="Robot Documentation" --export documentation.html

echo Fixing html document...

powershell -Command "(gc documentation.html) -replace '                  Robot Documentation', '' | Out-File documentation.html"
powershell -Command "(gc documentation.html) -replace '<span class=\"octicon octicon-book\"></span>', '' | Out-File documentation.html"
powershell -Command "(gc documentation.html) -replace '<h3>', '' | Out-File documentation.html"
powershell -Command "(gc documentation.html) -replace '&amp;degg', '&deg;' | Out-File documentation.html"

echo Setting timestamp on html document...

set Year=
for /f "skip=2" %%x in ('wmic Path Win32_LocalTime get Year^,Month^,Day^,Hour^,Minute^,Second /Format:List') do (
if not defined Year set %%x
)

set pa=

IF %Hour% GTR 12 (
set /a Hour=Hour-12
set pa=pm
) ELSE set pa=am

set DayF=

IF %Day% GEQ 4 (
IF %Day% LSS 21 set DayF=th

IF %Day% GEQ 24 (
IF %Day% LSS 31 set DayF=th
)
)

set _tempvar=0
IF %Day% EQU 1 set _tempvar=1
IF %Day% EQU 21 set _tempvar=1
IF %Day% EQU 31 set _tempvar=1
IF %_tempvar% EQU 1 set DayF=st

set _tempvar=0
IF %Day% EQU 2 set _tempvar=1
IF %Day% EQU 22 set _tempvar=1
IF %_tempvar% EQU 1 set DayF=nd

set _tempvar=0
IF %Day% EQU 3 set _tempvar=1
IF %Day% EQU 23 set _tempvar=1
if %_tempvar% EQU 1 set DayF=rd

IF %Month% EQU 1 (
set Month=January
) ELSE IF %Month% EQU 2 (
set Month=Febuary
) ELSE IF %Month% EQU 3 (
set Month=March
) ELSE IF %Month% EQU 4 (
set Month=April
) ELSE IF %Month% EQU 5 (
set Month=May
) ELSE IF %Month% EQU 6 (
set Month=June
) ELSE IF %Month% EQU 7 (
set Month=July
) ELSE IF %Month% EQU 8 (
set Month=August
) ELSE IF %Month% EQU 9 (
set Month=September
) ELSE IF %Month% EQU 10 (
set Month=October
) ELSE IF %Month% EQU 11 (
set Month=November
) ELSE IF %Month% EQU 12 (
set Month=December
)

echo Timestamp is "%Hour%:%Minute% %pa% | %Month% %Day%%DayF%, %Year%"

set comman="(gc documentation.html) -replace '</article>', '<div align=\"right\" style=\"font-size: 11px\">Last Updated <em>%Hour%:%Minute% %pa% | %Month% %Day%%DayF%, %Year%</em></div></article>' | Out-File documentation.html"
powershell -Command %comman%

PAUSE
