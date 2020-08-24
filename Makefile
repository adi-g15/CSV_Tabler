INCLUDES=includes
SOURCES=src
BUILD_DIR=build
TEST_DIR=test
DEBUG_FLAGS= -g 
CXX=g++
DBG=gdb

main: ${BUILD_DIR} ${BUILD_DIR}/main.o ${BUILD_DIR}/collection.o
	${CXX} ${BUILD_DIR}/main.o ${BUILD_DIR}/collection.o -o main

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/main.o: main.cpp
	${CXX} -c main.cpp -I${INCLUDES} -o ${BUILD_DIR}/main.o

${BUILD_DIR}/collection.o: ${SOURCES}/collection.cpp
	${CXX} -c ${SOURCES}/collection.cpp -I${INCLUDES} -o ${BUILD_DIR}/collection.o

test: ${BUILD_DIR} ${BUILD_DIR}/collection.o ${TEST_DIR}/test.cpp
	${CXX} ${TEST_DIR}/test.cpp ${BUILD_DIR}/collection.o -I${INCLUDES} -o ${TEST_DIR}/test.out
	${TEST_DIR}/test.out

debugT: ${BUILD_DIR} ${BUILD_DIR}/collection.o ${TEST_DIR}/test.cpp
	${CXX} ${TEST_DIR}/test.cpp ${BUILD_DIR}/collection.o -I${INCLUDES} ${DEBUG_FLAGS} -o ${TEST_DIR}/test.out
	${DBG} ${TEST_DIR}/test.out	

clean:
	rm -r test.out ${BUILD_DIR}
