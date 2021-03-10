#ifndef XPATH_QNAME
#define XPATH_QNAME

#include <string>

using namespace std;

class CXPathQName {
public:
	CXPathQName(void);
	CXPathQName(string name, string ns = "");

	string Namespace;
	string LocalName;

	bool operator==(CXPathQName other);
	bool operator<(CXPathQName other);
	bool operator>(CXPathQName other);
};
#endif
