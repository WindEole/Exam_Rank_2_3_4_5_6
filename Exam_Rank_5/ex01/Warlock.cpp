#include "Warlock.hpp"

Warlock::Warlock(std::string const &name, std::string const &title): _name(name), _title(title)
{
	std::cout << this->_name << ": This looks like another boring day.\n";
}

Warlock::~Warlock()
{
	std::cout << this->_name << ": My job here is done!\n";
}

std::string const	Warlock::getName() const
{
	return (_name);
}

std::string const	Warlock::getTitle() const
{
	return (_title);
}

void				Warlock::setTitle(std::string const &title)
{
	_title = title;
}

void				Warlock::introduce() const
{
	std::cout << this->_name << ": I am " << this->getName() << ", " << this->getTitle() << "!\n";
}

void				Warlock::learnSpell(ASpell *spell)
{
	if (spell)
		_spells.insert(std::pair<std::string, ASpell *>(spell->getName(), spell->clone()));
}

void				Warlock::forgetSpell(std::string spell_name)
{
	std::map<std::string, ASpell *>::iterator it = _spells.find(spell_name);
	if (it != _spells.end())
		delete it->second;
	_spells.erase(spell_name);
}

void				Warlock::launchSpell(std::string spell_name, ATarget const &target)
{
	ASpell	*spell = _spells[spell_name];
	if (spell)
		spell->launch(target);
}
