#pragma once

#include <string>
#include <iostream>
#include "ATarget.hpp"

class ATarget;

class ASpell
{
	public:
		ASpell();
		ASpell(ASpell const &other);
		ASpell(std::string const &name, std::string const &effects);
		ASpell &operator=(ASpell const &other);
		virtual ~ASpell();

		std::string const	&getName() const;
		std::string const	&getEffects() const;

		virtual ASpell*		clone() const = 0;
		void				launch(ATarget const &target) const;

	protected:
		std::string	_name;
		std::string	_effects;
};