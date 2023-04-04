#include <iostream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "scarp/instance_reader.hh"
#include "scarp/control_writer.hh"

#include "scarp/bounds.hh"
#include "scarp/log.hh"
#include "scarp/scarp/scarp_program.hh"

#include "scarp/sur/sur.hh"

using namespace scarp;

int main(int argc, char **argv)
{
  log_init();

  po::options_description desc("Allowed options");

  std::string input_name = "";
  std::string output_name = "";

  double scale_factor = 1.;

  bool vanishing_constraints = false;
  bool write_ampl = false;

  desc.add_options()
    ("help", "produce help message")
    ("vanishing_constraints", po::bool_switch(&vanishing_constraints)->default_value(false), "enable vanishing constraints")
    ("ampl", po::bool_switch(&write_ampl)->default_value(false), "write output in AMPL format")
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

  auto instance = InstanceReader().read_uniform(input);
  const FractionalControls& fractional_controls = instance.get_fractional_controls();

  const idx dimension = fractional_controls.dimension();

  BinaryControls sur_controls = compute_sur_controls(instance,
                                                     vanishing_constraints);

  std::vector<double> switch_on_costs = default_switch_on_costs(dimension);
  std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

  {
    SURCosts sur_costs(instance);

    const double total_control_costs = sur_costs.total_cost(sur_controls, fractional_controls);

    SwitchCosts switch_costs(instance,
                             switch_on_costs,
                             switch_off_costs);

    const double total_switch_costs = switch_costs.total_cost(sur_controls, fractional_controls);

    const double max_deviation = bounds::sur(dimension, vanishing_constraints).for_mesh(instance.get_mesh(),
                                                                                        scale_factor);

    Log(info) << "SUR costs of rounded controls: " << total_control_costs
              << ", upper bound: " << max_deviation
              << ", switching cost: " << total_switch_costs;

  }

  if(output_name != "")
  {
    Log(info) << "Writing controls to file \"" << output_name << "\"";

    std::ofstream output(output_name);


    if(write_ampl)
    {
      ControlWriter().writeAMPL(output, sur_controls);
    }
    else
    {
      ControlWriter().write(output, sur_controls);
    }
  }

  return 0;
}
