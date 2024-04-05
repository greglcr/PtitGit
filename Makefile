petitGit: src/parser.cpp src/cmd/init.cpp src/cmd/init.h
	g++ -o petitGit src/parser.cpp src/cmd/init.cpp

tests: petitGit
	@python3 test/run-tests.py
