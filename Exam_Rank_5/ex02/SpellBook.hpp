#pragma once

#include <map>
#include "ASpell.hpp"

class SpellBook
{
	public:
		SpellBook();
		~SpellBook();

		void				learnSpell(ASpell *spell);
		void				forgetSpell(std::string const &spell_name);
		ASpell*				createSpell(std::string const &spell_name);

	private:
		std::map<std::string, ASpell *> 	_book;

		SpellBook(SpellBook const &other);
		SpellBook &operator=(SpellBook const &other);
};