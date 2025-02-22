@echo off
set "source_file1=.\out\attr_defconfig.h"
set "source_file2=.\out\attr_pid_table.h"
set "destination_folder=..\inc"

rem 复制文件到目标文件夹
copy /y "%source_file1%" "%destination_folder%"
copy /y "%source_file2%" "%destination_folder%"

rem 检查复制是否成功
if errorlevel 1 (
    echo Error: Failed to copy file.
    exit /b 1
) else (
    echo File copied successfully.
)

exit /b 0
