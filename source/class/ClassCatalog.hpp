#ifndef CLASS_CATALOG_HPP
#define CLASS_CATALOG_HPP

#include "ClassRepresentation.hpp"

#include <unordered_map>

class ClassCatalog
{
public:
	ClassRepresentation& operator[](const std::string& class_name)
	{
		return classes[class_name];
	}
private:
	std::unordered_map<std::string, ClassRepresentation> classes;
};

#endif // CLASS_CATALOG_HPP