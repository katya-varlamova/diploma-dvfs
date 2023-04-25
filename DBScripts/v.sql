create table if not exists Data (
    id integer primary key autoincrement,
    cpuportion integer,
    attempt integer,
    cpufreq integer,
    ct double precision,
    power double precision,
    ipc double precision,
    mrpi double precision,
    inst double precision,
    cycles double precision,
    l3 double precision);

create table if not exists SleepData (
    id integer primary key autoincrement,
    rt integer,
    attempt integer,
    cpufreq integer,
    ct double precision,
    power double precision,
    ipc double precision,
    mrpi double precision,
    inst double precision,
    cycles double precision,
    l3 double precision);

