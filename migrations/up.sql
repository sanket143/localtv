create table if not exists global_context (
    id integer default 0,
    start_epoch bigint not null,

    primary key (id)
);

insert into global_context (id, start_epoch) values (0, 1781025636);

create table if not exists channel (
    id integer primary key autoincrement,
    name varchar(100) not null,
    is_deleted bool default false not null
);

-- So there's just one migration file, and we add all the migraitons in this single file.
-- Why do people even do stuff in multiple files anyway?
insert into channel(id, name) values (1, 'Respawn TV') on conflict (id) do nothing;
insert into channel(id, name) values (2, 'GlitchNet') on conflict (id) do nothing;
insert into channel(id, name) values (3, 'Zero-G Games') on conflict (id) do nothing;
insert into channel(id, name) values (4, 'Velocity X') on conflict (id) do nothing;
insert into channel(id, name) values (5, 'NeoPrime') on conflict (id) do nothing;
insert into channel(id, name) values (6, 'Zento Max') on conflict (id) do nothing;

create table if not exists video (
    id varchar(100) not null,
    title varchar(100) not null,
    description varchar(1024) null,
    duration integer not null,
    is_deleted bool default false not null,

    primary key (id)
);

create table if not exists schedule (
    id integer primary key autoincrement,
    fk_channel_id integer not null,
    fk_video_id varchar(100) not null,
    created_at datetime default current_timestamp,

    unique(fk_channel_id, fk_video_id)
);

-- testing videos in schedule
insert into schedule(fk_channel_id, fk_video_id) values
    (1, 'W7y7wNB-NZc'),
    (1, 'SBFB1ELmXHI'),
    (1, 'NIqp91dN6fk')
;

insert into video(id, title, description, duration) values
    ('W7y7wNB-NZc', 'WE SO BACK - GeoGuessr Game Of The Week #10', null, 1261),
    ('SBFB1ELmXHI', 'GEOGUESSR TOURNAMENT HIGHLIGHTS #9', null, 925),
    ('NIqp91dN6fk', 'LOCKED IN - GeoGuessr Game Of The Week #9', null, 1756)
;

-- Respawn TV
-- - https://youtu.be/W7y7wNB-NZc?si=uNxxBIJQsXyNuIE3 
-- - https://youtu.be/SBFB1ELmXHI?si=J2vfpJ0sWjSIXlGv 
-- - https://youtu.be/NIqp91dN6fk?si=_LgMIpAfsx7JWfgt 
-- - https://www.youtube.com/watch?v=5RRDVsWHTKM
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
--
-- Velocity X
-- - https://youtu.be/2nD0Dk7R5IE?si=oQ1B6pCwqi4gwFEe 
-- - https://youtu.be/wt7LbWWSYpI?si=x29bgEebFw9Uynqu 
-- - https://youtu.be/uGAV89Qrr8Q?si=lMgZiEZsHcJ-2zo9 
-- - https://youtu.be/HAj0ibTy3gw?si=VeQ7Qt3Rkh67XUd- 
-- - https://youtu.be/VrORfPucQVo?si=_GMfCDTR-v91izQK 
-- - https://youtu.be/C45_zPRNcdY?si=GhvxQG2175G0vV5t 
-- - https://www.youtube.com/live/EXwof-6LwDM?si=QpJCc6umskC22MCt 
