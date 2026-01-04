#!/bin/bash

# Sprawdź czy skrypt jest uruchomiony z uprawnieniami roota, jeśli nie, użyj sudo
SUDO=""
if [ "$EUID" -ne 0 ]; then
    SUDO="sudo"
fi

if command -v zypper &> /dev/null; then
    echo "Wykryto zypper. Instalowanie zależności z Dockerfile..."
    $SUDO zypper refresh
    $SUDO zypper install -y \
        gcc13 \
        gcc13-c++ \
        make \
        cmake \
        git \
        qt6-base-devel \
        qt6-base-common-devel \
        qt6-webenginecore-devel \
        qt6-webenginewidgets-devel \
        libatomic1 \
        libQt6Core6 \
        libQt6Widgets6 \
        libQt6WebEngineCore6 \
        libQt6WebEngineWidgets6 \
        libQt6Gui6 \
        libQt6DBus6 \
        libQt6Network6 \
        nodejs20 \
        npm20 \
        glibc-devel
    
    echo "Eksportowanie zmiennych środowiskowych dla GCC 13..."
    export CC=/usr/bin/gcc-13
    export CXX=/usr/bin/g++-13
elif command -v apt &> /dev/null; then
    echo "Wykryto apt. Instalowanie odpowiedników zależności z Dockerfile..."
    $SUDO apt-get update
    $SUDO apt-get install -y \
        gcc-13 \
        g++-13 \
        make \
        cmake \
        git \
        qt6-base-dev \
        qt6-webengine5-dev \
        libqt6webenginewidgets6 \
        libatomic1 \
        nodejs \
        npm
    
    echo "Eksportowanie zmiennych środowiskowych dla GCC 13..."
    export CC=/usr/bin/gcc-13
    export CXX=/usr/bin/g++-13
else
    echo "Nie wykryto apt ani zypper. Proszę zainstalować zależności ręcznie."
    exit 1
fi
