#ifndef _MG_STD_MG_LIST_H
#define _MG_STD_MG_LIST_H

#include <stdio.h>
#include <stddef.h>
//#include <stdlib.h>

typedef struct mglist_head
{
    struct mglist_head *prev;
    struct mglist_head *next;
}MG_LIST;

#define MG_LIST_POISON1  NULL
#define MG_LIST_POISON2  NULL

#define MG_LIST_HEAD_INIT(name) { &(name), &(name) }

#define MG_LIST_HEAD(name) \
	struct mglist_head name = MG_LIST_HEAD_INIT(name)


/*******************************************************/


void INIT_MG_LIST_HEAD(struct mglist_head *list);

/*
    在head后面添加new
*/
extern void mglist_add(struct mglist_head *new, struct mglist_head *head);

/*
   在head前面添加new
 */
extern void mglist_add_tail(struct mglist_head *new, struct mglist_head *head);

/*
   删除entry
*/
extern void mglist_del(struct mglist_head *entry);

/*
   把new替换成old, old->prev, old->next不变
*/
extern void mglist_replace(struct mglist_head *old,
				struct mglist_head *new);

/*
    把new替换成old, 并将old尾巴清理掉
  */
extern void mglist_replace_init(struct mglist_head *old,
					struct mglist_head *new);

/*
   删除entry，并将entry的尾巴清理掉
   */
extern void mglist_del_init(struct mglist_head *entry);

/*
    将list剪切到head后面
 */
extern void mglist_move(struct mglist_head *list, struct mglist_head *head);

/*
   将list剪切到head前面
*/
extern void mglist_move_tail(struct mglist_head *list,
				  struct mglist_head *head);

/*
   判断list是否为最后一个
   是：返回1
   否：返回0
  */
extern int mglist_is_last(const struct mglist_head *list,
				const struct mglist_head *head);

/*
   判断链表是否为空
   false return 0
   True return 1 
   */
extern int mglist_empty(const struct mglist_head *head);

/*
   严格判断链表是否为空
 */
extern int mglist_empty_careful(const struct mglist_head *head);

/*
   左移
   将head->next左移到head->prev
 */
extern void mglist_rotate_left(struct mglist_head *head);


/*
   判断链表是否只有一个，即链表中只有一个head
*/
extern int mglist_is_singular(const struct mglist_head *head);

/*
   切割链表，将head(不包含)至entry(包含)之间的链表切割至list中去，并head自组织成一个小链表
*/   
extern void mglist_cut_position(struct mglist_head *list,
		struct mglist_head *head, struct mglist_head *entry);

/*
    合并链表，将list链表合并到head后面
 */
extern void mglist_splice(const struct mglist_head *list,
				struct mglist_head *head);
/*
   合并链表，将list链表合并插入到head前面
 */
extern void mglist_splice_tail(struct mglist_head *list,
				struct mglist_head *head);
/*
   合并链表，将list链表合并到head后面，并将list_head清除
 */
extern void mglist_splice_init(struct mglist_head *list,
				    struct mglist_head *head);

extern void mglist_splice_tail_init(struct mglist_head *list,
					 struct mglist_head *head);

/**
 * mglist_entry - get the struct for this entry
 * @ptr:	the &struct mglist_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the mglist_struct within the struct.
 */

#ifndef container_of
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define mglist_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * mglist_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define mglist_first_entry(ptr, type, member) \
	mglist_entry((ptr)->next, type, member)

/**
 * mglist_last_entry - get the last element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define mglist_last_entry(ptr, type, member) \
	mglist_entry((ptr)->prev, type, member)

/**
 * mglist_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define mglist_first_entry_or_null(ptr, type, member) \
	(!mglist_empty(ptr) ? mglist_first_entry(ptr, type, member) : NULL)

/**
 * mglist_next_entry - get the next element in list
 * @pos:	the type * to cursor
 * @member:	the name of the mglist_struct within the struct.
 */
#define mglist_next_entry(pos, member) \
	mglist_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * mglist_prev_entry - get the prev element in list
 * @pos:	the type * to cursor
 * @member:	the name of the mglist_struct within the struct.
 */
#define mglist_prev_entry(pos, member) \
	mglist_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * mglist_for_each	-	iterate over a list
 * @pos:	the &struct mglist_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define mglist_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * mglist_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct mglist_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define mglist_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * mglist_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct mglist_head to use as a loop cursor.
 * @n:		another &struct mglist_head to use as temporary storage
 * @head:	the head for your list.
 */
#define mglist_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * mglist_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct mglist_head to use as a loop cursor.
 * @n:		another &struct mglist_head to use as temporary storage
 * @head:	the head for your list.
 */
#define mglist_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

/**
 * mglist_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 */
#define mglist_for_each_entry(pos, head, member)				\
	for (pos = mglist_first_entry(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = mglist_next_entry(pos, member))

/**
 * mglist_for_each_entry_reverse - iterate backwards over list of given type.
 *  反向遍历mglist
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 */
#define mglist_for_each_entry_reverse(pos, head, member)			\
	for (pos = mglist_last_entry(head, typeof(*pos), member);		\
	     &pos->member != (head); 					\
	     pos = mglist_prev_entry(pos, member))

/**
 * mglist_prepare_entry - prepare a pos entry for use in mglist_for_each_entry_continue()
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the mglist_struct within the struct.
 *
 * Prepares a pos entry for use as a start point in mglist_for_each_entry_continue().
 */
#define mglist_prepare_entry(pos, head, member) \
	((pos) ? : mglist_entry(head, typeof(*pos), member))

/**
 * mglist_for_each_entry_continue - continue iteration over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define mglist_for_each_entry_continue(pos, head, member) 		\
	for (pos = mglist_next_entry(pos, member);			\
	     &pos->member != (head);					\
	     pos = mglist_next_entry(pos, member))

/**
 * mglist_for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define mglist_for_each_entry_continue_reverse(pos, head, member)		\
	for (pos = mglist_prev_entry(pos, member);			\
	     &pos->member != (head);					\
	     pos = mglist_prev_entry(pos, member))

/**
 * mglist_for_each_entry_from - iterate over list of given type from the current point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define mglist_for_each_entry_from(pos, head, member) 			\
	for (; &pos->member != (head);					\
	     pos = mglist_next_entry(pos, member))

/**
 * mglist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 */
#define mglist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = mglist_first_entry(head, typeof(*pos), member),	\
		n = mglist_next_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = mglist_next_entry(n, member))

/**
 * mglist_for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define mglist_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = mglist_next_entry(pos, member), 				\
		n = mglist_next_entry(pos, member);				\
	     &pos->member != (head);						\
	     pos = n, n = mglist_next_entry(n, member))

/**
 * mglist_for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define mglist_for_each_entry_safe_from(pos, n, head, member) 			\
	for (n = mglist_next_entry(pos, member);					\
	     &pos->member != (head);						\
	     pos = n, n = mglist_next_entry(n, member))

/**
 * mglist_for_each_entry_safe_reverse - iterate backwards over list safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the mglist_struct within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define mglist_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = mglist_last_entry(head, typeof(*pos), member),		\
		n = mglist_prev_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = mglist_prev_entry(n, member))

/**
 * mglist_safe_reset_next - reset a stale mglist_for_each_entry_safe loop
 * @pos:	the loop cursor used in the mglist_for_each_entry_safe loop
 * @n:		temporary storage used in mglist_for_each_entry_safe
 * @member:	the name of the mglist_struct within the struct.
 *
 * mglist_safe_reset_next is not safe to use in general if the list may be
 * modified concurrently (eg. the lock is dropped in the loop body). An
 * exception to this is if the cursor element (pos) is pinned in the list,
 * and mglist_safe_reset_next is called after re-taking the lock and before
 * completing the current iteration of the loop body.
 */
#define mglist_safe_reset_next(pos, n, member)				\
	n = mglist_next_entry(pos, member)

/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */


/*******************************************************************

  哈希链表

*******************************************************************/	
typedef struct mghlist_node
{
    struct mghlist_node *next, **pprev;
}MG_HLIST_NODE;

typedef struct mghlist_head
{
    struct mghlist_node *first;
}MG_HLIST;


#define MG_HLIST_HEAD_INIT { .first = NULL }
#define MG_HLIST_HEAD(name) struct mghlist_head name = {  .first = NULL }
#define INIT_MG_HLIST_HEAD(ptr) ((ptr)->first = NULL)

/*
 *init mghlist node, 设置h->next 和h->pprev 为NULL
 */
extern void INIT_MG_HLIST_NODE(struct mghlist_node *h);

/*
 *判断一个结点是否已经存在于hash桶中    
 */
extern int mghlist_unhashed(const struct mghlist_node *h);

/*
 *判断hash桶是否为空
 */
extern int mghlist_empty(const struct mghlist_head *h);

/*
 *删除hash桶中node结点，并将node结点尾巴清理掉
*/
extern void mghlist_del(struct mghlist_node *n);

/*
   严格删除node结点，并将node结点尾巴清理掉
*/
extern void mghlist_del_init(struct mghlist_node *n);

/*
   将普通结点n插入到头结点h对应的hash桶的第一个结点的位置
*/
extern void mghlist_add_head(struct mghlist_node *n, struct mghlist_head *h);

/* next must be != NULL */
/*
   //在next结点之前插入结点n，即使next结点是hash桶中的第一个结点也可以  
 */
extern void mghlist_add_before(struct mghlist_node *n,
					struct mghlist_node *next);

/*
   //在结点n之后插入结点next 
 */
extern void mghlist_add_after(struct mghlist_node *n,
					struct mghlist_node *next);

/* after that we'll appear to be on some mghlist and mghlist_del will work */
/*
   假添加，作用?
 */
extern void mghlist_add_fake(struct mghlist_node *n);

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
/*
 *将old剪切到new中
 */
extern void mghlist_move_list(struct mghlist_head *old, struct mghlist_head *new);

#define mghlist_entry(ptr, type, member) container_of(ptr,type,member)

#define mghlist_for_each(pos, head) \
	for (pos = (head)->first; pos ; pos = pos->next)

#define mghlist_for_each_safe(pos, n, head) \
	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	     pos = n)

#define mghlist_entry_safe(ptr, type, member) \
	({ typeof(ptr) ____ptr = (ptr); \
	   ____ptr ? mghlist_entry(____ptr, type, member) : NULL; \
	})

/**
 * mghlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the mghlist_node within the struct.
 */
#define mghlist_for_each_entry(pos, head, member)				\
	for (pos = mghlist_entry_safe((head)->first, typeof(*(pos)), member);\
	     pos;							\
	     pos = mghlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * mghlist_for_each_entry_continue - iterate over a hlist continuing after current point
 * @pos:	the type * to use as a loop cursor.
 * @member:	the name of the mghlist_node within the struct.
 */
#define mghlist_for_each_entry_continue(pos, member)			\
	for (pos = mghlist_entry_safe((pos)->member.next, typeof(*(pos)), member);\
	     pos;							\
	     pos = mghlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * mghlist_for_each_entry_from - iterate over a hlist continuing from current point
 * @pos:	the type * to use as a loop cursor.
 * @member:	the name of the mghlist_node within the struct.
 */
#define mghlist_for_each_entry_from(pos, member)				\
	for (; pos;							\
	     pos = mghlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * mghlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another &struct mghlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the mghlist_node within the struct.
 */
#define mghlist_for_each_entry_safe(pos, n, head, member) 		\
	for (pos = mghlist_entry_safe((head)->first, typeof(*pos), member);\
	     pos && ({ n = pos->member.next; 1; });			\
	     pos = mghlist_entry_safe(n, typeof(*pos), member))

#endif
