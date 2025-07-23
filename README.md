# Измеритель параметров полупроводниковых приборов | Сборка ARM64

** Tools

Windows:
	Visual Studio 2022

Linux:
	Ubuntu 24.04.2 LTS
	gcc version 13.3.0

ARM64 (Orange Pi 5 Plus):
	Ubuntu 24.04 + KDE Neon Plasma
	gcc version 13.3.0
	Архитектура: aarch64

*** Сборка под Orange Pi 5 Plus (ARM64).

1. Установить Linux.

2. Зайти под root :
    sudo su - root

3. Обновиться :
    apt-get update
	apt-get upgrade -y

4. Установить git и файловый менеджер mc:
    apt install git
	apt install mc

5. Клонировать репозиторий :
    git clone https://github.com/ph1zmat/Linia.git

6. Установить зависимости для ARM64:
   cd Linia/sources/scripts/orange_pi
   chmod +x install_soft_arm64.sh
   ./install_soft_arm64.sh

   Если возникают проблемы с пакетами:
   chmod +x fix_packages_arm64.sh
   ./fix_packages_arm64.sh

7. Выполнить полную сборку для ARM64:
   chmod +x full_build_arm64.sh
   ./full_build_arm64.sh

   Если сборка не удалась:
   chmod +x fix_build_arm64.sh
   ./fix_build_arm64.sh

8. Скопировать файл files/CIC.loc в каталог sources/genereated/Linia2

9. Запустить приложение:
   cd ../../generated/Linia2
   ./Linia2
