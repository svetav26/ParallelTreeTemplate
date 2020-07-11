#ifndef PARALLELTREE_HPP
#define PARALLELTREE_HPP

#include <memory>
#include <vector>
#include <cstdint>

/// Рекорд
class Record
{
public:
    virtual ~Record();
    /**
     * @brief Должен возвращать true, если данный рекорд лучше (меньше в задачах
     *        минимизации и больше в задачах максимизации), чем @p other
     */
    virtual bool betterThan(const Record& other) const = 0;
    
    /**
     * @brief Должен возвращать копию данного рекорда.
     */
    virtual std::unique_ptr<Record> clone() const = 0;
};

/// Узел дерева вариантов
class Node
{
public:
    virtual ~Node();
    /**
     * @brief Функция, которая обрабатывает текущий узел и возвращает вектор
     *        потомков этого узла (или пустой вектор, если потомков нет).
     * 
     * Она не должна менять глобальных переменных, т.к. она будет исполняться
     * в нескольких потоках. Рекорд менять можно (при этом синхронизация не
     * требуется).
     */
    virtual std::vector< std::unique_ptr<Node> > process(Record& record) = 0;
    
    /**
     * @brief Возвращает true, если приоритет данного задания больше, чем other.
     *        Задания с большим приоритетом будут обрабатываться раньше.
     */
    virtual bool hasHigherPriority(const Node& other) const = 0;
};

/**
 * @brief Обработка дерева и получение результата.
 * @param root Корень дерева вариантов.
 * @param initialRecord Начальное значение рекорда.
 * @param threads Количество потоков. Если threads=0, то используется число
 *                потоков, равное количеству ядер.
 * @return Лучший найденный рекорд.
 */
std::unique_ptr<Record> parallelTree(
    std::unique_ptr<Node> root,
    const Record& initialRecord,
    uint32_t threads=0
);

#endif // PARALLELTREE_HPP
