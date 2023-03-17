#pragma once

#include <string>
#include <iostream>
#include <map>
#include "ASpell.hpp"
#include "SpellBook.hpp"

class Warlock
{
	public:
		Warlock(std::string const &name, std::string const &title);
		~Warlock();

		std::string const	getName() const;
		std::string const	getTitle() const;

		void				setTitle(std::string const &title);
		void				introduce() const;
		void				learnSpell(ASpell *spell);
		void				forgetSpell(std::string spell_name);
		void				launchSpell(std::string spell_name, ATarget const &target);

	private:
		std::string		_name;
		std::string		_title;
		SpellBook		_spellbook;

		Warlock();
		Warlock(Warlock const &other);
		Warlock &operator=(Warlock const &other);
};