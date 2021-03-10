#ifndef XPATHFUNCTION_H
#define XPATHFUNCTION_H

#include "XSDType.h"
#include "XPathVar.h"
#include "EvalEngine.h"
#include "XPathStaticEnv.h"
 
struct XPathFuncArity : public std::vector<CXSDType*> {
	~XPathFuncArity(){
		for(iterator it=begin();it!=end();it++)
			delete *it;
	}
public:
	bool operator== (XPathFuncArity other);
};



struct _XPathFunction;
 typedef _XPathFunction XPathFunction;


class CEvalEngine;

typedef CXPathSequence* (CEvalEngine::*PFunction)(CXPathSequence* input, XPathFunction* func);

struct _XPathFunction;

typedef struct _XPathFunction {
	_XPathFunction(string ns, string ln, CXSDType *rt): Namespace(ns), LocalName(ln), ReturnType(rt){}
	string Namespace; //this should point to an inscope ns
	string LocalName; 
	CXSDType *ReturnType;
	XPathFuncArity Arity;
    PFunction PFunc; 
} XPathFunction;

#endif
