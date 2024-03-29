#ifndef LAB2_DECL_H
#define LAB2_DECL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct element {
    union {
        char string_val[30];
        bool boolean;
        int32_t num;
        double double_num;
    };
    int element_type;
};

struct filter_scheme {
    int operator_val;
    bool is_single_val;
    struct element* val;
    char attribute[30]; /*optional*/
};

struct property_scheme {
    int node_type;
    char property_name[30];
};

struct Filter_obj {
    union {
        struct filter_scheme* filter;
        struct property_scheme* prop;
    };
    struct Filter_obj* next;
};

struct property {
    char* attr_name;
    struct element* el;
    struct property* next;
};

struct Filter_obj* create_filter_obj(int op, char* attribute, struct element* el);
struct Filter_obj* create_single_filter_obj(struct element* el);
struct Filter_obj* create_property_scheme_filter_obj(char* prop_name, int type);
void free_filter(struct Filter_obj* filter);

void print_newline();
void print_tab();
void print_filter_obj(struct Filter_obj* obj);
void print_element(struct element* el);
void print_filters(struct Filter_obj* filter);

struct element* add_int32_element(int32_t val);
struct element* add_bool_element(int val);
struct element* add_str_element(char* val);
struct element* add_double_element(double val);

#endif //LAB2_DECL_H
