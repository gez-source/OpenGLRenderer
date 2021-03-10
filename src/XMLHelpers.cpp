#include "XMLHelpers.h"

pugi::xpath_node GetChildWithAttribute(pugi::xpath_node node, std::string childName, std::string attributeName, std::string attributeValue)
{
	pugi::xpath_node result;
	auto it = node.node().children(childName.c_str());

	for (pugi::xpath_node child : it)
	{
		auto attribute = child.node().attribute(attributeName.c_str());
		if (!attribute.empty())
		{
			std::string value = std::string(attribute.value());
			if (value == attributeValue)
			{
				return child;
			}
		}
	}

	// BUGGY
	//std::string query = "//" + childName + "[@" + attributeName + "='" + attributeValue + "']";
	//pugi::xpath_node child = node.node().select_node(query.c_str());
	return result;
}

bool HasChildren(pugi::xpath_node node)
{
	auto it = node.node().children();
	return (it.begin() != it.end());
}

bool HasChild(pugi::xpath_node node, std::string name)
{
	auto nodes = node.node().children(name.c_str());
	return std::distance(nodes.begin(), nodes.end()) > 0;
}