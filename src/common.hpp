// standard C++ headers
#include <string>
#include <string_view>
#include <utility>

namespace common
{

static constexpr inline auto const spaces = "\t\n\v\f\r ";

constexpr inline void trim_begin(std::string_view& view) noexcept
{
	auto pos = view.find_first_not_of(spaces);
	auto count = std::min(pos, view.size());
	view.remove_prefix(count);
}

constexpr inline void trim_end(std::string_view& view) noexcept
{
	auto back = view.find_last_not_of(spaces);
	auto count = std::min(view.size() - back - 1, view.size());
	view.remove_suffix(count);
}

constexpr inline void trim(std::string_view& view) noexcept
{
	trim_begin(view);
	trim_end(view);
}

template<char Delim = ' '>
constexpr std::pair<std::string_view, std::string_view>
divide(std::string_view view) noexcept
{
	auto delim_pos = view.find(Delim);
	std::string_view first = view.substr(0, delim_pos);
	view.remove_prefix(std::min(first.size() + 1, view.size()));
	trim(first);
	trim(view);
	return {first, view};
}

inline void increase(std::string& numeric_string)
{
	auto const end = numeric_string.rend();
	for (auto begin = numeric_string.rbegin(); begin != end; ++begin) {
		if (*begin < '9') {
			++*begin;
			return;
		}
		*begin = '0';
	}
	numeric_string.insert(0, 1, '1');
}

} // namespace common
