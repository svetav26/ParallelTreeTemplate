/* ОЧЕНЬ ВАЖНО компилировать все проекты с использованием данной библиотки
 * с подключенным OpenMP (флаг -fopenmp для gcc. Для остальных компиляторов
 * см. документацию к ним).
 * 
 * В данном примере дерево вариантов имеет следующий вид:
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
#include <limits>
#include <memory>
#include <vector>
#include "TreeNode.hpp"
#include "TreeParams.hpp"
#include "NumericResult.hpp"

using namespace std;

/* Функция обработки узлов. Принимает текущий узел (node), ссылку на рекорд
 * (result) и указатель на дополнительные параметры (params) и возвращает
 * потомков данного узла (или пустой вектор, если потомков нет). В данном случае
 * params не используется (класс TreeParams приведен просто для примера).
 * 
 * В данную функцию всегда передается копия result (создается с помощью метода
 * clone), причем создается по копии для каждого потока, поэтому беспокоиться
 * о многопоточном доступе к нему не нужно. По завершении работы функции
 * processNode измененная копия result сравнивается с самим result (с помощью
 * метода betterThan) и result заменяется на копию, если копия лучше.
 */
vector<unique_ptr<Node>> processNode(std::unique_ptr<Node> node,
                                     Result& result, const Params* params)
{
    TreeNode* treeNode = (TreeNode*) node.get();
    NumericResult& numericResult = (NumericResult&) result;
    
    if(treeNode->number < numericResult.value)
        numericResult.value = treeNode->number;
    
    if(treeNode->number > 1)
    {
        vector<unique_ptr<Node>> children;
        children.emplace_back(new TreeNode(treeNode->number - 1));
        children.emplace_back(new TreeNode(treeNode->number - 1));
        
        return children;
    }
    else
        return vector<unique_ptr<Node>>();
}

/* Функция приоритетов. Возвращает true, если левый узел нужно обрабатывать до
 * правого и false, если правый нужно обрабатывать раньше, чем левый. В 
 * некоторых ситуациях порядок их обработки может быть нарушен (хотя большинство
 * узлов будут обработаны в правильном порядке), поэтому писать программу с
 * расчетом на определенный порядок обработки узлов нельзя.
 * Вместо higherPriority в функцию parallelTree можно передать nullptr. Тогда
 * узлы будут обрабатываться в том порядке, в котором получится.
 */
bool higherPriority(const std::unique_ptr<Node>& n1,
                    const std::unique_ptr<Node>& n2,
                    const Params* params)
{
    // Узлы с меньшим числом в них обрабатываем раньше
    TreeNode* treeNode1 = (TreeNode*) n1.get();
    TreeNode* treeNode2 = (TreeNode*) n2.get();
    return treeNode1->number < treeNode2->number;
}

int main()
{
    // Корень дерева
    unique_ptr<Node> root(new TreeNode(15));
    
    /* Если алгоритму нужны какие-либо константные параметры, то их нужно
     * хранить здесь. В данном примере они не нужны. Этот класс приведен просто
     * для демонстрации.
     */
    TreeParams treeParams;
    
    /* Рекорд.
     * Мы не можем присвоить int бесконечность, поэтому присваиваем максимальное
     * возможное значение.
     */
    NumericResult result(numeric_limits<int>::max());
    
    /* Функция parallelTree занимается параллельной обработкой узлов дерева
     * вариантов с помощью функции processNode. Для начала ей нужен только
     * корень дерева (root). После обработки корня она занимается обработкой
     * потомоков, которых вернет функция processNode (потомки также передаются
     * в функцию processNode). Алгоритм считается завершенным, если все узлы
     * были обработаны.
     *
     * Вместо treeParams и higherPriority можно указать nullptr, если они не
     * нужны.
     */
    parallelTree(processNode, move(root), result, &treeParams, higherPriority);
    
    /* По окончании работы функции parallelTree результат будет храниться в
     * result.
     */
    cout << "Result: " << result.value << "\n";
    
    return 0;
}
