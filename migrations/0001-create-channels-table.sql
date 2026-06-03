create table if not exists channel (
    id serial,
    name varchar(100),
    is_deleted bool default false,

    primary key (id)
);
