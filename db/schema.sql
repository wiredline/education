CREATE TABLE operations (
    id SERIAL PRIMARY KEY,
    request JSONB NOT NULL,
    normalized_key TEXT NOT NULL UNIQUE,
    result INTEGER,
    status INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_operations_request ON operations USING GIN (request);