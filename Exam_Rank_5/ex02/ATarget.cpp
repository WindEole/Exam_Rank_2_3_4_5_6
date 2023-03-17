#include "ATarget.hpp"

ATarget::ATarget()
{

}

ATarget::ATarget(ATarget const &other)
{
	*this = other;
}

ATarget::ATarget(std::string const &type): _type(type)
{

}

ATarget &ATarget::operator=(ATarget const &other)
{
	_type = other._type;
	return (*this);
}

ATarget::~ATarget()
{

}

std::string const &	ATarget::getType() const
{
	return (_type);
}

void				ATarget::getHitBySpell(ASpell const &spell) const
{
	std::cout << this->_type << " has been " << spell.getEffects() << "!\n";
}