/*
 * Date: 25/4/2016
 * Writter: ESS
 * Description: This file contains macros and function declarations for queue data structure
 */

// Include header files
#include <stdint.h>

// Macros declarations
#define QUEUE_MAX_SIZE 256

//struct declarations
typedef struct
{
    int data[QUEUE_MAX_SIZE];
    uint16_t capacity;
} iqueue_t;

// Function prototypes

/* Name: queue_init
 * Module: queue
 * Parameters:
 *   - queue - queue_t*
 * Return: void
 * Description: Initialize queue
 */
void iqueue_init(iqueue_t* queue);

/* Name: queue_push
 * Module: queue
 * Parameters:
 *   - queue - queue_t*
 *   - data - const char
 * Return: void
 * Description: push data to the bottom of queue
 */
void iqueue_push(iqueue_t* queue, const int data);

/* Name: queue_pop
 * Module: queue
 * Parameters:
 *   - queue - queue_t*
 * Return: data element 
 * Description: pop data element at the begining of queue. Then, remove it from queue
 */
int iqueue_pop(iqueue_t* queue);

/* Name: queue_peek
 * Module: queue
 * Parameters:
 *   - queue - const queue_t*
 * Return: data element at the begining of queue
 * Description: push data to queue (Not removing it from queue)
 */
int iqueue_peek(const iqueue_t* queue);

/* Name: queue_is_empty
 * Module: queue
 * Parameters:
 *   - queue - const queue_t*
 * Return: 1 means TRUE or 0 means FALSE
 * Description: check if queue is empty or not
 */
int iqueue_is_empty(const iqueue_t* queue);

/* Name: queue_is_empty
 * Module: queue
 * Parameters:
 *   - queue - const queue_t*
 * Return: 1 means TRUE or 0 means FALSE
 * Description: check if queue is full or not
 */
int iqueue_is_full(const iqueue_t* queue);

/* Name: queue_push_string
 * Module: queue
 * Parameters:
 *   - queue - const queue_t*
 *   - str - const char*
 *   - len - uint16_t
 * Return: void
 * Description: check if queue is full or not
 */
void iqueue_push_string(iqueue_t* queue, const int* str, uint16_t len);

/* End of file */
