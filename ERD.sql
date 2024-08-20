create table public.users
(
    id         bigserial,
    username   varchar(255),
    password   varchar(255),
    name       varchar(255),
    email      varchar(255),
    created_at timestamp,
    updated_at timestamp
);

create table public.roles
(
    id         bigserial,
    name       varchar(255),
    description varchar(255),
    created_at timestamp,
    updated_at timestamp
);

create table public.user_roles
(
    id      bigserial,
    user_id bigint,
    role_id bigint,
    created_at timestamp,
    updated_at timestamp
);

create table public.permissions
(
    id         bigserial,
    name       varchar(255),
    description varchar(255),
    created_at timestamp,
    updated_at timestamp
);

create table public.role_permissions
(
    id            bigserial,
    role_id       bigint,
    permission_id bigint,
    created_at    timestamp,
    updated_at    timestamp
);

create table public.projects (
    id bigserial,
    name varchar(255),
    description varchar(255),
    type varchar(255),
    pic_id bigint, -- User ID
    created_at timestamp,
    updated_at timestamp
);
