CXX=g++
CXXFLAGS=-g -MMD
OBJECTS=FredZip.o ZipIO.o LZ77.o HuffmanTree.o Node.o SubtreeStack.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=FredZip

${EXEC}: ${OBJECTS}
		${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
		del ${OBJECTS} ${DEPENDS} ${EXEC}.exe