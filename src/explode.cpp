#include "common.hpp"
// standard C++ headers
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
// system headers
#include <getopt.h>

class Range
{
	std::string m_prefix, m_postfix;

	static std::pair<std::string_view, std::string_view>
	range(std::string_view view) noexcept
	{
		auto [first, second] = common::divide(view);
		bool only_digits =
		    std::all_of(first.cbegin(), first.cend(), ::isdigit)
		    && std::all_of(second.cbegin(), second.cend(), ::isdigit);
		if (second.empty()) {
			second = first;
		}

		if (only_digits
		    && (first.size() < second.size()
		        || (first.size() == second.size() && first <= second))) {
			return {first, second};
		}
		return {};
	}

public:
	Range(std::string prefix, std::string postfix)
	    : m_prefix(std::move(prefix)), m_postfix(std::move(postfix))
	{
	}

	void print(std::ostream& out, std::istream& input_file)
	{
		std::string line;
		std::string start;
		std::string_view end;
		while (std::getline(input_file, line)) {
			std::tie(start, end) = range(line);
			if (start.empty()) {
				continue;
			}
			for (; start != end; common::increase(start)) {
				out << m_prefix << start << m_postfix << '\n';
			}
			out << m_prefix << start << m_postfix << '\n';
		}
	}
};

int main(int argc, char** argv)
{
	std::ios::sync_with_stdio(false);
	std::string prefix;
	std::string postfix;
	for (int opt{}; (opt = ::getopt(argc, argv, "a:b:")) != -1;) {
		switch (opt) {
		case 'a': // after
			postfix = optarg;
			break;
		case 'b': // before
			prefix = optarg;
			break;
		}
	}

	Range range(prefix, postfix);

	if (optind == argc) {
		range.print(std::cout, std::cin);
		return EXIT_SUCCESS;
	}

	while (optind < argc) {
		std::ifstream file(argv[optind++]);
		range.print(std::cout, file);
	}

	return EXIT_SUCCESS;
}
