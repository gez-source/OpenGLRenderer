#ifndef EXPR_TREE_NODE
#define EXPR_TREE_NODE

#include "ExprToken.h"
#include "XPathDynamicEnv.h"
#include "XPathFunction.h"
#include "XPathEnums.h"
#include <vector>
#include <string>

using namespace std;

typedef pair <string, CExprToken*> Pattern_Token_Pair;

struct XPathTokenInfo {
	union  {
		int iVal;
		double dVal;
		string *sVal;
		XPathFunction *funcVal;
	} Value;
	string Type;
};

struct XPathTokenEntry{
	XPathTokenEntry(Pattern_Token_Pair *t):
			Token(t){};
	Pattern_Token_Pair *Token;
	vector<XPathTokenInfo> Info;
};

class CExprTreeNode
{
public:
	CExprTreeNode(void);
	CExprTreeNode(XPathSyntaxNodeKind kind);
	virtual ~CExprTreeNode(void);

	XPathSyntaxNodeKind Kind;
	XPathTokenEntry *Token;
	vector<CExprTreeNode*> Children;
    vector<CExprTreeNode*> Siblings;

	bool IsExprStart;
	bool IsRoot;

	XPathTokenName GetAxisName(void);

	void Traverse();
	void Visit();

	void TestTraverse();
	void TestVisit();
};

#endif
