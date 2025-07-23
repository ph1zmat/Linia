#!/usr/bin/env bash
# Полная сборка для Orange Pi 5 Plus (ARM64)

set -e

echo "Сборка проекта Linia для Orange Pi 5 Plus (ARM64)"
echo "Архитектура: $(uname -m)"

# Проверяем архитектуру
if [ "$(uname -m)" != "aarch64" ]; then
    echo "Предупреждение: Скрипт предназначен для ARM64 (aarch64), текущая архитектура: $(uname -m)"
fi

# Очистка предыдущих сборок
echo "Очистка предыдущих сборок..."
rm -R -f ../../generated
rm -R -f ../../ThirdParty/wxWidgets/generated

# Сборка wxWidgets для ARM64
echo "Сборка wxWidgets для ARM64..."
cd ../../ThirdParty/wxWidgets
cmake CMakeLists.txt -Bgenerated -G "CodeBlocks - Unix Makefiles" \
    -DwxBUILD_SAMPLES=ALL \
    -DwxBUILD_SHARED=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-O2 -march=armv8-a"

cd generated
cmake --build . -- -j$(nproc)
cd ../../../scripts/orange_pi

# Сборка основного проекта
echo "Сборка основного проекта..."
cmake ../../VS/CMakeLists.txt -B../../generated -G "CodeBlocks - Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-O2 -march=armv8-a"

cd ../../generated

# Попробуем собрать проект
echo "Компиляция основного проекта..."
if cmake --build . -- -j$(nproc); then
    echo "✅ Сборка завершена успешно!"
    cd ../scripts/orange_pi
else
    echo "❌ Ошибка сборки основного проекта"
    echo ""
    echo "Возможные причины и решения:"
    echo "1. Отсутствие библиотеки GSpell - запустите: ./fix_packages_arm64.sh"
    echo "2. Проблемы с линковкой - запустите: ./fix_build_arm64.sh"
    echo ""
    echo "Для получения подробной информации об ошибке, посмотрите вывод выше"
    cd ../scripts/orange_pi
    exit 1
fi

echo "Сборка завершена!"
echo "Исполняемый файл: ../../generated/Linia2/Linia2"

# Копирование конфигурационного файла
if [ -f "../../../files/Linia.loc" ]; then
    echo "Копирование конфигурационного файла..."
    cp ../../../files/Linia.loc ../../generated/Linia2/
else
    echo "Предупреждение: Файл конфигурации files/Linia.loc не найден"
fi

echo "Для запуска выполните:"
echo "cd ../../generated/Linia2 && ./Linia2"
echo ""
echo "📋 Важно для GUI приложения:"
echo "   • Приложение требует графическое окружение (X11/Wayland)"
echo "   • Для SSH подключения используйте: ssh -X пользователь@orangepi"
echo "   • Для диагностики GUI: ./diagnose_gui.sh"
echo "   • Для автоматического запуска с настройкой дисплея: ./run_with_display.sh"
