#include "ShapeFactory.hpp"
#include <cmath>

namespace geometry {

class Circle : public Shape {
public:
  std::string getName() const override { return "Circle"; }
  std::string getDescription() const override {
    return "Round like the moon, defined by a radius.";
  }
  std::string getUsage() const override {
    return "describe_object circle <radius>";
  }

  bool setDimensions(const std::vector<double> &dims) override {
    if (dims.empty() || dims[0] <= 0)
      return false;
    radius_ = dims[0];
    return true;
  }

  std::map<std::string, double> calculate() const override {
    return {{"area", M_PI * std::pow(radius_, 2)},
            {"circumference", 2 * M_PI * radius_}};
  }

private:
  double radius_{0.0};
};

REGISTER_GEOMETRY(Circle, "circle");

} 
