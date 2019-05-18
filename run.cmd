@ECHO OFF

set compNum=

if %1.==. (
    set /p compNum="What competition number: "
) else (
    set compNum=%1%
)


ch -u competitions/comp%compNum%.h