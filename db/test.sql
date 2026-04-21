SELECT * FROM operations; -- main query

/* INSERT INTO operations (request, normalized_key, result, status)
VALUES (
    '{"operation":"+","args":[3,5]}',
    '3+5',
    8,
    0
);
calc_db=> select * from operations;
 id |              request               | normalized_key | result | status |         created_at         
----+------------------------------------+----------------+--------+--------+----------------------------
  1 | {"args": [3, 5], "operation": "+"} | 3+5            |      8 |      0 | 2026-04-21 08:11:59.097897
(1 row)
*/

