CXX = g++
CXXFLAGS = -g
EXEC = dns_parser
OBJS = sections.o main.o utils.o
# List of object files needed to
# build the executable.
${EXEC}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJS}

sections.o: sections.cpp
	${CXX} ${CXXFLAGS} -c sections.cpp

utils.o: utils.cpp
	${CXX} ${CXXFLAGS} -c utils.cpp

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp

clean:
	rm -f ${EXEC} ${OBJS}
