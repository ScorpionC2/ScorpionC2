//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "src/core/infra/sched/queue/main.h"
#include "infra/sched/tasks/stack/main.h"
#include "main.h"

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
 * @brief Initializate a new queue.
 *
 * @return Pointer to the new queue.
 * @retval NULL Memory error.
 *
 * @note Caller owns the returned queue and must release it's memory.
 */
struct SchedQueue_t *_initQueue() {
    struct SchedQueue_t *out = malloc(sizeof(struct SchedQueue_t));
    if (out == NULL)
        return NULL;

    out->head = NULL;
    out->tail = NULL;
    out->cursor = NULL;
    out->nextID = 1;

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
 * @retval NULL Memory error.
 * @retval NULL @p queue or @p task nullity fallback.
 *
 * @note Caller owns the created node, and must release it's memory.
 * @note The new task might be the head too if the queue is empty.
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

    struct SchedQueue_Node_t *node = malloc(sizeof(struct SchedQueue_Node_t));
    if (node == NULL)
        return NULL;

    node->id = queue->nextID++;
    node->next = NULL;
    node->prev = queue->tail;
    node->task = task;

    if (queue->tail != NULL)
        queue->tail->next = node;

    if (queue->head == NULL)
        queue->head = node;

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
 * @retval NULL Memory error.
 * @retval NULL @p queue or @p task nullity fallback.
 *
 * @note Caller owns the created node, and must release it's memory.
 * @note The new task might be the tail too if the queue is empty.
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

    struct SchedQueue_Node_t *node = malloc(sizeof(struct SchedQueue_Node_t));
    if (node == NULL)
        return NULL;

    node->id = queue->nextID++;
    node->next = queue->head;
    node->prev = NULL;
    node->task = task;

    if (queue->head != NULL)
        queue->head->prev = node;

    if (queue->tail == NULL)
        queue->tail = node;

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

    // This is a control flow optimization.
    // If the user tries to find the cursor
    // by it's ID, this ensure fast returning.
    if (queue->cursor != NULL) {
        if (id == queue->cursor->id)
            return queue->cursor->task;
    }

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

/**
 * @brief Return the next task of a id in a queue.
 *
 * @param queue A pointer to the SchedQueue_t instance for searching.
 * @param id Task ID to get the next task.
 *
 * @return Pointer to the found task.
 * @retval NULL - Parameter nullity fallback;
 *              - Task haven't a valid next (probably is tail);
 *              - Task with @p id was not found.
 */
struct SchedTask_t *_nextByID(struct SchedQueue_t *queue, uint64_t id) {
    if (queue == NULL)
        return NULL;

    if (queue->head == NULL)
        return NULL;

    // This is the same control flow optimization
    // seen in _findTask().
    if (queue->cursor != NULL) {
        if (id == queue->cursor->id)
            return queue->cursor->task;
    }

    struct SchedQueue_Node_t *current = queue->head;
    while (current != NULL) {
        if (current->id == id)
            return current->next->task ? current->next->task : NULL;

        current = current->next;
    }

    return NULL;
}

/**
 * @brief Return the prev task of a id in a queue.
 *
 * @param queue A pointer to the SchedQueue_t instance for searching.
 * @param id Task ID to get the prev task.
 *
 * @return Pointer to the found task.
 * @retval NULL - Parameter nullity fallback;
 *              - Task haven't a valid prev (probably is head);
 *              - Task with @p id was not found.
 */
struct SchedTask_t *_prevByID(struct SchedQueue_t *queue, uint64_t id) {
    if (queue == NULL)
        return NULL;

    if (queue->head == NULL)
        return NULL;

    // This is the same control flow optimization
    // seen in _findTask().
    if (queue->cursor != NULL) {
        if (id == queue->cursor->id)
            return queue->cursor->task;
    }

    struct SchedQueue_Node_t *current = queue->head;
    while (current != NULL) {
        if (current->id == id)
            return current->prev->task ? current->prev->task : NULL;

        current = current->next;
    }

    return NULL;
}

/**
 * @brief Set the cursor to a new task.
 *
 * @param queue A pointer to the SchedQueue_t that will have itself cursor set.
 * @param task A pointer to the SchedTask_t that'll be the new @p queue's cursor.
 *
 * @return Pointer to the new cursor.
 * @retval NULL Parameter nullity fallback or memory error.
 *
 * @note The created SchedQueue_Node_t is owned by the caller, that must release it.
 * @note If @p queue->cursor is NULL, the new task will be prepended.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @pre @p task being a valid pointer to a valid SchedTask_t instance.
 *
 * @post @p queue with a new task inserted as the cursor.
 * @post @p task unchanged.
 */
struct SchedTask_t *_setCursor(struct SchedQueue_t *queue,
                               struct SchedTask_t *task) {
    if (queue == NULL || task == NULL)
        return NULL;

    if (queue->cursor == NULL) {
        void *a = _prependQueue(queue, task);
        if (a == NULL)
            return NULL;

        queue->cursor = queue->head;
        return task;
    }

    struct SchedQueue_Node_t *nnode = malloc(sizeof(struct SchedQueue_Node_t));
    if (nnode == NULL)
        return NULL;

    nnode->next = NULL;
    nnode->prev = NULL;
    nnode->task = task;
    nnode->id = queue->nextID++;

    // [Cursor] <-> [Cursor->next]
    // ↓
    // [Cursor] <-> [nnode] <-> [Cursor->next]
    nnode->next = queue->cursor->next;
    nnode->prev = queue->cursor;
    if (nnode->next != NULL)
        nnode->next->prev = nnode;

    queue->cursor->next = nnode;
    queue->cursor = nnode;

    return task;
}

/**
 * @brief Set the queue cursor by another task in the same queue, using it's ID.
 *
 * @param queue A pointer to the SchedQueue_t iterated by the function.
 * @param id The task that'll be the new cursor id.
 *
 * @return Pointer to the task.
 * @retval NULL Param nullity fallback.
 * @retval NULL ID not found.
 *
 * @note The current @p queue->cursor isn't deleted.
 *
 * @pre @p queue being a vali pointer to a vali SchedQueue_t instance.
 * @pre @p id being the id for the task that'll be set as the cursor.
 *
 * @post @p queue with the new cursor set.
 * @post @p id unchanged.
 */
struct SchedTask_t *_setCursorByID(struct SchedQueue_t *queue, uint64_t id) {
    if (queue == NULL)
        return NULL;

    if (queue->head == NULL)
        return NULL;

    // This is a control flow optimization.
    // If the user tries to set the cursor
    // by it's own ID, this ensure fast returning.
    if (queue->cursor != NULL) {
        if (id == queue->cursor->id)
            return queue->cursor->task;
    }

    struct SchedQueue_Node_t *current = queue->head;
    while (current != NULL) {
        if (current->id == id) {
            queue->cursor = current;
            return queue->cursor->task;
        }

        current = current->next;
    }

    return NULL;
}

/**
 * @brief Set the cursor as the next.
 *
 * @param queue A pointer to the SchedQueue_t instance that'll have it's cursor changed.
 * @return Pointer to the new cursor's task.
 *
 * @retval NULL Param nullity error.
 * @retval NULL @p queue->cursor set to NULL.
 *
 * @note If the @p queue->cursor is the tail, it'll be set NULL.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @post @p queue with the cursor changed.
 */
struct SchedTask_t *_setNextCursor(struct SchedQueue_t *queue) {
    if (queue == NULL)
        return NULL;

    if (queue->cursor == NULL)
        return NULL;

    if (queue->cursor->next == NULL) {
        queue->cursor = NULL;
        return NULL;
    }

    queue->cursor = queue->cursor->next;
    return queue->cursor->task;
}

/**
 * @brief Set the cursor as the prev.
 *
 * @param queue A pointer to the SchedQueue_t instance that'll have it's cursor changed.
 * @return Pointer to the new cursor's task.
 *
 * @retval NULL Param nullity error.
 * @retval NULL @p queue->cursor set to NULL.
 *
 * @note If the @p queue->cursor is the head, it'll be set NULL.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @post @p queue with the cursor changed.
 */
struct SchedTask_t *_setPrevCursor(struct SchedQueue_t *queue) {
    if (queue == NULL)
        return NULL;

    if (queue->cursor == NULL)
        return NULL;

    if (queue->cursor->prev == NULL) {
        queue->cursor = NULL;
        return NULL;
    }

    queue->cursor = queue->cursor->prev;
    return queue->cursor->task;
}
