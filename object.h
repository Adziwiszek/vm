#ifndef clox_object_h
#define clox_object_h

#include <string.h>

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->str)

typedef enum {
  OBJ_STRING,
} ObjType;

struct Obj {
  ObjType type;
  struct Obj* next;
};

void printObject(Value value);

struct ObjString {
  Obj obj;
  int length;
  char* str;
  uint32_t hash;
};

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && OBJ_TYPE(value) == type;
}

#endif