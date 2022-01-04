#ifndef OTHER_CONSTANTS_HPP
#define OTHER_CONSTANTS_HPP

#include "ConstantBase.hpp"

#define CONSTANT_SINGLE_INDEX(NAME, INDEX_NAME)                                    \
class Constant##NAME : public ConstantBase 									       \
{ 																			       \
public: 																		   \
	Constant##NAME(const uint16_t idx) : INDEX_NAME##_index(idx) {} 			   \
 																				   \
	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_##NAME; }  \
	uint16_t get_##INDEX_NAME##_index() const { return INDEX_NAME##_index; } 	   \
 																			       \
private: 																		   \
	const uint16_t INDEX_NAME##_index; 											   \
}; 																			       \


#define CONSTANT_TWO_INDICES(NAME, INDEX_1_NAME, INDEX_2_NAME)                     \
class Constant##NAME : public ConstantBase 									       \
{ 																			       \
public: 																		   \
	Constant##NAME(const uint16_t idx1, const uint16_t idx2)                       \
            : INDEX_1_NAME##_index(idx1), INDEX_2_NAME##_index(idx2) {} 		   \
 																				   \
	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_##NAME; }  \
	uint16_t get_##INDEX_1_NAME##_index() const { return INDEX_1_NAME##_index; }   \
	uint16_t get_##INDEX_2_NAME##_index() const { return INDEX_2_NAME##_index; }   \
 																			       \
private: 																		   \
	const uint16_t INDEX_1_NAME##_index; 										   \
    const uint16_t INDEX_2_NAME##_index;                                           \
}; 																			       \

CONSTANT_SINGLE_INDEX(Class, name)
CONSTANT_SINGLE_INDEX(String, string)
CONSTANT_TWO_INDICES(Fieldref, class, name_and_type)
CONSTANT_TWO_INDICES(Methodref, class, name_and_type)
CONSTANT_TWO_INDICES(InterfaceMethodref, class, name_and_type)
CONSTANT_TWO_INDICES(NameAndType, name, descriptor)
CONSTANT_SINGLE_INDEX(MethodType, descriptor)
CONSTANT_TWO_INDICES(Dynamic, bootstrap_method_attr, name_and_type)
CONSTANT_TWO_INDICES(InvokeDynamic, bootstrap_method_attr, name_and_type)
CONSTANT_SINGLE_INDEX(Module, name)
CONSTANT_SINGLE_INDEX(Package, name)

#endif // OTHER_CONSTANTS_HPP