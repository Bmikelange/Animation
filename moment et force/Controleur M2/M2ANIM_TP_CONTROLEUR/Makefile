# Copyleft 🄯
# Par contre, n'hésitez pas à me payer un café quand Nautibus ré-ouvrira

# # Compiler et exécuter le TP contrôleur de mouvement sous Linux
#
# ## Installer GLFW et GLEW
#
# Sous Debian : apt install libglfw3-dev libglew-dev
# Sinon à voir avec votre gestionnaire de paquets
#
# ## Modifier les fichiers sources et Compiler les librairies
#
# Cette opération n'est qu'à faire une seule fois.
#
# `make once_upon_a_time`
#
## Compiler le projet
#
# `make`
#
# ## Exécuter l'exécutable (wow)
#
# `./a.out`
#
# ## Nettoyer le projet seul
#
# `make clean`
#
# ## Nettoyer aussi les librairies
#
# `make fullclean`
#

.DEFAULT_GOAL := main

IMGUI_CPP_DIR = Box2D/src/imgui
IMGUI_O_DIR = Box2D/obj/Release/imgui
IMGUI_CPP = $(wildcard $(IMGUI_CPP_DIR)/*.cpp)
IMGUI_O = $(subst $(IMGUI_CPP_DIR), $(IMGUI_O_DIR), $(patsubst %.cpp, %.o, $(IMGUI_CPP)))
IMGUI_AR = Box2D/lib/linux/libIMGUI_d.a

BOX2D_CPP_DIR = Box2D/src/Box2D
BOX2D_O_DIR = Box2D/obj/Release/Box2D
BOX2D_CPP = $(shell find $(BOX2D_CPP_DIR) -iname *.cpp)
BOX2D_O = $(subst $(BOX2D_CPP_DIR), $(BOX2D_O_DIR), $(patsubst %.cpp, %.o, $(BOX2D_CPP)))
BOX2D_AR = Box2D/lib/linux/libBox2D_d.a

MAIN_CPP_DIR = src
MAIN_O_DIR = obj/Debug
MAIN_CPP = $(shell find src -iname *.cpp)
MAIN_O = $(subst $(MAIN_CPP_DIR), $(MAIN_O_DIR), $(patsubst %.cpp, %.o, $(MAIN_CPP)))

MAIN_LIBS = -LBox2D/lib/linux -lBox2D_d -lGLEW -lglfw -lIMGUI_d -lGL -lGLU

$(IMGUI_O_DIR):
	mkdir -p $@

$(IMGUI_O_DIR)/%.o: $(IMGUI_CPP_DIR)/%.cpp
	g++ -Wall -O2 -std=c++11 -DGLEW_STATIC -IBox2D/src -c $< -o $@

$(BOX2D_O_DIR):
	mkdir -p $@
	mkdir -p $(subst $(BOX2D_CPP_DIR), $(BOX2D_O_DIR), $(shell find $(BOX2D_CPP_DIR) -type d))

$(BOX2D_O_DIR)/%.o: $(BOX2D_CPP_DIR)/%.cpp
	g++ -Wall -O2 -std=c++11 -IBox2D/src -c $< -o $@

compile_imgui: $(IMGUI_O_DIR) $(IMGUI_O)
	rm -f $(IMGUI_AR) && ar -rs $(IMGUI_AR) $(IMGUI_O)

compile_box2d: $(BOX2D_O_DIR) $(BOX2D_O)
	rm -f $(BOX2D_AR) && ar -rs $(BOX2D_AR) $(BOX2D_O)

$(MAIN_O_DIR):
	mkdir -p $@
	mkdir -p $@/Framework

$(MAIN_O_DIR)/%.o: $(MAIN_CPP_DIR)/%.cpp $(MAIN_O_DIR)
	g++ -g -Wall -std=c++11 -DGLEW_STATIC -IBox2D/src -Isrc -c $< -o $@

main: $(MAIN_O)
	g++ $^ $(MAIN_LIBS) -o ./a.out


once_upon_a_time: compile_imgui compile_box2d
	@echo "La configuration est terminée !"

clean:
	rm -rf obj/ *.out imgui.ini

fullclean: clean
	rm -rf Box2D/obj/
