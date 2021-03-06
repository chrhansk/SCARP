
#include <iostream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "control_reader.hh"
#include "log.hh"

int main(int argc, char *argv[])
{
  log_init();

  po::options_description desc("Allowed options");

  std::string fractional_name = "";
  std::string integral_name = "";

  double scale_factor = 1.;

  desc.add_options()
    ("help", "produce help message")
    ("scale", po::value<double>(&scale_factor)->default_value(1.), "scale factor")
    ("fractional", po::value<std::string>(&fractional_name)->required(), "fractional file")
    ("integral", po::value<std::string>(&integral_name), "integral file");

  po::variables_map vm;

  po::positional_options_description positional_options;
  positional_options.add("fractional", 1);
  positional_options.add("integral", 1);

  po::store(po::command_line_parser(argc, argv)
            .options(desc)
            .positional(positional_options)
            .run(),
            vm);

  if(vm.count("help"))
  {
    Log(error) << "Usage: "
               << argv[0]
               << " [options] <fractional> <integrals>";

    Log(error) << desc;

    return 1;
  }

  po::notify(vm);

  std::ifstream fractional_input(fractional_name);

  auto fractional_controls = ControlReader().read(fractional_input);

  if(!controls_are_convex(fractional_controls))
  {
    Log(error) << "Fractional controls are not convex";
  }

  std::ifstream integral_input(integral_name);

  auto integral_controls = ControlReader().read(integral_input);

  if(!controls_are_convex(integral_controls))
  {
    Log(error) << "Integral controls are not convex";
  }

  if(!controls_are_integral(integral_controls))
  {
    Log(error) << "Integral controls are not integral";
  }

  const idx fractional_size = fractional_controls.front().size();
  const idx fractional_dimension = fractional_controls.size();

  const idx integral_size = integral_controls.front().size();
  const idx integral_dimension = integral_controls.size();

  if(integral_size != fractional_size)
  {
    Log(error) << "Size mismatch";
  }

  if(integral_dimension != fractional_dimension)
  {
    Log(error) << "Dimension mismatch";
  }

  double deviation = max_control_deviation(fractional_dimension);

  deviation *= scale_factor;

  const double distance = control_distance(fractional_controls,
                                           integral_controls);

  if(!cmp::le(distance, deviation))
  {
    Log(error) << "Control deviation exceeded";
  }

  return 0;
}
