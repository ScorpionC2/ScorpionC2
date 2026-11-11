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
     * @note Must be initialized using the value returned by queue->nextID++ (ensure increment after use).
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

    /**
     * @brief DLL Cursor (current node).
     *
     * This cursor exists because I didn't find a idiomatic way
     * to get the next task in queue. I tried creating a _nextByID()
     * and _prevByID() BUT the task (exposed to the user) haven't a
     * ID parameter, the only that holds it is the SchedQueue_Node_t,
     * which isn't exposed by default to the user. So in a need to get
     * the current->next task, I added this cursor.
     *
     * @note Don't manually manipulate.
     * @note Must be initialized by _initQueue().
     * @note Caller owns it and must release the memory if it is no longer needed.
     * @note It can be the same node as head or/and tail.
     *
     * @warning Might be null, if manual handling is truly needed (not recomended), please check it.
     */
    struct SchedQueue_Node_t *cursor;

    /**
     * @brief Tracks the next ID.
     *
     * @note This structure was created to optimize the ID assignment workload.
     *       Previously the _getNewID function was iterating a LCG PRNG Algorithm
     *       over every DLL node, basically O(n). This field remove this overhead.
     */
    uint64_t nextID;
};

/**
 * @brief Remove a node from the DLL.
 *
 * Remove a node by updating next and
 * previous node in the struct so they
 * don't "see" the removed node.
 *
 * @param node Pointer to the node to be removed.
 * @param queue Pointer to the node's queue.
 *
 * @return Pointer to the removed node.
 * @retval NULL Parameter nullity fallback.
 *
 * @note The node aren't deleted, just aren't visible in the DLL.
 * @note If the @p node is the unique @p queue's node, the @p queue become NULL.
 *
 * @pre @p node being a valid pointer to a SchedQueue_Node_t instance.
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance that must be the @p node queue.
 *
 * @post @p node not visible by other nodes in the DLL.
 * @post @p queue without @p node.
 */
void *_removeNode(struct SchedQueue_Node_t *node, struct SchedQueue_t *queue);

/**
 * @brief Delete a node from the DLL.
 *
 * @param node Pointer to the node to be deleted.
 * @param queue Pointer to the node's queue.
 *
 * @note The node is permanentely deleted and it's memory released.
 *
 * @pre @p node being a valid pointer to a SchedQueue_Node_t instance.
 * @pre @p queue being a valid pointer to a valid SchedQueue_t instance that must be the @p node queue.
 *
 * @post @p node deleted.
 * @post @p queue without @p node.
 */
void _deleteNode(struct SchedQueue_Node_t *node, struct SchedQueue_t *queue);

/**
 * @brief Initializate a new queue.
 *
 * @return Pointer to the new queue.
 * @retval NULL Memory error.
 *
 * @note Caller owns the returned queue and must release it's memory.
 */
struct SchedQueue_t *_initQueue();

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
struct SchedTask_t *_nextByID(struct SchedQueue_t *queue, uint64_t id);

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
struct SchedTask_t *_prevByID(struct SchedQueue_t *queue, uint64_t id);

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
                               struct SchedTask_t *task);

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
struct SchedTask_t *_setCursorByID(struct SchedQueue_t *queue, uint64_t id);

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
struct SchedTask_t *_setNextCursor(struct SchedQueue_t *queue);

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
struct SchedTask_t *_setPrevCursor(struct SchedQueue_t *queue);
