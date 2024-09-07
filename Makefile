CC = gcc

# Executable Names
TARGETS = brain launch ur us

# Default target
all: $(TARGETS)

# Compile brain.c into brain executable
brain: brain.c
	$(CC) $(CFLAGS) -o brain brain.c

# Compile launch.c into launch executable
launch: launch.c
	$(CC) $(CFLAGS) -o launch launch.c

# Compile ur.c into ur executable
ur: ur.c
	$(CC) $(CFLAGS) -o ur ur.c

# Compile us.c into us executable
us: us.c
	$(CC) $(CFLAGS) -o us us.c

# Clean up build files
clean:
	rm -f $(TARGETS)

