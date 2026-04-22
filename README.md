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
=======
     Save to Cache
>>>>>>> 3ce2e6b (feat: raw pointers deleted, added PGresultPtr and PGconnPtr as smart pointers)
