#!/bin/bash

# Диагностический скрипт для проверки GUI окружения

echo "=== Диагностика GUI окружения ==="

# Проверяем архитектуру
echo "🖥️  Архитектура системы: $(uname -m)"
echo "🐧 Операционная система: $(cat /etc/os-release | grep PRETTY_NAME | cut -d'"' -f2)"

# Проверяем пользователя
echo "👤 Текущий пользователь: $(whoami)"

# Проверяем переменную DISPLAY
if [ -z "$DISPLAY" ]; then
    echo "❌ DISPLAY не установлен"
    
    # Ищем активные X сессии
    echo "🔍 Поиск активных X сессий:"
    ps aux | grep -E "(Xorg|X)" | grep -v grep
    
    # Проверяем systemd services
    echo "🔍 Проверка графических служб:"
    systemctl is-active display-manager 2>/dev/null || echo "Display manager не активен"
    systemctl is-active gdm 2>/dev/null || echo "GDM не активен"
    systemctl is-active lightdm 2>/dev/null || echo "LightDM не активен"
    systemctl is-active sddm 2>/dev/null || echo "SDDM не активен"
    
else
    echo "✅ DISPLAY установлен: $DISPLAY"
    
    # Проверяем доступность X сервера
    if command -v xset &> /dev/null; then
        if xset q &>/dev/null; then
            echo "✅ X сервер доступен"
            xset q | head -5
        else
            echo "❌ X сервер недоступен"
        fi
    else
        echo "❌ Утилита xset не найдена. Установите: sudo apt install x11-utils"
    fi
fi

# Проверяем SSH соединение и X11 forwarding
if [ ! -z "$SSH_CLIENT" ] || [ ! -z "$SSH_TTY" ]; then
    echo "🔗 Обнаружено SSH соединение"
    if [ ! -z "$DISPLAY" ]; then
        echo "✅ X11 forwarding активен"
    else
        echo "❌ X11 forwarding не активен. Используйте: ssh -X или ssh -Y"
    fi
fi

# Проверяем установленные GUI библиотеки
echo "📚 Проверка GUI библиотек:"
ldconfig -p | grep -E "(gtk|qt|wx)" | head -10

# Проверяем исполняемый файл
EXEC_PATH="../../generated/Linia2/Linia2"
if [ -f "$EXEC_PATH" ]; then
    echo "✅ Исполняемый файл найден: $EXEC_PATH"
    echo "🔍 Зависимости исполняемого файла (графические):"
    ldd "$EXEC_PATH" | grep -E "(gtk|gdk|wx|X11)" | head -10
else
    echo "❌ Исполняемый файл не найден: $EXEC_PATH"
fi

echo ""
echo "=== Рекомендации ==="

if [ -z "$DISPLAY" ]; then
    echo "1. 🖥️  Для локального запуска: войдите в графическую сессию KDE"
    echo "2. 🌐 Для удаленного доступа через SSH:"
    echo "   ssh -X пользователь@orangepi_ip"
    echo "   ssh -Y пользователь@orangepi_ip  # для trusted connections"
    echo "3. 🖼️  Для VNC: установите и настройте VNC сервер"
    echo "4. 🖼️  Для виртуального дисплея:"
    echo "   sudo apt install xvfb"
    echo "   export DISPLAY=:99"
    echo "   Xvfb :99 -screen 0 1024x768x16 &"
fi

if [ "$(whoami)" = "root" ]; then
    echo "⚠️  Вы запускаете от root. Для GUI приложений лучше использовать обычного пользователя"
    echo "   Если нужен root доступ к дисплею: xhost +local:root"
fi
