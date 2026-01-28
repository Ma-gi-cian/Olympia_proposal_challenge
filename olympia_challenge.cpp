
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include <memory>
#include <string>
#include <string_view>

namespace geometry {

// SHape interface 

class Shape {
public:
  virtual ~Shape() = default;

  virtual std::string getName() const = 0;
  virtual std::string getDescription() const = 0;
  virtual std::string getUsage() const = 0;
  virtual bool setDimensions(const std::vector<double> &dims) = 0;
  virtual std::map<std::string, double> calculate() const = 0;

  void displayInfo() const {
    std::cout << "Object Info: " << getDescription()
              << "\nUsage: " << getUsage() << std::endl;
  }
};

// The factory class 
class ShapeFactory {
public:
  using CreateFunc = std::function<std::unique_ptr<Shape>()>;

  static ShapeFactory &get() {
    static ShapeFactory instance;
    return instance;
  }

  void registerShape(std::string_view name, CreateFunc creator) {
    creators_[std::string(name)] = std::move(creator);
  }

  std::unique_ptr<Shape> create(std::string_view name) const {
    auto it = creators_.find(std::string(name));
    return (it != creators_.end()) ? it->second() : nullptr;
  }

private:
  std::map<std::string, CreateFunc> creators_;
  ShapeFactory() = default;
  ShapeFactory(const ShapeFactory &) = delete;
  ShapeFactory &operator=(const ShapeFactory &) = delete;
};


template <typename T> struct Registrar {
  explicit Registrar(std::string_view name) {
    ShapeFactory::get().registerShape(name,
                                      []() { return std::make_unique<T>(); });
  }
};

// Register macro
#define REGISTER_GEOMETRY(Class, Name)                                         \
  static inline geometry::Registrar<Class> global_reg_##Class(Name)



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
    if (dims.size() < 2 || dims[0] <= 0 || dims[1] <= 0) {
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

    return {{"area", area}, {"circumference", perimeter}};
  }

private:
  double base_{0.0};
  double height_{0.0};
};


REGISTER_GEOMETRY(Circle, "circle");
REGISTER_GEOMETRY(Triangle, "triangle");

} 


int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Missing shape argument.\n";
    std::cerr << "Usage: " << argv[0] << " <shape> [dimensions...]\n";
    return 1;
  }

  const std::string_view target_shape = argv[1];
  auto shape = geometry::ShapeFactory::get().create(target_shape);

  if (!shape) {
    std::cerr << "Object '" << target_shape
              << "' is not supported by the current framework.\n";
    std::cerr << "Available shapes: circle, triangle\n";
    return 1;
  }

  // Display info if no dimensions provided
  if (argc == 2) {
    shape->displayInfo();
    return 0;
  }

  // Parse dimensions from command line
  std::vector<double> dims;
  for (int i = 2; i < argc; ++i) {
    try {
      dims.push_back(std::stod(argv[i]));
    } catch (...) {
      std::cerr << "Invalid numeric input: " << argv[i] << "\n";
      return 1;
    }
  }

  // Set dimensions and calculate
  if (!shape->setDimensions(dims)) {
    std::cerr << "Configuration Error!\nRequired: " << shape->getUsage()
              << "\n";
    return 1;
  }

  std::cout << std::fixed << std::setprecision(4);
  for (const auto &[metric, value] : shape->calculate()) {
    std::cout << " > " << metric << " : " << value << std::endl;
  }

  return 0;
}