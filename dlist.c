// dlist.c
#include "dlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <assert.h>

struct dlist *dlist_init(void) {
    struct dlist *list = malloc(sizeof(struct dlist));
    if (!list) {
        err(EXIT_FAILURE, "Failed to allocate memory for dlist");
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

int dlist_push_front(struct dlist *list, int element) {
    if (element < 0) {
        errno = EINVAL;
        return 0;
    }

    struct dlist_item *new_item = malloc(sizeof(struct dlist_item));
    if (!new_item) {
        err(EXIT_FAILURE, "Failed to allocate memory for dlist_item");
    }

    new_item->data = element;
    new_item->next = list->head;
    new_item->prev = NULL;

    if (list->head) {
        list->head->prev = new_item;
    } else {
        // Empty list, set the tail also
        list->tail = new_item;
    }

    list->head = new_item;
    list->size++;

    return 1;
}

void dlist_print(const struct dlist *list) {
    const struct dlist_item *current = list->head;
    while (current) {
        printf("%d$\n", current->data);
        current = current->next;
    }
}

int dlist_push_back(struct dlist *list, int element) {
    if (element < 0) {
        errno = EINVAL;
        return 0;
    }

    struct dlist_item *new_item = malloc(sizeof(struct dlist_item));
    if (!new_item) {
        err(EXIT_FAILURE, "Failed to allocate memory for dlist_item");
    }

    new_item->data = element;
    new_item->next = NULL;
    new_item->prev = list->tail;

    if (list->tail) {
        list->tail->next = new_item;
    } else {
        // Empty list, set the head also
        list->head = new_item;
    }

    list->tail = new_item;
    list->size++;

    return 1;
}

size_t dlist_size(const struct dlist *list) {
    return list->size;
}

int dlist_get(const struct dlist *list, size_t index) {
    if (index >= list->size) {
        errno = EINVAL;
        return -1;
    }

    struct dlist_item *current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    return current->data;
}

int dlist_insert_at(struct dlist *list, int element, size_t index) {
    if (element < 0 || index > list->size) {
        errno = EINVAL;
        return -1;
    }

    if (index == list->size) {
        // Insertion at the end, equivalent to push_back
        return dlist_push_back(list, element);
    }

    if (index == 0) {
        // Insertion at the beginning, equivalent to push_front
        return dlist_push_front(list, element);
    }

    struct dlist_item *current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    struct dlist_item *new_item = malloc(sizeof(struct dlist_item));
    if (!new_item) {
        err(EXIT_FAILURE, "Failed to allocate memory for dlist_item");
    }

    new_item->data = element;
    new_item->next = current;
    new_item->prev = current->prev;
    current->prev->next = new_item;
    current->prev = new_item;

    list->size++;

    return 1;
}

int dlist_find(const struct dlist *list, int element) {
    struct dlist_item *current = list->head;
    size_t index = 0;
    while (current) {
        if (current->data == element) {
            return index;
        }
        current = current->next;
        ++index;
    }

    return -1;
}

int dlist_remove_at(struct dlist *list, size_t index) {
    if (index >= list->size) {
        errno = EINVAL;
        return -1;
    }

    struct dlist_item *current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    if (current->prev) {
        current->prev->next = current->next;
    } else {
        // Removing the head
        list->head = current->next;
    }

    if (current->next) {
        current->next->prev = current->prev;
    } else {
        // Removing the tail
        list->tail = current->prev;
    }

    int data = current->data;
    free(current);
    list->size--;

    return data;
}

void dlist_clear(struct dlist *list) {
    while (list->head) {
        struct dlist_item *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void dlist_map_square(struct dlist *list) {
    struct dlist_item *current = list->head;
    while (current) {
        current->data = current->data * current->data;
        current = current->next;
    }
}

void dlist_reverse(struct dlist *list) {
   
