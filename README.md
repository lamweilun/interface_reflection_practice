# C++26 Type Erasure/Interface with Reflection

This project demonstrates a technique for creating type-erased interfaces in C++26 using compile-time reflection. It allows different types with compatible member functions to be treated uniformly through a common interface without inheritance or virtual functions.

## Requirements

- A C++26 compiler with support for compile-time reflection (e.g., GCC with experimental reflection features or Clang with appropriate flags)
- CMake 3.15+ (optional, for building)
- Standard library support for `<any>`, `<functional>`, `<print>`, `<vector>`, `<numbers>`, and `<meta>`

## Example
```C++
// Define a Circle type
class Circle {
public:
    explicit Circle(float radius) : _radius{radius} {}
    float area() const { return 3.14159f * _radius * _radius; }
private:
    float _radius = 0.0f;
};

// Define a Rect type
class Rect {
public:
    explicit Rect(float length, float width) : _length{length}, _width{width} {}
    float area() const { return _length * _width; }
private:
    float _length, _width = 0.0f;
};

// Define your Shape interface
struct Shape;
consteval {
    std::vector<std::meta::info> shape_members =
    {
        std::meta::data_member_spec(^^std::any, {.name = "value"}),
        std::meta::data_member_spec(^^std::function<float()>, {.name = "area"}),
    };
    std::meta::define_aggregate(^^Shape, shape_members);
}

// make_interface implementation detail... (Look into main.cpp for the source code)

int main() {
    // Now you can associate Circle and Rect by their "area" member function
    std::vector<Shape> shapes;
    shapes.push_back(make_interface<Shape, Circle>(1.0f));
    shapes.push_back(make_interface<Shape, Rect>(1.0f, 2.0f));
    for (auto const& shape : shapes) { std::println("{}", shape.area()); }
    // Sample output:
    // 3.1415927
    // 2
}
```

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

## NOTE
- This project serves as a practice for code generation via reflection, the implementation is currently incomplete. For eg. Does not work with non-const member functions.
