# C++26 Type Erasure/Interface with Reflection

This project demonstrates a technique for creating type-erased interfaces in C++26 using compile-time reflection. It allows different types with compatible member functions to be treated uniformly through a common interface without inheritance or virtual functions.

## Requirements

- A C++26 compiler with support for compile-time reflection (e.g., GCC with experimental reflection features or Clang with appropriate flags)
- CMake 3.15+ (optional, for building)
- Standard library support for `<any>`, `<functional>`, `<print>`, `<vector>`, `<numbers>`, and `<meta>`

## How It Works

The core idea is to use C++26's reflection capabilities to:
1. Define an interface structure (`Shape`) that specifies the desired members (data and functions)
2. Create a type-erased container that holds:
   - A `std::any` object storing the actual concrete object
   - `std::function` objects for each interface member, bound to the concrete object's methods
3. Use reflection to automatically map interface members to corresponding members of concrete types at compile time

## How It Compares to Traditional Approaches

### Traditional Virtual Function Approach
```cpp
class ShapeInterface {
public:
    virtual float area() const = 0;
    virtual ~ShapeInterface() = default;
};

class Circle : public ShapeInterface { /* ... */ };
class Rect : public ShapeInterface { /* ... */ };

std::vector<std::unique_ptr<ShapeInterface>> shapes;
shapes.push_back(std::make_unique<Circle>(1.0f));
// Requires inheritance, virtual function table, and heap allocation
```

### This Reflection-Based Approach
- No inheritance hierarchy needed
- No virtual function table overhead
- All interface binding resolved at compile time
- Value semantics (objects stored directly in vector, no heap allocation for small types)
- Works with any type that has the required members, regardless of their hierarchy
