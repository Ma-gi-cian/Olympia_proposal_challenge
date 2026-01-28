#pragma once

#include <iostream>
#include <map>
#include <vector>

#include <functional>
#include <iomanip>

#include <string>
#include <string_view>

#include <memory>

namespace geometry {

class Shape {
public:
  virtual ~Shape() = default;

  virtual std::string getName() const = 0;
  virtual std::string getDescription() const = 0;
  virtual std::string getUsage() const = 0;

  virtual bool setDimensions(const std::vector<double> &dims) = 0;

  virtual std::map<std::string, double> calculate() const = 0;

  void displayInfo() const {
    std::cout << "Object Info: " << getDescription() << "\nUsage" << getUsage()
              << std::endl;
  }
};

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
}

#define REGISTER_GEOMETRY(Class, Name)                                         \
  static inline geometry::Registrar<Class> global_reg_##Class(Name)
