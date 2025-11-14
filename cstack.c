#include "cstack.h"
#include <stdlib.h>  // Для malloc и free. Тк работаем с динамическим выделением памяти
#include <string.h> // возмем для копирования данных типо memcpy и тд


#define MaxStacks 20  // Определяем максимальное кол-во стеков. Слышал что define не всегда верный выход, наверно можно по другому, но пока так.

// тут у нас макрос для подавления предупреждений о неиспользуемых переменных. Как уже говорили на лекциях, компилятор видит что данные вычислились, но результат
// не выводится. Он бы мог поругаться на то что рез-т не исп-ся, но не делает этого. 
#define UNUSED(VAR) (void)(VAR)

//Опишем наш стек, а точнее его внутреннюю структуру данных. Тут что храним, какого размера.
struct node
{
    struct node* prev; // Это указатель на предыдущий элемент
    unsigned int size; // это будет размер данных( в байтах )
    char data[0]; // Массив для данных.
}; 


//Данные определили, теперь разберемся как управлять стеком. пусть наш stack_manager_t будет отвечать за управление самим стеком.
typedef struct 
{
    struct node* stackTop; //  Это наш указатель на вершину
    unsigned int elemCount; // кол-во элементов в стеке
    int isUsed;                                                     // Для определения: 1 - стек используется. 0 - стек освобожден
} stack_manager_t;

//Массив с нашими стеками.
static stack_manager_t stacks[MaxStacks] = {0}; 



// Вспомогательная функция для управления стеком и проверки его состояния. Как будто бы админу звоним, говорим, проверь ка чё он хочет от нас))
static stack_manager_t* get_stack_manager(const hstack_t hstack) {
    if (hstack < 0 || hstack >= MaxStacks) {   // проверка на то что мы обращаемся вообще к существующему стеку? Может такого нет вообще
        return NULL;
    }
    if (stacks[hstack].isUsed == 0) {
        return NULL;                    // Если обратились к существующему стеку, то смотрим, занят ли он, туда можно свои байты класить или нет
    }
    return &stacks[hstack]; // ссылка на элемент стека. Если там что-то есть, результат будет показан.
}



hstack_t stack_new(void)
{
    for (int i = 0; i < MaxStacks; ++i) {
        if (stacks[i].isUsed == 0)          // Создаем условие. Необходимо найти свободный стек. То есть если стек == 0 т.е освобожден -> занимаем его
        {                                   
            stacks[i].stackTop = NULL;      
            stacks[i].elemCount = 0;
            stacks[i].isUsed = 1;           // Помечаем что теперь он используется.

            return i; // Возвращаем индекс стека как дискриптор
        }
    }
    return -1; // Если ,опять же, нет свободных стеков имеется ввиду
}


void stack_free(const hstack_t hstack) 
{
    stack_manager_t* manager = get_stack_manager(hstack) ; 
    if (!manager) return;

    // Далее освобождаем все узлы стека
    struct node* current = manager->stackTop;
        while(current != NULL) {
            struct node* prev = current->prev;
            free(current);
            current = prev;
        }   

    manager->stackTop = NULL;
    manager->elemCount = 0;
    manager->isUsed = 0;
}










int stack_valid_handler(const hstack_t hstack)
{
    UNUSED(hstack);
    return 1;
}


unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    return 0;
}


void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_in);
    UNUSED(size);
}


unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_out);
    UNUSED(size);
    return 0;
}

