#include <sstream>
#include <iostream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "control_reader.hh"
#include "control_writer.hh"

#include "log.hh"
#include "scarp/scarp_program.hh"

int main(int argc, char **argv)
{
  log_init();

  po::options_description desc("Allowed options");

  std::string original_name;
  std::vector<std::string> input_names;

  desc.add_options()
    ("original", po::value<std::string>(&original_name)->required(), "original file")
    ("input", po::value<std::vector<std::string>>(&input_names)->required(), "input files");

  po::variables_map vm;

  po::positional_options_description positional_options;
  positional_options.add("original", 1);
  positional_options.add("input", -1);

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

  std::ifstream input(original_name);

  auto original_controls = ControlReader().read(input);

  const idx dimension = original_controls.size();

  const idx size = original_controls.begin()->size();

  std::vector<double> switch_on_costs(dimension, 1.);
  std::vector<double> switch_off_costs(dimension, 0.);

  SwitchCosts switch_costs(switch_on_costs, switch_off_costs);
  SURCosts sur_costs;

  for(const auto& input_name : input_names)
  {
    std::ifstream input(input_name);

    auto controls = ControlReader().read(input);

    assert(dimension == controls.size());
    assert(size == controls.begin()->size());

    const double switch_cost_value = switch_costs.total_cost(controls,
                                                             original_controls);

    const double sur_cost_value = sur_costs.total_cost(controls, original_controls);

    Log(info) << "Controls: "
              << input_name
              << ", switch costs: "
              << switch_cost_value
              << ", SUR costs: "
              << sur_cost_value;

  }


}
