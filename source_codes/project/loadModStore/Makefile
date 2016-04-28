all: loadModStore.so LMStest.so findShared.so useCount.so allBranchCount.so getThreadFunc.so funcSVUseCount.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

%.so: %.o
	$(CXX) -dylib -shared $^ -o $@
clean:
	rm -f *.o *~ *.so
