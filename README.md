# education

Публичный проект для обучения куда будут скидываться материалы для отчетов
Версия ООП с spdlog и gtest

JSON input
   ↓
Parse
   ↓
Validate
   ↓
MakeKey
   ↓
Cache lookup
   ↓
   ├─ HIT → return
   └─ MISS
         ↓
     Calculate
         ↓
     Save to DB (JSON + key + result + status)
         ↓
<<<<<<< HEAD
     Save to Cache

TCP thread (asio)
    ↓
std::queue<std::string> tasks <- legacy input from cmd
    ↓
Worker thread
    ↓
Runner
    ↓
response back to client

127.0.0.1:55555 TCP listen