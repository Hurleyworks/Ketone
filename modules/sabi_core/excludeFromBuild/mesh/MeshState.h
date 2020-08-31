// This header file was auto-generated by ClassMate++
// Created: 16 Sep 2017 5:37:16 pm
// Copyright (c) 2017, HurleyWorks

#pragma once
static const char* MeshStateTable[] =
{
	"Pickable",
	"Selected",
	"MeshChanged",
	"Invalid"
};

struct MeshState
{
	enum EMeshState
	{
		Pickable = 1 << 0,
		Selected = 1 << 1,
		MeshChanged = 1 << 2,
		Invalid = 1 << 3
	};

	union
	{
		EMeshState name;
		unsigned long value;
	};

	MeshState(EMeshState name) : name(name) {}
	MeshState(unsigned long value) : value(value) {}
	MeshState() : value(0) {}
	void clear() { value = 0; }
	operator EMeshState() const { return name; }
	unsigned long operator|=(MeshState m) { return value |= m; }
	unsigned long operator&=(MeshState m) { return value &= m; }
	unsigned long operator^=(MeshState m) { return value ^= m; }

	std::string toString()
	{
		std::ostringstream ostr;
		ostr << "  MeshState::";

		if (value & Pickable)
			ostr << "Pickable:";

		if (value & Selected)
			ostr << "Selected:";

		if (value & MeshChanged)
			ostr << "MeshChanged:";

		if (value & Invalid)
			ostr << "Invalid:";

		return ostr.str();
	}
};

struct MeshBufferState
{
	bool hasMeshChanged() const { return (state & MeshState::MeshChanged) == MeshState::MeshChanged; }
	bool isSelected() const { return (state & MeshState::Selected) == MeshState::Selected; }
	bool isPickable() const { return (state & MeshState::Pickable) == MeshState::Pickable; }
	
	std::string toString() { return state.toString(); }

	MeshState state = 0;

}; // end class PhysicsBodyState