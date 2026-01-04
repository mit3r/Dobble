# Dobble

Projekt Dobble to implementacja gry karcianej Dobble w języku C++. Serwer gry powstał przy pomocy BSD Sockets API, a klient korzysta z biblioteki Qt6 do tworzenia interfejsu użytkownika, utworzonego w React. 

## Instrukcja budowania

### Wymagania wstępne

- CMake (wersja 3.10 lub nowsza)
- Kompilator C++ (np. GCC, Clang, MSVC)

Projekt wykorzystuje CMake jako system budowania. Poniżej znajdują się instrukcje dotyczące konfiguracji i budowania projektu.

Z poziomu katalogu głównego projektu:

- `cmake -S . -B build` - konfiguracja projektu i utworzenie katalogu build
- `cmake --build build` - zbudowanie całego projektu

- `cmake --build build --target dobble_lobbyserver` - zbudowanie lobbyserver
- `cmake --build build --target dobble_gameserver` - zbudowanie gameserver
- `cmake --build build --target dobble_client` - zbudowanie klienta

## Uruchamianie
Po zbudowaniu projektu, pliki wykonywalne będą znajdować się w katalogu `build`.

### Uruchamianie serwera

1. Przejdź do katalogu `build` (`dobble_gameserver` i `dobble_lobbyserver` muszą znajdować się w tym samym katalogu).
2. Uruchom serwer lobby:
  - `./dobble_lobbyserver <port> <ścieżka_do_pliku> <adres_ip>` <br>
  - np. `./dobble_lobbyserver 5000 /tmp/lobby_uds_socket 146.59.16.213`
3. Serwer będzie nasłuchiwał na podanym porcie i adresie IP.

### Uruchamianie klienta
  1. Przejdź do katalogu `build`.
  2. Uruchom klienta:
  - `./dobble_client`
  3. Interfejs użytkownika powinien się otworzyć, umożliwiając połączenie z serwerem.