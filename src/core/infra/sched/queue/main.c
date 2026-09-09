//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/queue/main.h"
#include "infra/sched/tasks/stack/main.h"
#include "main.h"
#include "src/core/shared/utils/random/main.h"

#include <stdint.h>
#include <stdlib.h>

struct SchedTask_t *_findTask(struct SchedQueue_t *queue, uint64_t id);

/**
 * @brief Remove a node from the DLL.
 *
 * Remove a node by updating next and
 * previous node in the struct so they
 * don't "see" the removed node.
 *
 * @param node Pointer to the node to be removed.
 *
 * @return Pointer to the removed node.
 * @retval NULL Parameter nullity fallback.
 *
 * @note The node aren't deleted, just aren't visible in the DLL.
 *
 * @pre @p node being a valid pointer to a SchedQueue_Node_t instance.
 * @post @p node not visible by other nodes in the DLL.
 */
void *_removeNode(struct SchedQueue_Node_t *node) {
    if (node == NULL)
        return NULL;

    // @p node isn't head
    if (node->prev != NULL) {
        // @p node isn't tail
        if (node->next != NULL)
            node->prev->next = node->next;
        else
            node->prev->next = NULL;
    }

    if (node->next != NULL) {
        if (node->prev != NULL)
            node->next->prev = node->prev;
        else
            node->next->prev = NULL;
    }

    return node;
}

/**
 * @brief Delete a node from the DLL.
 *
 * @param node Pointer to the node to be deleted.
 *
 * @note The node is permanentely deleted and it's memory released.
 *
 * @pre @p node being a valid pointer to a SchedQueue_Node_t instance.
 * @post @p node deleted.
 */
void _deleteNode(struct SchedQueue_Node_t *node) {
    if (node == NULL)
        return;

    if (_removeNode(node) == NULL)
        return;

    if (_cleanStack(&node->task->stack) == FALSE)
        return;

    free(node->task);
    free(node);
}

/**
 * @brief Delete a node from the DLL.
 *
 * @param node Pointer to the node to be deleted.
 *
 * @note The node is permanentely deleted and it's memory released.
 *
 * @pre @p node being a valid pointer to a SchedQueue_Node_t instance.
 * @post @p node deleted.
 */
void _deleteNode(struct SchedQueue_Node_t *node);

/**
 * @brief Get a new non-colided ID for a queue.
 *
 * @param queue Pointer to the SchedQueue_t instance that will be used to check for colisions.
 * @return New ID.
 *
 * @note If @p queue is NULL, return a random ID without colision checking.
 *
 * @pre @p queue being a valid pointer to a valid SchedTask_t instance.
 * @post @p queue unchanged.
 */
uint64_t _getNewID(struct SchedQueue_t *queue) {
    if (queue == NULL) {
        return Random.rand();
    }

    uint64_t newID = Random.rand();
    while (_findTask(queue, newID) != NULL)
        newID = Random.rand();

    return newID;
}

/**
 * @brief Initializate a new queue.
 *
 * @param task Pointer to the SchedTask_t instance that will be made head and tail.
 *
 * @return Pointer to the new queue.
 * @retval NULL Memory error or task nullity fallback.
 *
 * @note Caller owns the returned queue and must release it's memory.
 * @note Caller owns the SchedQueue_Node_t instance used as initial head and must release it's memory.
 *
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 * @post @p task unchanged.
 */
struct SchedQueue_t *_initQueue(struct SchedTask_t *task) {
    struct SchedQueue_t *out = malloc(sizeof(struct SchedQueue_t));
    if (out == NULL || task == NULL)
        return NULL;

    struct SchedQueue_Node_t *node = malloc(sizeof(struct SchedQueue_Node_t));
    if (node == NULL)
        return NULL;

    node->id = _getNewID(out);
    node->next = NULL;
    node->prev = NULL;
    node->task = task;

    out->head = node;
    out->tail = node;

    return out;
}

/**
 * @brief Append a task to the queue.
 *
 * New task will be the tail.
 *
 * @param queue A pointer to the SchedQueue_t instance to append.
 * @param task A pointer to the SchedTask_t instance that will be appended.
 *
 * @return Pointer to @p queue.
 * @retval NULL Memory error or param nullity fallback.
 *
 * @note Caller owns the created node, and must release it's memory.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 *
 * @post @p queue unchanged.
 * @post @p task unchanged.
 */
void *_appendQueue(struct SchedQueue_t *queue, struct SchedTask_t *task) {
    if (queue == NULL || task == NULL)
        return NULL;

    if (queue->tail == NULL)
        return NULL;

    struct SchedQueue_Node_t *node = malloc(sizeof(struct SchedQueue_Node_t));
    if (node == NULL)
        return NULL;

    node->id = _getNewID(queue);
    node->next = NULL;
    node->prev = queue->tail;
    node->task = task;

    // Don't check nullity because this can't be null (by default)
    // if just one node, this is head and tail
    // if null return (check above)
    queue->tail->next = node;
    queue->tail = node;

    return queue;
}

/**
 * @brief Prepend a task to the queue.
 *
 * New task will be the head.
 *
 * @param queue A pointer to the SchedQueue_t instance to append.
 * @param task A pointer to the SchedTask_t instance that will be appended.
 *
 * @return Pointer to @p queue.
 * @retval NULL Memory error or param nullity fallback.
 *
 * @note Caller owns the created node, and must release it's memory.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 *
 * @post @p queue unchanged.
 * @post @p task unchanged.
 */
void *_prependQueue(struct SchedQueue_t *queue, struct SchedTask_t *task) {
    if (queue == NULL || task == NULL)
        return NULL;

    if (queue->head == NULL)
        return NULL;

    struct SchedQueue_Node_t *node = malloc(sizeof(struct SchedQueue_Node_t));
    if (node == NULL)
        return NULL;

    node->id = _getNewID(queue);
    node->next = queue->head;
    node->prev = NULL;
    node->task = task;

    queue->head->prev = node;
    queue->head = node;

    return queue;
}

/**
 * @brief Find a specific task by ID.
 *
 * @param queue A postinter to the SchedQueue_t instance that will be preprended.
 * @param id The ID for being searched across the queue.
 *
 * @return Pointer to found task.
 * @retval NULL Memory error or param nullity fallback.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 *
 * @post @p queue unchanged.
 * @post @p task unchanged.
 */
struct SchedTask_t *_findTask(struct SchedQueue_t *queue, uint64_t id) {
    if (queue == NULL)
        return NULL;

    if (queue->head == NULL)
        return NULL;

    struct SchedQueue_Node_t *current = queue->head;
    while (current != NULL) {
        if (current->id == id)
            return current->task;

        current = current->next;
    }

    return NULL;
}

/**
 * @brief Delete the queue head.
 *
 * @param queue A pointer to the SchedQueue_t instance that will have it's head deleted.
 *
 * @return Pointer to @p queue.
 * @retval NULL Parameter nullity fallback.
 *
 * @note If @p queue->head->next is NULL, queue head will be NULL too.
 * @warning A empty @p queue will result in a NULL return.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @post @p queue unchanged.
 */
void *_deleteHead(struct SchedQueue_t *queue) {
    if (queue == NULL)
        return NULL;

    if (queue->head == NULL)
        return NULL;

    if (queue->head->next != NULL)
        queue->head = queue->head->next;
    else
        queue->head = NULL;

    return queue;
}

/**
 * @brief Delete the queue tail.
 *
 * @param queue A pointer to the SchedQueue_t instance that will have it's tail deleted.
 *
 * @return Pointer to @p queue.
 * @retval NULL Parameter nullity fallback.
 *
 * @note If @p queue->tail->prev is NULL, queue tail will be NULL too.
 * @warning A empty @p queue will result in a NULL return.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @post @p queue unchanged.
 */
void *_deleteTail(struct SchedQueue_t *queue) {
    if (queue == NULL)
        return NULL;

    if (queue->tail == NULL)
        return NULL;

    if (queue->tail->prev != NULL)
        queue->tail = queue->tail->prev;
    else
        queue->tail = NULL;

    return queue;
}
