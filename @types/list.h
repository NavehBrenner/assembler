#ifndef TYPES_LIST_H
#define TYPES_LIST_H

#define List(Type, name)                                                       \
  typedef struct name##Node {                                                  \
    Type data;                                                                 \
    struct name##Node *next;                                                   \
  } name##Node;                                                                \
                                                                               \
  name##Node *new##name##List(void) {                                          \
    name##Node *head = (name##Node *)malloc(sizeof(name##Node));               \
    head->next = NULL;                                                         \
    return head;                                                               \
  }                                                                            \
                                                                               \
  name##Node *empty##name(void) { return new##name##List(); }                  \
  void free##name##List(name##Node *head) {                                    \
    name##Node *current = head;                                                \
    name##Node *next;                                                          \
    while (current != NULL) {                                                  \
      next = current->next;                                                    \
      free(current);                                                           \
      current = next;                                                          \
    }                                                                          \
  }                                                                            \
                                                                               \
  void push##name(name##Node *head, Type data) {                               \
    name##Node *newNode = (name##Node *)malloc(sizeof(name##Node));            \
    newNode->data = data;                                                      \
    newNode->next = NULL;                                                      \
    name##Node *current = head;                                                \
    while (current->next != NULL) {                                            \
      current = current->next;                                                 \
    }                                                                          \
    current->next = newNode;                                                   \
  }                                                                            \
                                                                               \
  void pop##name(name##Node *head) {                                           \
    if (head->next == NULL) {                                                  \
      return;                                                                  \
    }                                                                          \
    name##Node *current = head;                                                \
    while (current->next->next != NULL) {                                      \
      current = current->next;                                                 \
    }                                                                          \
    free(current->next);                                                       \
    current->next = NULL;                                                      \
  }                                                                            \
                                                                               \
  int isEmpty##name(name##Node *head) { return head->next == NULL; }

#endif