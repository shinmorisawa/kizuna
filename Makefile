CXX := g++
CXXFLAGS := -Iapp/include -Iinclude -Wall -Wextra -Os -flto -std=c++23

SRC_DIR := src
OBJ_DIR := obj
APP_SRC_DIR := app/src
APP_OBJ_DIR := app/obj
BUILD_DIR := build
ASSET_DIR := asset
TARGET := $(BUILD_DIR)/kizuna

APP_SRCS := $(wildcard $(APP_SRC_DIR)/*.cpp)
APP_OBJS := $(patsubst $(APP_SRC_DIR)/%.cpp,$(APP_OBJ_DIR)/%.o,$(APP_SRCS))
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

release: $(TARGET)
	strip $(TARGET)

upx: $(TARGET)
	upx -9 --ultra-brute $(TARGET)

ultra: release upx

$(TARGET): $(OBJS) $(APP_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) $(APP_OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(APP_OBJ_DIR)/%.o: $(APP_SRC_DIR)/%.cpp
	@mkdir -p $(APP_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean build
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
	rm -rf $(APP_OBJ_DIR)

install:
	@mkdir -p /var/www/kizuna
	cp ./$(TARGET) /usr/local/bin
	cp -r ./$(ASSET_DIR) /var/www/kizuna

.PHONY: all clean

