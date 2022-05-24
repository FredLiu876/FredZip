CXX=g++
CXXFLAGS=-g -MMD
OBJECTS=FredZip.o HuffmanTree.o IndexList.o node.o SubtreeStack.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=FredZip

${EXEC}: ${OBJECTS}
		${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
		del ${OBJECTS} ${DEPENDS} ${EXEC}.exe