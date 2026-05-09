#include <any>
#include <print>
#include <functional>
#include <vector>
#include <numbers>
#include <meta>

template <class R, class T, class... Args>
constexpr R make_interface(Args&&... args)
{
    R ret;
    ret.value = T(std::forward<Args>(args)...);

    constexpr auto ctx = std::meta::access_context::current();
    template for (constexpr auto ret_member : std::define_static_array(nonstatic_data_members_of(^^R, ctx)))
    {
        template for (constexpr auto t_member : std::define_static_array(members_of(^^T, ctx)))
        {
            if constexpr (has_identifier(ret_member) && has_identifier(t_member) && identifier_of(ret_member) == identifier_of(t_member))
            {
                ret.[:ret_member:] = std::bind(&[:t_member:], std::any_cast<T const&>(ret.value));
            }
        }
    }

    return ret;
}

class Circle
{
public:
    explicit Circle(float radius) : _radius{radius}
    {
    }

    float area() const
    {
        return std::numbers::pi_v<float> * _radius * _radius;
    }

private:
    float _radius = 0.0f;
};

class Rect
{
public:
    explicit Rect(float length, float width) : _length{length}, _width{width}
    {
    }

    float area() const
    {
        return _length * _width;
    }

private:
    float _length = 0.0f;
    float _width  = 0.0f;
};

struct Shape;
consteval
{
    std::vector<std::meta::info> shape_members =
        {
            std::meta::data_member_spec(^^std::any, {.name = "value"}),
            std::meta::data_member_spec(^^std::function<float()>, {.name = "area"}),
        };
    std::meta::define_aggregate(^^Shape, shape_members);
}

template <class T, class... Args>
constexpr auto make_shape(Args&&... args)
{
    return make_interface<Shape, T>(std::forward<Args>(args)...);
}

int main()
{
    std::vector<Shape> shapes;
    shapes.push_back(make_shape<Circle>(1.0f));
    shapes.push_back(make_shape<Rect>(1.0f, 2.0f));
    for (auto const& shape : shapes)
    {
        std::println("{}", shape.area());
    }

    // Sample output:
    // 3.1415927
    // 2
}
