#include "common.hpp"
// system headers
#ifdef WIN32
#include <winsock2.h>
#define STDIN_FILENO (_fileno(stdin))
#else
#include <unistd.h>
#endif
// standard C++ headers
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

static constexpr char const* const syntax =
    R"(implode - print numeric ranges contained in input
SYNTAX
	implode [OPTION] [filename...]

Options:
	filename - optional path to a file
		Everything from first-non digit on each line will be discarded

	-s  sort all input data

DESCRIPTION
	Clump together ranges of files and print start of range, end of range and size of range.

EXAMPLE
	implode <(seq 120 199) <(seq 100 109) -s
100 109 10
120 199 80
)";

static bool filter_number(std::string& number) noexcept
{
	if (auto pos = number.find_first_not_of("0123456789");
	    pos != std::string::npos) {
		number.erase(pos);
	}
	return !number.empty();
}

static auto numbers_from_file(std::vector<std::string>& numbers,
                              std::string const& filename)
{
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line)) {
		if (filter_number(line)) {
			numbers.push_back(line);
		}
	}
}

static bool fd_has_data(int fd) noexcept
{
	fd_set fds{};
	FD_SET(fd, &fds);
	timeval timeout{};
	return select(fd + 1, &fds, nullptr, nullptr, &timeout) > 0;
}

static void numbers_from_stdin(std::vector<std::string>& numbers)
{
	if (!fd_has_data(STDIN_FILENO)) {
		return;
	}
	std::string line;
	while (std::getline(std::cin, line)) {
		if (filter_number(line)) {
			numbers.push_back(line);
		}
	}
}

static void print_ranges(std::ostream& out,
                         std::vector<std::string> const& numbers)
{
	if (numbers.empty()) {
		return;
	}
	std::string expected = numbers.front();
	std::string first = numbers.front();
	std::string previous = numbers.front();
	int count = 0;
	for (auto const& number : numbers) {
		if (number == expected) {
			++count;
			previous = expected;
			common::increase(expected);
			continue;
		}
		out << first << ' ' << previous << ' ' << count << '\n';
		previous = first = expected = number;
		common::increase(expected);
		count = 1;
	}
	out << first << ' ' << previous << ' ' << count << '\n';
}

int main(int argc, char const* const* argv)
{
	std::ios::sync_with_stdio(false);
	std::vector<std::string> numbers;
	bool sort = false;
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] != '-') {
			numbers_from_file(numbers, argv[i]);
			continue;
		}
		switch (argv[i][1]) {
		case 's':
			sort = true;
			break;
		case 'h':
		default:
			std::cout << syntax << '\n';
			return EXIT_FAILURE;
		}
	}

	numbers_from_stdin(numbers);
	if (sort) {
		std::sort(numbers.begin(), numbers.end(),
		          [](std::string const& lhs, std::string const& rhs) -> bool {
			          return std::forward_as_tuple(lhs.size(), lhs)
			                 < std::forward_as_tuple(rhs.size(), rhs);
		          });
	}
	numbers.erase(std::unique(numbers.begin(), numbers.end()), numbers.end());

	print_ranges(std::cout, numbers);
	return EXIT_SUCCESS;
}
