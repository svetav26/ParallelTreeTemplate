#ifndef PARALLELTREE_HPP
#define PARALLELTREE_HPP

#include <algorithm>
#include <list>
#include <memory>
#include <vector>
#include <omp.h>

class Node {};

class Result
{
public:
    /**
     * @brief Должен возвращать true, если данный рекорд лучше (меньше в задачах
     *        минимизации и больше в задачах максимизации), чем @p other
     */
    virtual bool betterThan(const Result& other) = 0;
    
    /**
     * @brief Должен возвращать копию данного объекта.
     */
    virtual std::unique_ptr<Result> clone() = 0;
    
    virtual Result& operator= (const Result& other) = 0;
};

class Params {};

/**
 * @brief Обработка дерева и получение результата.
 * @param processNode Функция, которая принимает текущий узел, ссылку на
 *        результат, и возвращает вектор потомков этого узла (или пустой
 *        вектор, если потомков нет). Эта функция должна сама обеспечивать
 *        потокобезопасность изменения результата.
 * @param node Корень дерева.
 * @param result Результат. Потокобезопасность изменения результата должна
 *        обеспечить сама функция processNode
 * @param params Дополнительные параметры древовидного алгоритма. Если ему не
 *        нужны дополнительные параметры, то можно указать nullptr.
 * @param higherPriority Функция, которая сравнивает два задания, и возвращает
 *        true, если приоритет левого задания больше, чем правого. Если
 *        higherPriority != nullptr, то задания с большим приоритетом будут
 *        обрабатываться раньше. Если же higherPriority == nullptr, то задания
 *        будут обрабатываться примерно в том порядке, в котором их возвращает
 *        @p processNode.
 */
void parallelTree(
    std::vector<std::unique_ptr<Node>> (*processNode)(std::unique_ptr<Node>,
                                                      Result&, const Params*),
    std::unique_ptr<Node> node, Result& result, const Params* params = nullptr,
    bool (*higherPriority)(const std::unique_ptr<Node>& n1,
                           const std::unique_ptr<Node>& n2,
                           const Params* params) = nullptr);

#endif // PARALLELTREE_HPP
