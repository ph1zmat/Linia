@echo off
echo Установка прав на выполнение для ARM64 скриптов...

:: Для Windows-пользователей: этот файл поможет установить права при работе через WSL
echo.
echo Выполните следующие команды в WSL или на реальной ARM64 системе:
echo.
echo cd sources/scripts/orange_pi
echo chmod +x install_soft_arm64.sh
echo chmod +x full_build_arm64.sh
echo.
echo Затем запустите сборку:
echo ./install_soft_arm64.sh
echo ./full_build_arm64.sh
echo.
pause
