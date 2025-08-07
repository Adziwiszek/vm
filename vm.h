#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

typedef struct {
  Chunk *chunk;
  uint8_t *ip;
  ValueArray stack;
  Value *stackTop;
} VM;

void initVM();
void freeVM();
void push(Value value);
Value pop();
InterpretResult interpret(const char* source);

static InterpretResult run();
static void upatedStackPointer();

#endif
