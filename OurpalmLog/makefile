CC = g++
CPPFLAGS = -m64 -fPIC -fpermissive
logsender: ini.cpp json.hpp logsender.cpp main.cpp
	$(CC) $(CPPFLAGS) -std=c++11 *.cpp -luuid -lcurl -lpthread -g -O2 -o $@
clean:
	rm logsender 
