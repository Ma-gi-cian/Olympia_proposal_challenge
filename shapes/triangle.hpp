#include "ShapeFactory.hpp"
#include <cmath>

namespace geometry {
class Triangle : public Shape {
public:
  std::string getName() const override { return "Triangle"; }

  std::string getDescription() const override {
    return "Triangle has three sides, you need to give base and height";
  }

  std::string getUsage() const override {
    return "describe_object triangle <base> <height>";
  }

  bool setDimensions(const std::vector<double> &dims) override {
    if (dims.empty() || dims[0] <=0 || dims[1] <= 0){
        return false;
    }
    base_ = dims[0];
    height_ = dims[1];
    return true;
  }

  std::map<std::string, double> calculate() const override {

    double area = 0.5 * base_ * height_;

    double side = std::sqrt(std::pow(base_ / 2.0, 2) + std::pow(height_, 2));
    double perimeter = base_ + (2.0 * side);

    return {{"area", area}, {"circumfernce", perimeter}};
  }

  private:
    double base_{0.0};
    double height_{0.0};
};

REGISTER_GEOMETRY(Triangle, "triangle");
}