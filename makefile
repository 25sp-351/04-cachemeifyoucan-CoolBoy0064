CC = gcc
CFLAGS = -g -Wall -lm
LDFLAGS =
OBJFILES = piece_values.o cut_list.o vec.o FIFOCache.o
OBJFILES2 = piece_values.o cut_list.o vec.o LRU_cache.o
TARGET = LRU_cache
TARGET2 = FIFOCache

all: $(TARGET) $(TARGET2)

$(TARGET): $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJFILES)

$(TARGET2): $(OBJFILES2)
	$(CC) $(LDFLAGS) -o $(TARGET2) $(OBJFILES2)

clean:
	rm -f $(OBJFILES) $(OBJFILES2) $(TARGET) $(TARGET2)