#pragma once

#include <map>
#include "ATarget.hpp"

class TargetGenerator
{
	public:
		TargetGenerator();
		~TargetGenerator();

		void				learnTargetType(ATarget *target);
		void				forgetTargetType(std::string const &target_name);
		ATarget*			createTarget(std::string const &target_name);

	private:
		std::map<std::string, ATarget *> 	_targets;

		TargetGenerator(TargetGenerator const &other);
		TargetGenerator &operator=(TargetGenerator const &other);
};