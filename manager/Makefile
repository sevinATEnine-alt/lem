CXX = g++
ARCH = $(shell uname -s)


ifeq ($(ARCH),Darwin)
	CXXFLAGS = -std=c++17 -I./toml -I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/libarchive/include
	LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -lcurl -larchive
else
	CXXFLAGS = -std=c++17 -I./toml -I/usr/include -I/usr/include/openssl -I/usr/local/include/openssl -I/usr/include/libarchive -I/usr/local/include/libarchive
	LDFLAGS = -L/usr/lib -L/usr/lib64 -L/usr/lib/libssl -lssl -lcrypto -lcurl -larchive
endif



SRC_DIR = src
OBJ_DIR = objects
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
TARGET = lem

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)