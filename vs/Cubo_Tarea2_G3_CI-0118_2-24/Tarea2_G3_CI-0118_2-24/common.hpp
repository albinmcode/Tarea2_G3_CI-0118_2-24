// commmon definitions

// Vector
#define VECTOR_SIZE 3
#define VECTOR_EXTEND (VECTOR_SIZE + 1)
// Matrix
#define MATRIZ_SIZE 16
// Cube
#define CUBE_VERTICES 24
#define CUBE_VERTICES_ARRAY (CUBE_VERTICES*3)

// Classes
#define DECLARE_RULE4(Class, action) \
  Class(const Class& other) = action; \
  Class(Class&& other) = action; \
  Class& operator=(const Class& other) = action; \
  Class& operator=(Class&& other) = action

#define DISABLE_COPY(Class) \
  DECLARE_RULE4(Class, delete)
