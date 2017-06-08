#pragma once

#include <string>

namespace tree {
	class TreeFactory {
	private:
		void readFile(std::string);
	public:
		TreeFactory(std::string);
	};
}