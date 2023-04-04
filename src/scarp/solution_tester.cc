
#include <iostream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "scarp/instance_reader.hh"
#include "scarp/log.hh"

using namespace scarp;

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

  auto instance = InstanceReader().read_uniform(fractional_input);
  const FractionalControls& fractional_controls = instance.get_fractional_controls();
  const Mesh& mesh = instance.get_mesh();

  if(!fractional_controls.are_convex())
  {
    Log(error) << "Fractional controls are not convex";
  }

  std::ifstream integral_input(integral_name);

  auto integral_instance = InstanceReader().read_uniform(integral_input);
  const FractionalControls& integral_controls = integral_instance.get_fractional_controls();

  if(!(integral_controls.are_convex()))
  {
    Log(error) << "Integral controls are not convex";
  }

  if(!integral_controls.are_integral())
  {
    Log(error) << "Integral controls are not integral";
  }

  const idx fractional_size = fractional_controls.num_cells();
  const idx fractional_dimension = fractional_controls.dimension();

  const idx integral_size = integral_controls.num_cells();
  const idx integral_dimension = integral_controls.dimension();

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

  const double distance = fractional_controls.distance(integral_controls,
                                                       mesh);

  if(!cmp::le(distance, deviation))
  {
    Log(error) << "Control deviation exceeded";
  }

  return 0;
}
