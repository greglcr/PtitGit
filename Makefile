petitGit: src/parser.cpp src/cmd/*.cpp src/cmd/*.h src/lib/*.cpp src/lib/*.h
	g++ -o petitGit src/parser.cpp src/cmd/init.cpp src/lib/object.cpp src/lib/hashing.cpp src/cmd/hash_object.cpp src/lib/object_file.cpp src/lib/object_tree.cpp -lssl -lcrypto

tests: petitGit
	@python3 test/run-tests.py
