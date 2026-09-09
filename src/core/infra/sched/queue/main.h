//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "src/core/infra/sched/context/main.h"
#include "src/core/infra/sched/tasks/main.h"

#include <stdint.h>

/**
 * @brief Scheduler tasks queue node.
 *
 * @note You probably wants to use the SchedQueue_t wrappers instead of manually handling these nodes.
 */
struct SchedQueue_Node_t {
    /**
     * @brief Node unique ID.
     * @note Must be initialized using the value returned by _getNewID.
     */
    uint64_t id;

    /**
     * @brief Scheduler task.
     * @note Must be initialized by _initTask().
     */
    struct SchedTask_t *task;

    /**
     * @brief Previous task in the dll.
     * @note Must be handled by SchedQueue_t wrappers.
     */
    struct SchedQueue_Node_t *prev;

    /**
     * @brief Next task in the dll.
     * @note Must be handled by SchedQueue_t wrappers.
     */
    struct SchedQueue_Node_t *next;
};

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
void *_removeNode(struct SchedQueue_Node_t *node);

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
 * @brief The tasks queue.
 *
 * @note Must be initialized by _initQueue().
 * @note Must be handled by SchedQueue_t wrappers.
 */
struct SchedQueue_t {
    /**
     * @brief DLL head.
     *
     * @note Don't manually manipulate.
     * @note Must be initialized by _initQueue().
     * @note Caller owns it and must release the memory if it is no longer needed.
     *
     * @warning Might be null, please check if manual handling is needed (not recomended).
     */
    struct SchedQueue_Node_t *head;

    /**
     * @brief DLL tail.
     *
     * @note Don't manually manipulate.
     * @note Must be initialized by _initQueue().
     * @note Caller owns it and must release the memory if it is no longer needed.
     * @note It can be the same node as head.
     *
     * @warning Might be null, please check if manual handling is needed (not recomended).
     */
    struct SchedQueue_Node_t *tail;
};

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
uint64_t _getNewID(struct SchedQueue_t *queue);

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
struct SchedQueue_t *_initQueue(struct SchedTask_t *task);

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
void *_appendQueue(struct SchedQueue_t *queue, struct SchedTask_t *task);

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
void *_prependQueue(struct SchedQueue_t *queue, struct SchedTask_t *task);

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
struct SchedTask_t *_findTask(struct SchedQueue_t *queue, uint64_t id);

/**
 * @brief Delete the queue head.
 *
 * @param queue A pointer to the SchedQueue_t instance that will have it's head deleted.
 *
 * @return Pointer to @p queue.
 * @retval NULL Parameter nullity fallback.
 *
 * @note If @p queue->head->next is NULL, queue head will be NULL too.
 *
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance.
 * @post @p queue unchanged.
 */
void *_deleteHead(struct SchedQueue_t *queue);

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
void *_deleteTail(struct SchedQueue_t *queue);

void *_removeHead(struct SchedQueue_t *queue);

void *_removeTail(struct SchedQueue_t *queue);
