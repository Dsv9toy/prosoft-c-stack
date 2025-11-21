#include "cstack.h"
#include <stdlib.h>  // Для malloc и free. Тк работаем с динамическим выделением памяти
#include <string.h>


#define MaxStacks 20  // Определяем максимальное кол-во стеков. Слышал что define не всегда верный выход, наверно можно по другому, но пока так.

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
    if (hstack < 0 || hstack >= MaxStacks){ 
          return 1; // Ошибка. Не валидный дескриптор
    }
 
    //Обращение
    return (stacks[hstack].isUsed == 1) ? 0 : 1 ;
}

unsigned int stack_size(const hstack_t hstack)
{
    const stack_manager_t* manager = get_stack_manager(hstack); // Явно укажем что изменению не подлежит(const)
    
    if (!manager) {
        return 0u;  // Возвращение числа 0 - то есть нет элементов в стеке. тип unsigned чтоб компилятор не ругался.
    }
    return manager->elemCount; // Если всё хорошо, то вернется кол-во элементов.
}


void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    stack_manager_t* manager = get_stack_manager(hstack);
    if(!manager){
        return; // если что-то не так - выходим
    }
    // Проверяем входные данные и если всё хорошо, выделяем место в памяти под них
    if(!data_in || size == 0 ) return;
    
    struct node* new_node = (struct node*)malloc(sizeof(struct node) + size); // Размер - это наш размер структуры и еще плюс размер данных от пользователя

    if(!new_node) return; // при ошибке выходим.
    
    new_node->prev = manager->stackTop; // связываем с предыдущ. вершиной
    new_node->size = size; // и сохраняем размер данных

    memcpy(new_node->data, data_in, size); // мем копирует побайтово.

    //Обновляем вершину после добавления, а также счётчик
    manager->stackTop = new_node; // теперь вершина new_node
    manager->elemCount++;
}


unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{   
//Это должно работать по принципу: определить стек, выбрать данные, отправить их в буфер для пользователя, удалить вершину, обновить значение новой вершины
    stack_manager_t* manager = get_stack_manager(hstack);
    if(!manager || !manager->stackTop) {
        return 0u;
    }
//проверка на отсутствующие данные. 

    if(!data_out) return 0u;

    struct node* top_node = manager->stackTop;  

    if (size != top_node->size) {
        return 0u;  
    }

//Копирование
    memcpy(data_out, top_node->data, size);

    manager->stackTop = top_node->prev;
    manager->elemCount--;

    free(top_node);
    
    return size;
}

