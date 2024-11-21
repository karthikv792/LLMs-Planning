@echo off

echo "Format code using Google style"

FOR /d %%A in (applications libraries) DO (
    REM echo %%A
    cd %%A
    FOR /d %%B in ("*") DO (
        REM echo %%B
        cd %%B
        FOR /d %%C in ("*") DO (
            REM echo %%C
            cd %%C
            FOR %%F in ("*.h" "*.cpp") DO (
                echo|set /p="Formatting %%A/%%B/%%C/%%F ... "
                clang-format -style=file -i %%F
                echo Formatted
            )
            cd ..
        )
        cd ..
    )
    cd ..
)
