#ifndef TREEPARAMS_HPP
#define TREEPARAMS_HPP

#include <ParallelTree.hpp>

/* Если алгоритму нужны какие-либо константные параметры, то их нужно хранить
 * здесь. В данном примере они не нужны. Этот класс приведен просто для
 * демонстрации.
 */
class TreeParams : public Params
{
public:
    const int param1 = 10;
    const double param2 = 2.5;
};

#endif // TREEPARAMS_HPP
