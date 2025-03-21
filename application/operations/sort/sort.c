#include "sort.h"

#include <math.h>
#include <string.h>

#include <time.h>

int compare_tm(const struct tm first, const struct tm second) {
    int difference = (first.tm_hour * 3600 + first.tm_min * 60 + first.tm_sec)
        - (second.tm_hour * 3600 + second.tm_min * 60 + second.tm_sec);

    if (difference == 0)
        return 0;

    return difference / fabs(difference);
}

int compare_pid(const ProcessInfo first, const ProcessInfo second) {
    return (first.pid > second.pid) - (first.pid < second.pid);
}

int compare_name(const ProcessInfo first, const ProcessInfo second) {
    return strcmp(first.name, second.name);
}

int compare_priority(const ProcessInfo first, const ProcessInfo second) {
    return (first.priority > second.priority) - (first.priority < second.priority);
}

int compare_kern_tm(const ProcessInfo first, const ProcessInfo second) {
    return compare_tm(first.kern_tm, second.kern_tm);
}

int compare_file_tm(const ProcessInfo first, const ProcessInfo second) {
    return compare_tm(first.file_tm, second.file_tm);
}

int compare_cpu_usage(const ProcessInfo first, const ProcessInfo second) {
    return (first.cpu_usage > second.cpu_usage) - (first.cpu_usage < second.cpu_usage);
}

int compare_status(const ProcessInfo first, const ProcessInfo second) {
    return (first.status > second.status) - (first.status < second.status);
}

static int (*comparers[FIELDS_COUNT])(ProcessInfo, ProcessInfo) = {
    compare_pid, compare_name, compare_priority, compare_kern_tm, compare_file_tm, compare_cpu_usage, compare_status
};

int compare_processes(ProcessInfo first, ProcessInfo second, const SortField* fields, int fields_count) {
    for (int field_index = 0; field_index < fields_count; field_index++) {
        int compare_result = 0;
        for (int index = 0; index < FIELDS_COUNT; index++) {
            if (strcmp(fields[field_index].field, fields_string[index]) == 0) {
                compare_result = comparers[index](first, second);
                break;
            }
        }

        if (compare_result != 0)
            return fields[field_index].ascending
                ? compare_result
                : -compare_result;
    }

    return 0;
}

Node* merge_sorted_lists(Node* left, Node* right, const SortField* fields, int fields_count) {
    Node* head = NULL;
    Node** tail_pointer = &head;

    while (left && right) {
        if (compare_processes(left->data, right->data, fields, fields_count) <= 0) {
            *tail_pointer = left;
            left->previous = *tail_pointer == head
                ? NULL : *tail_pointer;

            left = left->next;
        }

        else {
            *tail_pointer = right;
            right->previous = *tail_pointer == head
                ? NULL : *tail_pointer;

            right = right->next;
        }

        tail_pointer = &(*tail_pointer)->next;
    }

    *tail_pointer = left != NULL ? left : right;
    if (*tail_pointer != NULL)
        (*tail_pointer)->previous = *tail_pointer == head
            ? NULL : *tail_pointer;

    return head;
}

void split_list(Node* head, Node** left, Node** right) {
    if (!head || !head->next) {
        *left = head;
        *right = NULL;
        return;
    }

    Node* slow = head;
    Node* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    *left = head;
    *right = slow->next;
    slow->next = NULL;
}

Node* merge_sort_recursive(Node* head, SortField* fields, int fields_count) {
    if (!head || !head->next) {
        return head;
    }
    Node* left;
    Node* right;

    split_list(head, &left, &right);
    left = merge_sort_recursive(left, fields, fields_count);
    right = merge_sort_recursive(right, fields, fields_count);

    return merge_sorted_lists(left, right, fields, fields_count);
}

void merge_sort(Database* database, SortField* fields, int fields_count) {
    if (!database || !database->head || fields_count <= 0)
        return;

    database->head = merge_sort_recursive(database->head, fields, fields_count);

    Node* temp = database->head;
    while (temp->next)
        temp = temp->next;

    database->tail = temp;
}