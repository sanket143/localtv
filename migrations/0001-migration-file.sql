create table if not exists channel (
    id serial,
    name varchar(100),
    is_deleted bool default false,

    primary key (id)
);

-- So there's just one migration file, and we add all the migraitons in this single file.
-- Why do people even do stuff in multiple files anyway?
insert into channel(id, name) values (1, 'YouNeverKnow') on conflict (id) do nothing;
