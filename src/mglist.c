#include "./mglist.h"

void INIT_MG_LIST_HEAD(struct mglist_head *list)
{
	list->next = list;
	list->prev = list;
}

static void __mglist_add(struct mglist_head *new,
			      struct mglist_head *prev,
			      struct mglist_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void mglist_add(struct mglist_head *new, struct mglist_head *head)
{
	__mglist_add(new, head, head->next);
}


void mglist_add_tail(struct mglist_head *new, struct mglist_head *head)
{
	__mglist_add(new, head->prev, head);
}

void __mglist_del(struct mglist_head * prev, struct mglist_head * next)
{
	next->prev = prev;
	prev->next = next;
}

void __mglist_del_entry(struct mglist_head *entry)
{
	__mglist_del(entry->prev, entry->next);
}

void mglist_del(struct mglist_head *entry)
{
	__mglist_del(entry->prev, entry->next);
	entry->next = MG_LIST_POISON1;
	entry->prev = MG_LIST_POISON2;
}

void mglist_replace(struct mglist_head *old,
				struct mglist_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

void mglist_replace_init(struct mglist_head *old,
					struct mglist_head *new)
{
	mglist_replace(old, new);
	INIT_MG_LIST_HEAD(old);
}

void mglist_del_init(struct mglist_head *entry)
{
	__mglist_del_entry(entry);
	INIT_MG_LIST_HEAD(entry);
}

/**
 * mglist_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
void mglist_move(struct mglist_head *list, struct mglist_head *head)
{
	__mglist_del_entry(list);
	mglist_add(list, head);
}

/**
 * mglist_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
void mglist_move_tail(struct mglist_head *list,
				  struct mglist_head *head)
{
	__mglist_del_entry(list);
	mglist_add_tail(list, head);
}

/**
 * mglist_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
int mglist_is_last(const struct mglist_head *list,
				const struct mglist_head *head)
{
	return list->next == head;
}

/**
 * mglist_empty - tests whether a list is empty
 * @head: the list to test.
 */
int mglist_empty(const struct mglist_head *head)
{
	return head->next == head;
}

/**
 * mglist_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using mglist_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is mglist_del_init(). Eg. it cannot be used
 * if another CPU could re-mglist_add() it.
 */
int mglist_empty_careful(const struct mglist_head *head)
{
	struct mglist_head *next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * mglist_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
void mglist_rotate_left(struct mglist_head *head)
{
	struct mglist_head *first;

	if (!mglist_empty(head)) {
		first = head->next;
		mglist_move_tail(first, head);
	}
}

/**
 * mglist_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
int mglist_is_singular(const struct mglist_head *head)
{
	return !mglist_empty(head) && (head->next == head->prev);
}

void __mglist_cut_position(struct mglist_head *list,
		struct mglist_head *head, struct mglist_head *entry)
{
	struct mglist_head *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * mglist_cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
void mglist_cut_position(struct mglist_head *list,
		struct mglist_head *head, struct mglist_head *entry)
{
	if (mglist_empty(head))
		return;
	if (mglist_is_singular(head) &&
		(head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_MG_LIST_HEAD(list);
	else
		__mglist_cut_position(list, head, entry);
}

void __mglist_splice(const struct mglist_head *list,
				 struct mglist_head *prev,
				 struct mglist_head *next)
{
	struct mglist_head *first = list->next;
	struct mglist_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * mglist_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
void mglist_splice(const struct mglist_head *list,
				struct mglist_head *head)
{
	if (!mglist_empty(list))
		__mglist_splice(list, head, head->next);
}

/**
 * mglist_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
void mglist_splice_tail(struct mglist_head *list,
				struct mglist_head *head)
{
	if (!mglist_empty(list))
		__mglist_splice(list, head->prev, head);
}

/**
 * mglist_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
void mglist_splice_init(struct mglist_head *list,
				    struct mglist_head *head)
{
	if (!mglist_empty(list)) {
		__mglist_splice(list, head, head->next);
		INIT_MG_LIST_HEAD(list);
	}
}

/**
 * mglist_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
void mglist_splice_tail_init(struct mglist_head *list,
					 struct mglist_head *head)
{
	if (!mglist_empty(list)) {
		__mglist_splice(list, head->prev, head);
		INIT_MG_LIST_HEAD(list);
	}
}
/*******************************************************.

  哈希链表

  ******************************************************/
void INIT_MG_HLIST_NODE(struct mghlist_node *h)
{
	h->next = NULL;
	h->pprev = NULL;
}

int mghlist_unhashed(const struct mghlist_node *h)
{
	return !h->pprev;
}

int mghlist_empty(const struct mghlist_head *h)
{
	return !h->first;
}

void __mghlist_del(struct mghlist_node *n)
{
	struct mghlist_node *next = n->next;
	struct mghlist_node **pprev = n->pprev;
	*pprev = next;
	if (next)
		next->pprev = pprev;
}

void mghlist_del(struct mghlist_node *n)
{
	__mghlist_del(n);
	n->next = MG_LIST_POISON1;
	n->pprev = MG_LIST_POISON2;
}

void mghlist_del_init(struct mghlist_node *n)
{
	if (!mghlist_unhashed(n)) {
		__mghlist_del(n);
		INIT_MG_HLIST_NODE(n);
	}
}

void mghlist_add_head(struct mghlist_node *n, struct mghlist_head *h)
{
	struct mghlist_node *first = h->first;
	n->next = first;
	if (first)
		first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

/* next must be != NULL */
void mghlist_add_before(struct mghlist_node *n,
					struct mghlist_node *next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

void mghlist_add_after(struct mghlist_node *n,
					struct mghlist_node *next)
{
	next->next = n->next;
	n->next = next;
	next->pprev = &n->next;

	if(next->next)
		next->next->pprev  = &next->next;
}

/* after that we'll appear to be on some hlist and mghlist_del will work */
void mghlist_add_fake(struct mghlist_node *n)
{
	n->pprev = &n->next;
}

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
void mghlist_move_list(struct mghlist_head *old,
				   struct mghlist_head *new)
{
	new->first = old->first;
	if (new->first)
		new->first->pprev = &new->first;
	old->first = NULL;
}

