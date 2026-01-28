#include "ShapeFactory.hpp"
#include "circle.hpp"
#include "triangle.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Missing shape argument.\n";
    return 1;
  }

  const std::string_view target_shape = argv[1];
  auto shape = geometry::ShapeFactory::get().create(target_shape);

  if (!shape) {
    std::cerr << "Object '" << target_shape
              << "' is not supported by the current framework.\n";
    return 1;
  }

  // will only return info here -> because only 2 argsgiven
  if (argc == 2) {
    shape->displayInfo();
    return 0;
  }

  // parsing the command line here
  std::vector<double> dims;
  for (int i = 2; i < argc; ++i) {
    try {
      dims.push_back(std::stod(argv[i]));
    } catch (...) {
      std::cerr << "Invalid numeric input: " << argv[i] << "\n";
      return 1;
    }
  }

  if (!shape->setDimensions(dims)) {
    std::cerr << "Configuration Error!\nRequired: " << shape->getUsage()
              << "\n";
    return 1;
  }

  std::cout << std::fixed << std::setprecision(4);
  for (const auto &[metric, value] : shape->calculate()) {
    std::cout << " > " << metric << " : " << value
              << std::endl;
  }

  return 0;
}
