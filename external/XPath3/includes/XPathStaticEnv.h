#ifndef XPATH_STATIC_ENV
#define XPATH_STATIC_ENV

#include <string>
#include <vector>
#include <map>

#include "XSDType.h"
#include "XPathVar.h"
#include "EvalEngine.h"
#include "XPathFunction.h"

using namespace std;

//NB: have made functions in this structure, so that functions
//from different namespaces can share the same record in the
//Function member...this might make the search for functions
//more effective.
//CXPathStaticEnv
//	.Functions (1..*) 
//		-> Signatures (1..*)
//				-> Function
//					.Namespace
//					.LocalName
//					.ReturnType (1)
//						-> CXSDType
//							.Namespace
//							.LocalName
//							.Definition
//					.Arity (0..*)
//						-> CXSDType
//	...






//A function with a name can be over-ridden.
struct XPathFuncSigs {
	vector<XPathFunction*> Signatures;
};

typedef pair <string, XPathFuncSigs*> String_FuncSignature_Pair;
typedef pair <string,CXSDType*> String_Type_Pair;

class CXPathStaticEnv{
public:
	CXPathStaticEnv(void);
	virtual ~CXPathStaticEnv(void);

	//all types available to the expression;
	map<string, CXSDType*> Types;

	//See all variable info in dynamic environment

	//allows multiple function of the same name
	//differing only in the number and type
	//of the arguments.
	map<string, XPathFuncSigs*> Functions;

	void Init();
	void InitTypes();
	void InitFunctions();

	XPathFunction* GetFunction(string ns, string name, XPathFuncArity* arity);

};

#endif
