BUILD   = build
NAME	= $(BUILD)/main
FLAGS	= -g -Wall -Wextra -std=c++17 -Wshadow -Wno-shadow
GLAD	= external/glad/src/glad.c
GLADO	= $(BUILD)/glad.o
GLFWLIB	= external/glfw-3.4/build/src/libglfw3.a
LIBS	= ${GLFWLIB} -lm -ldl -lpthread -lX11 -lGL -lXrandr -lXi
INCLUDE	= -I./include -I./external -I./external/glad/include -I./external/glfw-3.4/include

GALLIUM = GALLIUM_HUD_PERIOD=0.05 GALLIUM_HUD=".dfps"

SRC		= src/main.cpp src/Shader.cpp\
			src/LineDrawer.cpp\
			src/Object.cpp\
			src/ObjectLoader.cpp\
			src/utils.cpp\
			src/Scop.cpp\
			src/InputManager.cpp\
			src/math.cpp\

FILE?=teapot

VALGRIND := valgrind --leak-check=full --suppressions=valgrind.supp
VALGRINDGEN := valgrind --leak-check=full --gen-suppressions=all

all: ${NAME}
	@echo "use make run FILE=filename"

run: all
	${GALLIUM} ./${NAME} resources/${FILE}.obj

vrun: all
	${GALLIUM} ${VALGRIND} ./${NAME} resources/${FILE}.obj

${GLADO}: ${GLAD} | $(BUILD)
	gcc -Wall -Iexternal/glad/include -c ${GLAD} -o ${GLADO}

${NAME}: ${SRC} ${GLADO}
	c++ ${INCLUDE} ${FLAGS} -o ${NAME} ${SRC} ${GLADO} ${LIBS}

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

fclean: clean

re: fclean all
