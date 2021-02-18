#include "queueInt.h"
#include <string.h>

void iqueue_init(iqueue_t* queue)
{
    queue->capacity = 0;
    memset(queue->data, 0, sizeof(queue->data) / sizeof(int));
}

void iqueue_push(iqueue_t* queue, const int data)
{
    if (!iqueue_is_full(queue))
    {
        queue->data[queue->capacity] = data;
        queue->capacity = queue->capacity + 1;
    }
}

int iqueue_pop(iqueue_t* queue)
{
    // Preparations
    int result = 0;
    uint16_t index_data = 0;
    
    if (!iqueue_is_empty(queue))
    {
        result = queue->data[0];
        queue->capacity = queue->capacity - 1;
        
        for (index_data = 0; index_data < queue->capacity; index_data++)
        {
            queue->data[index_data] = queue->data[index_data + 1];
        }
        queue->data[queue->capacity] = 0;
    }
    

    return result;
}

int iqueue_peek(const iqueue_t* queue)
{
    // Preparations
    int result = 0;
    
    if (!iqueue_is_empty(queue))
    {
        result = queue->data[0];
    }
   
   return result;
}
int iqueue_is_empty(const iqueue_t* queue)
{
    // Preparations
    int result = 0;
    
    if (queue->capacity == 0)
    {
        result = 1;
    }
    
    return result;
}

int iqueue_is_full(const iqueue_t* queue)
{
    // Preparations
    int result = 0;
    
    if (queue->capacity == (sizeof(queue->data) / sizeof(int)))
    {
        result = 1;
    }
    
    return result;
}

void iqueue_push_string(iqueue_t* queue, const int* str, uint16_t len)
{
    // Preparations
    uint16_t index = 0;
    
    while ((index < len) && !iqueue_is_full(queue))
    {
        iqueue_push(queue, str[index]);
        index = index + 1;
    }
}

/* End of file */
