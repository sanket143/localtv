run:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ./build/stv

migrate:
	cmake -B build && cmake --build build && ./build/stv_migration
