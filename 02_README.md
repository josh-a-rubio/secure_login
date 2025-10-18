# SecureLogin

A lightweight, secure command-line authentication system written in C that demonstrates fundamental security practices including password hashing, salting, and input validation.

---

## 🔐 Features

- **Password Hashing** — `djb2`-based hash with a unique per-user salt  
- **Salt Generation** — 8-character pseudo-random salt  
- **Input Validation** — bounds checking and sanitization to reduce overflow/injection risk  
- **Username Enumeration Protection** — generic failure messages to avoid leaking valid usernames  
- **Memory Security** — password buffers cleared from memory immediately after use  
- **File Permissions**  
  - Unix: credentials file created with `0600` permissions  
  - Windows: credentials file is hidden  
- **Session Controls** — only one account may be created per program execution  
- **Brute-Force Resistance** — program exits after a login attempt to limit rapid retries

---

## 📋 Requirements

- **Unix / Linux / macOS:** GCC or Clang  
- **Windows:** MinGW (feature support may be limited)  
- Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, etc.)

---

## 🚀 Build & Run

### Compile
```bash
gcc -o securelogin 01_securelogin.c -Wall -Wextra
```

### Run (macOS / Linux)
```bash
./securelogin
```

### Run (Windows)
```bash
securelogin.exe
```

---

## 💡 Example Session

### Sign Up
Choice: 2  
Username: john_doe  
Password: ********  
Account created successfully!

### Login
Choice: 1  
Username: john_doe  
Password: ********  

✓ LOGIN SUCCESSFUL!  
Welcome, john_doe

### Failed Login
Username: wrong_user  
Password: ********  
Login failed: Invalid username or password.

---

## 🏗️ Design Overview

### Security Principles
- **Defense in Depth** — multiple validation layers and safe defaults  
- **Least Privilege** — file permission restrictions for credential storage  
- **Secure by Default** — generic error messages, immediate clearing of secrets, session-based limits  
- **Input Sanitization** — all user input validated before processing

### Architecture Choices

#### Why `djb2`?
- Simple and fast for educational purposes  
- ❗ Not secure enough for production — prefer Argon2 / bcrypt / scrypt

#### Why flat-file storage (`users.txt`)?
- Portable and easy to inspect/debug  
- Teaches safe file I/O in C  
- ❗ Not recommended for production — use encrypted databases and proper secrets management

---

## 🧩 Function Layout

| Category       | Example Functions                        |
|----------------|------------------------------------------|
| UI / UX        | `user_greeting()`, `user_menu()`          |
| Authentication | `user_signup()`, `user_login()`          |
| Validation     | `validate_username()`, `username_exists()`|
| Crypto         | `generate_salt()`, `toy_hash()`           |

---

## 📁 Repository Structure

```
securelogin/
├── 01_securelogin.c   # Main source code
├── 02_README.md       # This file
├── 03_LICENSE         # MIT License
└── users.txt          # Generated at runtime (ignored)
```

---

## `users.txt` Format

Credentials are stored as colon-separated entries:

```
username:salt:hash
```

Example:

```
john_doe:aB3xK9mQ:17364821749123456789
alice123:pL5nR2wY:98765432101234567890
```

> **Important:** `users.txt` contains simulated hashes for demo/learning only. Do **not** commit it.

---

## ⚠️ Security Notice

This project is for **learning and demonstration only**.  
It is **not** production-grade. Replace toy hashing with Argon2 / bcrypt / scrypt, use encrypted storage, TLS, and robust authentication for real applications.

---

## 📄 License

MIT License — see `03_LICENSE` for details.
