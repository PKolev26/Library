# Library C++ project for OOP course at FMI / Sofia University

## 📚 Информационна система за библиотека

Този проект представлява конзолно C++ приложение за управление на библиотека чрез команден ред. Позволява работа с книги и потребители, съхранение във файлове, вход и изход, сортиране, търсене, регистрация и други административни действия.

---

## ✨ Основни функционалности

- Зареждане и запис на книги във файл
- Добавяне, премахване и сортиране на книги
- Търсене по заглавие, автор и ключови думи
- Управление на потребители – вход, регистрация, роли
- Поддръжка на администраторски права
- Интерфейс за команди чрез `CommandFactory`
- Тестове с `doctest.h`

---

## 🧱 Структура на проекта

```
├── src/
│   ├── core/
│   │   ├── AppContext.h
│   │   ├── Book.h
│   │   ├── User.h
│   │   ├── Command.h
│   ├── interfaces/
│   │   ├── ICommand.h
│   ├── doctest/
│   │   └── doctest.h
│   ├── CommandFactory.h
│   ├── main.cpp
│   └── test_commands.cpp
├── data/
│   └── users.txt
```

---

## ⚙️ Инструкции за компилация

### 📌 Изисквания:
- g++ 9+ (или по-нов)
- C++17
- `doctest.h` (включен в `doctest/`)

### ▶️ Компилация на програмата:
```bash
1) g++ -Isrc -o library src/main.cpp
2) ./library
```

### 🧪 Стартиране на тестове:
```bash
1.1) g++ -std=c++17 -static-libgcc -static-libstdc++ -Isrc -o tests .\src\tests\test_commands.cpp
1.2) g++ -std=c++17 -static-libgcc -static-libstdc++ -Isrc -o tests .\src\tests\test_main_classes.cpp
2) ./tests
```

📌 Забележка: тестовете ще създават и изтриват временни файлове като `save_test_file.txt`, `open_test_book.txt`.

---

## 🛠 Бъдещо развитие

- Добавяне на `promote <username>` за повишаване в админ
- Поддръжка на `like`, `unlike`, `liked` команди
- Лична библиотека за всеки потребител
- Поддръжка на JSON или SQLite вместо .txt

---
