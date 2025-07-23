#!/bin/bash

# Скрипт для запуска GUI приложения Linia2 с поддержкой дисплея

echo "=== Настройка окружения для запуска GUI приложения ==="

# Проверяем, запущена ли система с GUI
if [ -z "$DISPLAY" ]; then
    echo "⚠️  DISPLAY не установлен. Пытаемся настроить..."
    
    # Попробуем найти активную X11 сессию
    DISPLAY_NUM=$(ps aux | grep Xorg | grep -v grep | head -1 | sed 's/.*:\([0-9]\+\).*/\1/')
    
    if [ ! -z "$DISPLAY_NUM" ]; then
        export DISPLAY=:$DISPLAY_NUM
        echo "✅ Установлен DISPLAY=:$DISPLAY_NUM"
    else
        echo "❌ Активная X11 сессия не найдена"
        echo "Возможные решения:"
        echo "1. Запустите скрипт из графического терминала"
        echo "2. Подключитесь через SSH с флагом -X: ssh -X user@orangepi"
        echo "3. Используйте VNC для удаленного доступа к рабочему столу"
        echo "4. Запустите виртуальный X сервер: sudo apt install xvfb && export DISPLAY=:99 && Xvfb :99 -screen 0 1024x768x16 &"
        exit 1
    fi
fi

echo "✅ DISPLAY установлен: $DISPLAY"

# Проверяем доступность X сервера
if ! xset q &>/dev/null; then
    echo "❌ X сервер недоступен"
    echo "Возможные решения:"
    echo "1. Убедитесь, что графическая оболочка запущена"
    echo "2. Проверьте права доступа к X сессии: xhost +local:"
    echo "3. Для SSH подключения используйте: ssh -X или ssh -Y"
    exit 1
fi

echo "✅ X сервер доступен"

# Переходим в папку с исполняемым файлом
cd "$(dirname "$0")/../../generated/Linia2" || {
    echo "❌ Не найдена папка с исполняемым файлом: ../../generated/Linia2"
    exit 1
}

# Проверяем наличие исполняемого файла
if [ ! -f "./Linia2" ]; then
    echo "❌ Исполняемый файл Linia2 не найден"
    echo "Сначала выполните сборку проекта: ./full_build_arm64.sh"
    exit 1
fi

# Проверяем наличие конфигурационного файла
if [ ! -f "CIC.loc" ] && [ -f "../../../files/Linia.loc" ]; then
    echo "📁 Копируем конфигурационный файл..."
    cp "../../../files/Linia.loc" "./CIC.loc"
fi

echo "🚀 Запускаем приложение Linia2..."
echo "Исполняемый файл: $(pwd)/Linia2"
echo "DISPLAY: $DISPLAY"

# Запускаем приложение
./Linia2

echo "🏁 Приложение завершено"
