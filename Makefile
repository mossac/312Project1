# For sockets project.  
# From: https://app.box.com/v/dcc10e
#
#	apt install gcc make

CFLAGS = -pthread
FILES = chatClient forkServer ThreadServer socketsClient socketsServer

all: $(FILES)

clean:
	rm $(FILES)
