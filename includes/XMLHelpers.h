#ifndef XML_HELPERS
#define XML_HELPERS

#include "pugixml.hpp"

pugi::xpath_node GetChildWithAttribute(pugi::xpath_node node, std::string childName, std::string attributeName, std::string attributeValue);
bool HasChildren(pugi::xpath_node node);
bool HasChild(pugi::xpath_node node, std::string name);

#endif