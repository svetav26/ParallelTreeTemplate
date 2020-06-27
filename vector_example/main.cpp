/*
 * Дана задача:
 * Максимизировать f(x) = 5*x0 + 10*x1 + 4*x2 + 3*x3
 * С ограничением 4*x0 + 7*x1 + 2*x2 + 3*x3 <= 10
 * где x1,x2,x3,x4 двоичные (т.е. принадлежат {0,1})
 */

#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

#include <ParallelTree.hpp>

using namespace std;

// Функция, которую мы максимизируем
int f(const vector<int>& x)
{
    return 5*x[0] + 10*x[1] + 4*x[2] + 3*x[3];
}

// Функция, которая проверяет выполнение ограничения
bool constraint(const vector<int>& x)
{
    return 4*x[0] + 7*x[1] + 2*x[2] + 3*x[3] <= 10;
}

// Рекорд. Должен наследоваться от класса Record и реализовать методы
// betterThan и clone.
class ExampleRecord : public Record
{
public:
    ExampleRecord() :
        x(4, 0)
    {}
    
    // Вектор с решением
    vector<int> x;
    
    /*
     * Должна возвращать true, если данный рекорд лучше (меньше в задачах
     * минимизации и больше в задачах максимизации), чем other
     */
    bool betterThan(const Record& other) const override
    {
        const ExampleRecord& otherCast = static_cast<const ExampleRecord&>(other);
        // Поскольку у нас задача максимизации, то используем оператор "больше".
        return f(x) > f(otherCast.x);
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
    ExampleNode() :
        x(4, 0),
        lastX(-1)
    {}
    
    // Вектор с 4 переменными x.
    vector<int> x;
    // Какой x мы меняли последним
    int lastX;
    
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
        
        // Потомки
        std::vector< std::unique_ptr<Node> > childNodes;
        // Если lastX == 3, то мы дошли до листа дерева и потомков у текущего
        // узла нет.
        if(lastX == 3)
        {
            // Если текущее решение лучше рекорда, то меняем рекорд
            if(f(x) > f(recordCast.x))
                recordCast.x = x;
            // Потомков нет. childNodes пуст.
            return childNodes;
        }
        else
        {
            lastX += 1;
            // Рассматриваем 2 случая: x[lastX] = 0 и x[lastX] = 1
            x[lastX] = 0;
            // Если ограничение не выполняется, то отсекаем ветвь.
            if(constraint(x))
                childNodes.emplace_back(new ExampleNode(*this));
            
            x[lastX] = 1;
            // Если ограничение не выполняется, то отсекаем ветвь.
            if(constraint(x))
                childNodes.emplace_back(new ExampleNode(*this));
            
            return childNodes;
        }
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
        return f(x) > f(otherCast.x);
    }
};


int main()
{
    // Полагаем в начале все x равными 0 и начальный рекорд равным 0.
    ExampleRecord initialRecord;
    // Корень дерева вариантов.
    unique_ptr<ExampleNode> root = make_unique<ExampleNode>();
    auto startTime = chrono::high_resolution_clock::now();
    // Параллельно находим решение
    unique_ptr<Record> bestSolution = parallelTree(move(root), initialRecord, 16);
    const ExampleRecord* bestSolutionCast = reinterpret_cast<const ExampleRecord*>(bestSolution.get());
    auto finishTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(finishTime - startTime);
    
    cout << "x0 = " << bestSolutionCast->x[0] << ",  "
         << "x1 = " << bestSolutionCast->x[1] << ",  "
         << "x2 = " << bestSolutionCast->x[2] << ",  "
         << "x2 = " << bestSolutionCast->x[3] << ",  "
         << endl;

    cout << "finished in " << duration.count() << " microseconds" << endl;
    
    return 0;
}
