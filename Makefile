CC=gcc
BUILD_DIR=build
SRC_DIR=src
INC_DIR=include
INCLUDE=-I$(INC_DIR)
LIBS=-lm -lMLV
CFLAGS=-fdiagnostics-color=always -Wall -pedantic -std=c17 -O2
NOM_ZIP=SEBAN_ABDALLAH.zip
EXEC=quadtree
CONTENU_ZIP=$(SRC_DIR) $(INC_DIR) .clang-format .clang-tidy Makefile Doxyfile README.md

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)

# On récupère tous les fichiers sources .c, et on leurs préfixe
# le chemin de build, et suffixes en .o :
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
#$(info $(OBJS))

all: $(EXEC)

# Assemblage de l'exécutable final
$(EXEC): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

# Dépendances
demo.o: demo.c args.h scenes.h
generation.o: generation.c types.h
graphics.o: graphics.c graphics.h types.h
scenes.o: scenes.c scenes.h graphics.h generation.h quadtree.h
quadtree.o: quadtree.c quadtree.h
velocite.o: velocite.c velocite.h

# Création des fichiers objets à partir des fichiers sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

format: $(SOURCES) $(HEADERS)
	clang-format -i --style=file $^
	clang-tidy --fix $^

clean:
	rm -f $(OBJS)

mrproper: clean
	rm -f ./$(EXEC)
	rm -f $(NOM_ZIP)

doc: $(SOURCES) $(HEADERS) Doxyfile
	@mkdir --parents doc
	doxygen

zip:
	zip -FSr $(NOM_ZIP) $(CONTENU_ZIP)

rendu:
	@$(MAKE) --no-print-directory rapport
	@$(MAKE) --no-print-directory zip

.PHONY: clean all
