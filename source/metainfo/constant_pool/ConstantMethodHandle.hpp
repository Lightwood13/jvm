#ifndef CONSTANT_METHOD_HANDLE_HPP
#define CONSTANT_METHOD_HANDLE_HPP

#include "ConstantBase.hpp"
#include <format>

class ConstantMethodHandle : public ConstantBase
{
public:
	enum class ReferenceKind : uint8_t 
	{
		REF_getField = 1,
		REF_getStatic = 2,
		REF_putField = 3,
		REF_putStatic = 4,
		REF_invokeVirtual = 5,
		REF_invokeStatic = 6,
		REF_invokeSpecial = 7,
		REF_newInvokeSpecial = 8,
		REF_invokeInterface = 9
	};

	ConstantMethodHandle(uint8_t reference_kind, uint16_t reference_index)
		: reference_kind(from_u1(reference_kind)), reference_index(reference_index) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_MethodHandle; }
	ReferenceKind get_reference_kind() const { return reference_kind; }
	uint16_t get_reference_index() const { return reference_index; }

private:
	const ReferenceKind reference_kind;
	const uint16_t		reference_index;

	static ReferenceKind from_u1(uint8_t reference_kind) {
		if (reference_kind < 1 || reference_kind > 9)
			throw std::logic_error(std::format("Unresolved reference kind: {}", reference_kind));

		return static_cast<ReferenceKind>(reference_kind);
	}
};

#endif // CONSTANT_METHOD_HANDLE_HPP