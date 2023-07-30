CROSS_COMPILE = /usr/local/gp2xdev/bin/arm-linux-
SDL_BASE = /usr/local/gp2xdev/bin/arm-linux-
LDFLAGS = -static

CC = $(CROSS_COMPILE)g++
CXX = $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip

CFLAGS = -I/usr/local/gp2xdev/include/SDL -O2 -Wall
CXXFLAGS = -I/usr/local/gp2xdev/include/SDL -DTARGET_GP2X -O2 -Wall
LIBS = -L/usr/local/gp2xdev/lib -lSDL_image -lpng12 -lz -ljpeg -lm -ldl -lSDL_mixer -lvorbisidec -lmikmod -lsmpeg -lSDL -lpthread
TARGET = beat2x.gpe
OBJS = main.o Engine.o Game.o Menu.o MenuMain.o MenuHiscores.o MenuSettings.o MenuChoosePack.o MenuChooseLevel.o MenuEnterInitials.o Song.o ButtonEvent.o SongStatistics.o ScoresManager.o InputHandler.o FPSCounter.o TextManager.o ThemeManager.o Settings.o usbjoy.o

ALL_TARGETS = $(TARGET)

all: $(ALL_TARGETS)

$(TARGET): $(OBJS)
	   $(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	   $(STRIP) $(TARGET)

clean:
		rm -f $(ALL_TARGETS) *.o *~
