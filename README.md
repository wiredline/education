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
     Save to Cache
