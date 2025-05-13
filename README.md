# 🧩 Distributed Sudoku Game System

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Grade](https://img.shields.io/badge/grade-19.7%2F20-brightgreen)](#grade)

---

## ✨ Project Overview

This project implements a distributed Sudoku game using a client-server architecture, developed for the Operating Systems course at the University of Madeira.  

**Grade Achieved:** 🏆 **19.7/20**  

**Supervisor:** Prof. Dr. Eduardo Marques

---

## 📦 Project Structure

```

├── cliente/
│   ├── cliente
│   ├── makefile
│   ├── cfg/
│   │   └── cliente.conf
│   ├── logs/
│   │   └── client.log
│   └── source/
│       ├── main.c
│       ├── handlers/
│       ├── jogo/
│       └── socket/
├── servidor/
│   ├── servidor
│   ├── makefile
│   ├── cfg/
│   │   └── server.conf
│   ├── database/
│   │   └── db.json
│   ├── jogos/
│   │   └── jogos.json
│   ├── logs/
│   │   └── logs.log
│   └── source/
│       ├── main.c
│       ├── handlers/
│       ├── libs/
│       ├── modosjogo/
│       └── socket/
└── .README
```


---

## 🚀 Features

- 🖥️ **Client-Server Architecture**: Real-time multiplayer Sudoku over TCP sockets.
- 👥 **Multiple Game Modes**: Supports several modes (A, B, C, etc.) including team play.
- 🔒 **Thread-Safe & Concurrent**: Handles multiple clients with threads and synchronization.
- 📝 **Logging**: Detailed logs for both client and server.
- 📊 **Statistics**: Server tracks and displays game, player, and team stats.
- ⚙️ **Configurable**: All settings via external config files.
- 🗄️ **JSON Storage**: Game data and stats stored in JSON for easy parsing.

---

## 🛠️ Build Instructions

### Server

```sh
cd servidor
make
```

### Client

```sh
cd cliente
make
```

---

## ▶️ How to Run

### Server

```sh
./servidor cfg/server.conf
```

### Client

```sh
./cliente cfg/cliente.conf
```

---

## ⚙️ Configuration

- **Server:** Edit `servidor/cfg/server.conf` to set the game file, port, log file, and game mode.
- **Client:** Edit `cliente/cfg/cliente.conf` to set the server IP, port, client ID, and log file.

---

## 📈 Grade

| Grade      | Supervisor                | Institution              |
|------------|---------------------------|--------------------------|
| 🏆 19.7/20 | Prof. Dr. Eduardo Marques | University of Madeira    |

---

## 👤 Author

- Marco Ferreira

---

## 📄 License

This project is for educational purposes.