/* В данном примере дерево вариантов имеет следующий вид:
 *          15
 *         /  \
 *        /    \
 *      14      14
 *     /  \    /  \
 *   13   13  13  13
 *   / \  / \ / \ / \
 *  ..................
 *  / \  / \ ..... / \
 *  1 1  1 1       1 1
 * Т.е. дерево двоичное, каждый узел содержит целое число, в корне - 15, а в
 * потомках содержится число на 1 меньше, чем в родителе. У узлов, содержащих
 * число 1, потомков нет.
 * 
 * Необходимо найти минимальное число в дереве (ясно, что оно будет равно 1).
 */

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <chrono>

#include <ParallelTree.hpp>

using namespace std;

// Рекорд. Должен наследоваться от класса Record и реализовать методы
// betterThan и clone.
class ExampleRecord : public Record
{
public:
    ExampleRecord() :
        x(std::numeric_limits<int>::max())
    {}
    
    // Минимальное найденное значение узла
    int x;
    
    /*
     * Должна возвращать true, если данный рекорд лучше (меньше в задачах
     * минимизации и больше в задачах максимизации), чем other
     */
    bool betterThan(const Record& other) const override
    {
        const ExampleRecord& otherCast = static_cast<const ExampleRecord&>(other);
        // Поскольку у нас задача максимизации, то используем оператор "больше".
        return x < otherCast.x;
    }
    
    // Должен возвращать копию данного рекорда.
    virtual std::unique_ptr<Record> clone() const override
    {
        // Здесь просто используем конструктор копий
        return std::make_unique<ExampleRecord>(*this);
    }
};

// Узел дерева вариантов. Должен наследоваться от класса Node и реализовать
// методы process и hasHigherPriority.
class ExampleNode : public Node
{
public:

    ExampleNode(int value) :
        x(value)
    {}
    
    // Значение в данном узле дерева.
    int x;
    
    /*
     * Функция, которая обрабатывает текущий узел и возвращает вектор
     * потомков этого узла (или пустой вектор, если потомков нет).
     * 
     * Она не должна менять глобальных переменных, т.к. она будет исполняться
     * в нескольких потоках. Рекорд менять можно (при этом синхронизация не
     * требуется).
     */
    virtual std::vector< std::unique_ptr<Node> > process(Record& record) override
    {
        ExampleRecord& recordCast = static_cast<ExampleRecord&>(record);
        
        if (x < recordCast.x) {
            recordCast.x = x;
        }

        // Потомки
        std::vector< std::unique_ptr<Node> > childNodes;

        if (x > 0) {
            childNodes.emplace_back(new ExampleNode(x - 1));    
            childNodes.emplace_back(new ExampleNode(x - 1));    
        }
        return childNodes;
    }
    
    /*
     * Возвращает true, если приоритет данного задания больше, чем other.
     * Задания с большим приоритетом будут обрабатываться раньше.
     */
    virtual bool hasHigherPriority(const Node& other) const
    {
        const ExampleNode& otherCast = static_cast<const ExampleNode&>(other);
        // Если у данного узда значение f больше, то считаем что у него больше
        // приоритет.
        return x < otherCast.x;
    }
};


int main()
{
    // Полагаем в начале все x равными 0 и начальный рекорд равным 0.
    ExampleRecord initialRecord;
    // Корень дерева вариантов.
    unique_ptr<ExampleNode> root = make_unique<ExampleNode>(16);
    auto startTime = chrono::high_resolution_clock::now();
    // Параллельно находим решение
    unique_ptr<Record> bestSolution = parallelTree(move(root), initialRecord, 16);
    const ExampleRecord* bestSolutionCast = reinterpret_cast<const ExampleRecord*>(bestSolution.get());
    auto finishTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(finishTime - startTime);
    
    cout << "x = " << bestSolutionCast->x << endl;
    cout << "finished in " << duration.count() << " microseconds" << endl;
    
    return 0;
}
