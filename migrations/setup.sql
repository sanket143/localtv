create table if not exists global_context (
    id int default 0,
    start_epoch bigint,

    primary key (id)
);

insert into global_context (id, start_epoch) values (0, 1781025636);

create table if not exists channel (
    id serial,
    name varchar(100),
    is_deleted bool default false,

    primary key (id)
);

-- So there's just one migration file, and we add all the migraitons in this single file.
-- Why do people even do stuff in multiple files anyway?
insert into channel(id, name) values (1, 'Respawn TV') on conflict (id) do nothing;
insert into channel(id, name) values (2, 'GlitchNet') on conflict (id) do nothing;
insert into channel(id, name) values (3, 'Zero-G Games') on conflict (id) do nothing;

create table if not exists video (
    id varchar(100),
    title varchar(100),
    duration integer,
    is_deleted bool default false,

    primary key (id)
);

create table if not exists schedule (
    id serial,
    fk_channel_id int,
    fk_video_id varchar(100),
    display_sequence serial default 0,

    primary key (id)
);

-- testing videos in schedule
insert into schedule(fk_channel_id, fk_video_id, display_sequence) values
    (1, 'W7y7wNB-NZc', 0),
    (2, 'SBFB1ELmXHI', 1),
    (3, 'NIqp91dN6fk', 2)
;

-- Respawn TV
-- - https://youtu.be/W7y7wNB-NZc?si=uNxxBIJQsXyNuIE3 
-- - https://youtu.be/SBFB1ELmXHI?si=J2vfpJ0sWjSIXlGv 
-- - https://youtu.be/NIqp91dN6fk?si=_LgMIpAfsx7JWfgt 
-- - https://www.youtube.com/live/5RRDVsWHTKM?si=gEG9C5b1jBa6bH1q
--
-- GlitchNet
-- - https://www.youtube.com/playlist?list=PLs1-UdHIwbo6PGYwjCWzrnAUZ7AkeCc95 
-- - https://www.youtube.com/playlist?list=PLs1-UdHIwbo7kCKGfol9f4T1hvwE2nntG 
-- - https://www.youtube.com/playlist?list=PLs1-UdHIwbo6PhaqfzORhDfwbDl2K-QgI 
-- 
-- Zero-G Games
-- - https://youtu.be/lnGvPESpmjY?si=dQ0L24x27eTho-LM 
-- - https://youtu.be/0tRbebJ0uUE?si=RPeP7S6olbw8wtT9
-- - https://www.youtube.com/playlist?list=PLTCabuBw1RkvTmJrOgdoG_wpRuOgE-_Lb 

