#include "SpellBook.hpp"

SpellBook::SpellBook() {}
SpellBook::~SpellBook() {}

void	SpellBook::learnSpell(ASpell *spell)
{
	if (spell)
		_book.insert(std::pair<std::string, ASpell *>(spell->getName(), spell->clone()));
}

void	SpellBook::forgetSpell(std::string const &spell_name)
{
	std::map<std::string, ASpell *>::iterator it = _book.find(spell_name);
	if (it != _book.end())
		delete it->second;
	_book.erase(spell_name);
}

ASpell*	SpellBook::createSpell(std::string const &spell_name)
{
	std::map<std::string, ASpell *>::iterator it = _book.find(spell_name);
	if (it != _book.end())
		return _book[spell_name];
	return NULL;
}
