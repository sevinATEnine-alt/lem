TARGET_ARCH ?= native

ARCH = $(shell uname -s)

OSX_FLAGS ?=

ifeq ($(TARGET_ARCH), osx-x86_64)
	ifeq ($(ARCH),Darwin)
		CXX = clang++
		OSX_FLAGS = -target x86_64-apple-macos10.15
	else
		echo "Can only compile for OSX if on Darwin."
	endif
else ifeq ($(TARGET_ARCH), osx-arm64)
	ifeq ($(ARCH),Darwin)
		CXX = clang++
		OSX_FLAGS = -target arm64-apple-macos11
	else
		echo "Can only compile for OSX if on Darwin."
	endif
else
	CXX = g++
endif

ifeq ($(ARCH),Darwin)
	ifeq ($(TARGET_ARCH), osx-x86_64)
		CXXFLAGS = -std=c++17 -I./toml -I/usr/local/include/openssl/ -I/usr/local/opt/libarchive/include -I/usr/local/opt/curl/include
		LDFLAGS = -L/usr/local/Cellar/openssl@3/3.5.0/lib/ -lssl -lcrypto -lcurl -larchive -L/usr/local/opt/libarchive/lib
	else
		CXXFLAGS = -std=c++17 -I./toml -I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/libarchive/include
		LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -L/opt/homebrew/opt/curl/lib -L/opt/homebrew/opt/libarchive/lib -lssl -lcrypto -lcurl -larchive
	endif
else
	CXXFLAGS = -std=c++17 -I./toml -I/usr/include -I/usr/include/libarchive -I/usr/local/include/libarchive
	LDFLAGS = -L/usr/lib -L/usr/lib64 -lssl -lcrypto -lcurl -larchive
endif

SRC_DIR = src
OBJ_DIR = objects
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
TARGET = lem

all: $(TARGET)

$(TARGET): $(OBJ)
	echo "OSX_FLAGS: $(OSX_FLAGS) | CXXFLAGS: $(CXXFLAGS)"
	$(CXX) $(OBJ) $(LDFLAGS) -o $@ $(OSX_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OSX_FLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)