#include "benchmark.hh"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include "scarp/control_writer.hh"
#include "scarp/instance_reader.hh"
#include "scarp/log.hh"
#include "scarp/parameters.hh"

#include "timer.hh"

void Benchmark::execute_all(int argc, char** argv)
{
  log_init(true);

  po::options_description desc("Allowed options");

  std::string input_name = "";
  std::string output_name = "";

  double scale_factor = 1.;

  desc.add_options()
    ("help", "produce help message")
    ("scale", po::value<double>(&scale_factor)->default_value(1.), "scale factor")
    ("input", po::value<std::string>(&input_name)->required(), "input directory")
    ("output", po::value<std::string>(&output_name)->required(), "output directory");

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
              << " [options] <input> <output>"
              << std::endl;

    std::cerr << desc << std::endl;

    return;
  }

  po::notify(vm);

  fs::path instance_path(input_name);

  fs::path output_path(output_name);

  std::vector<fs::path> instances;

  for(const auto& instance : fs::directory_iterator(instance_path))
  {
    if(!fs::is_regular_file(instance))
    {
      continue;
    }

    instances.push_back(instance.path());
  }

  std::sort(std::begin(instances), std::end(instances));

  std::cout << "Instance,Time" << std::endl;

  for(const auto& current_path : instances)
  {
    fs::ifstream instance_input{current_path};

    auto instance = InstanceReader().read(instance_input, false);

    const idx dimension = instance.dimension();

    std::vector<double> switch_on_costs = default_switch_on_costs(dimension);
    std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

    SwitchCosts switch_costs(instance,
                             switch_on_costs,
                             switch_off_costs);

    const int num_runs = 100;

    double elapsed = 0.;

    for(int i = 0; i < num_runs; ++i)
    {
      Timer timer;

      execute(instance, switch_costs);

      elapsed += timer.elapsed();
    }

    elapsed /= (double) num_runs;

    std::cout << current_path.filename()
              << ","
              << elapsed
              << std::endl;

    auto current_output_path = output_path / current_path.filename().replace_extension("dat");

    fs::ofstream output{current_output_path};

    auto min_controls = execute(instance, switch_costs);

    ControlWriter().write(output, min_controls);
  }

}
