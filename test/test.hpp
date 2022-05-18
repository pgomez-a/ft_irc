#ifndef TEST_HPP
# define TEST_HPP

# include <string>
# include <iostream>
# include <sstream>

void    dummy(void);

template <typename T>
T   dummy(T x)
{
    std::cout << x << std::endl;

    return (x);
}

#endif