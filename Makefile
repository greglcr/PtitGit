petitGit: src/parser.cpp src/cmd/*.cpp src/cmd/*.h src/lib/*.cpp src/lib/*.h
	g++ -o petitGit src/parser.cpp src/cmd/init.cpp src/lib/object.cpp src/lib/hashing.cpp src/cmd/hash_object.cpp

tests: petitGit
	@python3 test/run-tests.py
