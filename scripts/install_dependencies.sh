#!/bin/bash

# Sprawdź czy skrypt jest uruchomiony z uprawnieniami roota, jeśli nie, użyj sudo
SUDO=""
if [ "$EUID" -ne 0 ]; then
    SUDO="sudo"
fi

if command -v apt &> /dev/null; then
    echo "Wykryto apt. Instalowanie zależności SFML..."
    $SUDO apt-get update
    $SUDO apt-get install -y libsfml-dev
elif command -v zypper &> /dev/null; then
    echo "Wykryto zypper. Instalowanie zależności SFML..."
    $SUDO zypper install -y sfml-devel
else
    echo "Nie wykryto apt ani zypper. Proszę zainstalować SFML ręcznie."
    exit 1
fi
