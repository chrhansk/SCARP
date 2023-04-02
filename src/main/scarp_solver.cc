#include <sstream>
#include <iostream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "instance_reader.hh"
#include "control_writer.hh"

#include "log.hh"
#include "scarp/scarp_program.hh"

int main(int argc, char **argv)
{
  log_init();

  po::options_description desc("Allowed options");

  std::string input_name = "";
  std::string output_name = "";

  double scale_factor = 1.;

  bool vanishing_constraints = false;
  bool sur_costs = false;
  bool collect_all = false;
  bool write_ampl = false;

  desc.add_options()
    ("help", "produce help message")
    ("vanishing_constraints", po::bool_switch(&vanishing_constraints)->default_value(false), "enable vanishing constraints")
    ("sur_costs", po::bool_switch(&sur_costs)->default_value(false), "use SUR costs")
    ("collect", po::bool_switch(&collect_all)->default_value(false), "collect all optimal solutions")
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
  const FractionalControls& controls = instance.get_fractional_controls();

  const idx dimension = controls.dimension();

  std::vector<double> switch_on_costs = default_switch_on_costs(dimension);
  std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

  std::vector<BinaryControls> all_controls;

  if(sur_costs)
  {
    SURCosts sur_costs(instance);

    SCARPProgram program(instance,
                         sur_costs,
                         scale_factor,
                         vanishing_constraints);

    if(collect_all)
    {
      all_controls = program.solve_all();
    }
    else
    {
      all_controls.push_back(program.solve());
    }

  }
  else
  {
    SwitchCosts switch_costs(instance,
                             switch_on_costs,
                             switch_off_costs);

    SCARPProgram program(instance,
                         switch_costs,
                         scale_factor,
                         vanishing_constraints);

    if(collect_all)
    {
      all_controls = program.solve_all();
    }
    else
    {
      all_controls.push_back(program.solve());
    }
  }

  if(output_name != "")
  {
    if(collect_all)
    {
      int i = 1;
      for(const auto& controls : all_controls)
      {
        std::stringstream current_name;

        current_name << output_name << "_" << i;

        std::string current_output_name = current_name.str();

        std::ofstream output(current_output_name);

        Log(info) << "Writing optimal solution #" << i
                  << " to file \""
                  << current_output_name
                  << "\"";

        if(write_ampl)
        {
          ControlWriter().writeAMPL(output, controls);
        }
        else
        {
          ControlWriter().write(output, controls);
        }

        ++i;
      }
    }
    else
    {
      Log(info) << "Writing controls to file \"" << output_name << "\"";

      std::ofstream output(output_name);

      if(write_ampl)
      {
        ControlWriter().writeAMPL(output, all_controls.front());
      }
      else
      {
        ControlWriter().write(output, all_controls.front());
      }
    }
  }

  return 0;
}
