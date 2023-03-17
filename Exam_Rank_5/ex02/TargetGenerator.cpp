#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator() {}
TargetGenerator::~TargetGenerator() {}

void	TargetGenerator::learnTargetType(ATarget *target)
{
	if (target)
		_targets.insert(std::pair<std::string, ATarget *>(target->getType(), target->clone()));
}

void	TargetGenerator::forgetTargetType(std::string const &target_name)
{
	std::map<std::string, ATarget *>::iterator it = _targets.find(target_name);
	if (it != _targets.end())
		delete it->second;
	_targets.erase(target_name);
}

ATarget*	TargetGenerator::createTarget(std::string const &target_name)
{
	std::map<std::string, ATarget *>::iterator it = _targets.find(target_name);
	if (it != _targets.end())
		return _targets[target_name];
	return NULL;
}