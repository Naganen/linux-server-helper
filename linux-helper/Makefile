CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Iinclude -pthread
TARGET = linux-server-helper
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	install -d /usr/local/bin
	install -m 755 $(TARGET) /usr/local/bin/$(TARGET)
	install -d /etc/linux-server-helper
	if [ ! -f /etc/linux-server-helper/config.json ]; then \
		install -m 600 config.json.example /etc/linux-server-helper/config.json; \
	fi

.PHONY: all clean install

