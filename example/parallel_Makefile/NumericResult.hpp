#ifndef NUMERICRESULT_HPP
#define NUMERICRESULT_HPP

#include <memory>
#include <ParallelTree.hpp>

// Класс, который содержит рекорд.
class NumericResult : public Result
{
public:
    NumericResult(int value) :
        value(value) {}
        
    /* Эти три метода нужны для внутреннего использования в библиотеке.
     * Пользоваться ими самому не обязательно.
     */
    
    /* Возвращает true, если данный рекорд лучше other. Поскольку данная задача
     * - задача минимизации, то здесь стоит оператор <. В задачах максимизации
     * нужно поставить >.
     */
    bool betterThan(const Result& other) override
    {
        const NumericResult o = (NumericResult&) other;
        return value < o.value;
    }
    
    // Создает копию данного рекорда.
    std::unique_ptr<Result> clone() override
    {
        return std::unique_ptr<Result>(new NumericResult(*this));
    }
    
    // Присваивает данному рекорду значение other.
    Result& operator= (const Result& other) override
    {
        const NumericResult o = (NumericResult&) other;
        value = o.value;
        return *this;
    }
    
    int value;
};

#endif // NUMERICRESULT_HPP
