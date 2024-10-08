/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "permissions" CASCADE;

CREATE TABLE "permissions" (
  "id" BIGSERIAL NOT NULL PRIMARY KEY,
  "code" VARCHAR(255) NOT NULL,
  "name" VARCHAR(255) NOT NULL,
  "role_id" BIGINT NOT NULL,
  "is_active" BOOLEAN NOT NULL,
  "created_at" TIMESTAMP NOT NULL,
  "updated_at" TIMESTAMP NOT NULL);

