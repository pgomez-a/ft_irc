#ifndef WORD_COMPOSITION_HPP
# define WORD_COMPOSITION_HPP
# include <string>
# include <list>
//Word validty

bool	valid_shortname(std::string s);
bool	valid_nickname(std::string s);
bool	valid_user(std::string s);
bool	valid_hostaddr(std::string s);
bool	valid_host(std::string s);
bool	valid_servername(std::string n);
bool	valid_user_origin(std::string n);
bool	valid_rest(std::string n);
bool	valid_parameter(std::string p);

//Word Composition

typedef	bool (*char_filter)(char);

bool	letter(char c);
bool	digit(char c);
bool	hyphen(char c);
bool	special(char c);
bool	user(char c);
bool	host(char c);
bool	parameter(char c);
bool	rest(char c);


bool	c(char c, char d);
bool	is_in_set(char c, std::string set);
bool	is_in_set(char c, const char *set, size_t len);

bool	check_str(std::string s, char_filter f);
bool	check_str(std::string s, std::list<char_filter> l);

#endif