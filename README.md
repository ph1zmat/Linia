# Измеритель параметров полупроводниковых приборов

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

*** Сборка под Linux x86_64.

1. Установить Linux.

2. Зайти под root :
    sudo su

3. Обновиться :
    apt-get update
	apt-get upgrade -y

4. Установить git и файловый менеджер mc:
    apt install git
	apt install mc

5. Клонировать репозиторий :
    git clone github.com/Sasha7b9Work/Linia

6. Установить инструменты сборки:
   cd Linia/sources/scripts/cb_lin
   ./install_soft.sh

7. Установить зависимости для wxWidgets :
   cd ../../ThirdParty/linux
   ./install_software.sh

8. Собрать wxWidgets :
  ./full_build.sh

9. Собрать проект :
    cd ../../scripts/cb_lin
	./full_build.sh

10. Скопировать файл files/CIC.loc в каталог sources/genereated/Linia2

11. Запустить файл sources/generated/Linia2/Linia2 :
    ./Linia2

*** Сборка для Orange Pi 5 Plus (ARM64).

ВНИМАНИЕ: Для ARM64 архитектуры используйте специальные скрипты!

1-5. Выполнить шаги 1-5 из инструкции для Linux x86_64

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

8. Запустить приложение:
   cd ../../generated/Linia2
   ./Linia2

*** Особенности ARM64 сборки:

- Автоматическое определение архитектуры aarch64
- Поиск библиотек в /usr/lib/aarch64-linux-gnu/
- Оптимизация для ARM Cortex-A76 (флаг -march=armv8-a)
- Совместимость с KDE Neon Plasma на Orange Pi 5 Plus
