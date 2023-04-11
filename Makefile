CC=gcc
BUILD_DIR=build
SRC_DIR=src
INC_DIR=include
INCLUDE=-I$(INC_DIR)
LIBS=-lm -lMLV
CFLAGS=-fdiagnostics-color=always -Wall -pedantic -std=c17 -g -O0
DM_N=2
NOM_ZIP=TP$(DM_N)_SEBAN_ABDALLAH.zip
EXEC=demo
CONTENU_ZIP=$(SRC_DIR) $(INC_DIR) .clang-format .clang-tidy Makefile rapport.pdf

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)

# On récupère tous les fichiers sources .c, et on leurs préfixe
# le chemin de build, et suffixes en .o :
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
#$(info $(OBJS))

all: $(BUILD_DIR)/$(EXEC)

# Assemblage de l'exécutable final
$(BUILD_DIR)/$(EXEC): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

# Dépendances
demo.o: demo.c args.h scenes.h
generation.o: generation.c types.h
graphics.o: graphics.c graphics.h types.h
grille.o: grille.c grille.h
scenes.o: scenes.c scenes.h UI.h graphics.h generation.h quadtree.h
quadtree.o: quadtree.c quadtree.h
UI.o: UI.c UI.h grille.h
velocite.o: velocite.c velocite.h

# Création des fichiers objets à partir des fichiers sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

rapport: rapport.pdf

rapport.pdf: rapport.md
	@mkdir --parents logos
	@wget --quiet --show-progress --no-clobber -O logos/LogoLIGM.png "https://drive.google.com/uc?export=download&confirm=yes&id=1cZjxS6Rwp8LU4Eyahqz0eUS8aH0_VrVB" || true
	@wget --quiet --show-progress --no-clobber -O logos/namedlogoUGE.png "https://drive.google.com/uc?export=download&confirm=yes&id=1YGm1N7griuDbJhC6rSgBHrrcOsHKM5xg" || true
	pandoc --toc rapport.md -o rapport.pdf

format: $(SOURCES) $(HEADERS)
	clang-format -i --style=file $^
	clang-tidy --fix $^

clean:
	rm -f $(OBJS)

mrproper: clean
	rm -f $(BUILD_DIR)/$(EXEC)
	rm -f $(NOM_ZIP)

zip:
	zip -FSr $(NOM_ZIP) $(CONTENU_ZIP)

rendu:
	@$(MAKE) --no-print-directory rapport
	@$(MAKE) --no-print-directory zip

.PHONY: clean all
