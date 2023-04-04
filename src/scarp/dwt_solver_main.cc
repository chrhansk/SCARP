#include <iostream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "scarp/bounds.hh"
#include "scarp/parameters.hh"
#include "scarp/instance_reader.hh"
#include "scarp/control_writer.hh"

#include "scarp/log.hh"
#include "scarp/dwt/dwt_program.hh"

using namespace scarp;

int main(int argc, char **argv)
{
  log_init();

  po::options_description desc("Allowed options");

  std::string input_name = "";
  std::string output_name = "";

  double scale_factor = 1.;

  desc.add_options()
    ("help", "produce help message")
    ("scale", po::value<double>(&scale_factor)->default_value(1.), "scale factor")
    ("input", po::value<std::string>(&input_name)->required(), "input file")
    ("output", po::value<std::string>(&output_name), "output file");

  po::variables_map vm;

  po::positional_options_description positional_options;
  positional_options.add("input", 1);
  positional_options.add("output", 1);

  po::store(po::command_line_parser(argc, argv)
            .options(desc)
            .positional(positional_options)
            .run(),
            vm);

  if(vm.count("help"))
  {
    std::cerr << "Usage: "
              << argv[0]
              << " [options] <input> <ouput>"
              << std::endl;

    std::cerr << desc << std::endl;

    return 1;
  }

  po::notify(vm);

  Log(info) << "Reading in controls from file \"" << input_name << "\"";

  std::ifstream input(input_name);

  Instance instance = InstanceReader().read_uniform(input);

  const idx dimension = instance.dimension();

  const double max_deviation = bounds::sur_simple(dimension).for_mesh(instance.get_mesh(),
                                                                      scale_factor);

  const auto& controls = instance.get_fractional_controls();

  std::vector<double> switch_on_costs = default_switch_on_costs(dimension);
  std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

  std::vector<idx> minimum_dwt{2, 2, 0};

  SwitchCosts switch_costs(instance,
                           switch_on_costs,
                           switch_off_costs);

  DWTProgram program(controls,
                     switch_costs,
                     minimum_dwt,
                     max_deviation);

  auto min_controls = program.solve();

  if(output_name != "")
  {
    std::ofstream output(output_name);

    ControlWriter().write(output, min_controls);
  }

  return 0;
}
