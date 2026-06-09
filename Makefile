run:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ./build/localtv

migrate:
	cmake -B build && cmake --build build && ./build/localtv_migration

reset:
	cmake -B build && cmake --build build && ./build/localtv_migration reset

setup:
	perl setup.pl
