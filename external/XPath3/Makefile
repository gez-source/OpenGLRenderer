IDIR=./includes
ODIR=./obj
SDIR=./src

CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -pedantic -static
CXX_DEFINES = -DGLEW_STATIC -DTW_NO_DIRECT3D -DTW_NO_LIB_PRAGMA -DTW_STATIC -D_CRT_SECURE_NO_WARNINGS
CXX_INCLUDES = -I$(IDIR)

_OBJ = EvalEngine.o Text.o XPathQName.o ExprEval.o xh_scanner.o XPathSequence.o ExprFunctions.o XmlParser.o XPathStaticEnv.o ExprParser.o XmlParserMemoryPool.o XPathTerm.o ExprToken.o XPathDynamicEnv.o XPathVar.o ExprTree.o XPathItem.o XSDType.o ExprTreeNode.o XPathNode.o 

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

bin/static/libxpath3.a: $(OBJ)
	$(shell mkdir -p bin/static)\
	ar rcs bin/static/libxpath3.a $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(shell mkdir -p obj)\
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(CXX_INCLUDES) $(CXX_DEFINES) 	

.PHONY: clean

clean:
	rm -f bin/static/libxpath3.a \
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ \
	rm -rf obj\
	rm -rf bin




