#include <cstdlib>  // for std::exit
#include <cstring>  // for std::strcmp
#include <iostream> // for std::cout, std::cerr

// For the DynamicHandler class
#include <osmium/dynamic_handler.hpp>

// For the Dump handler
#include <osmium/handler/dump.hpp>

// For the NodeLocationForWays handler
#include <osmium/handler/node_locations_for_ways.hpp>

// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>

// For osmium::apply()
#include <osmium/visitor.hpp>

#include <gdalcpp.hpp>
#include <boost/program_options.hpp>

class NameHandler : public osmium::handler::Handler {
	public:
		void dumptags(const osmium::TagList& taglist, osmium::object_id_type id, const char *type) {
			if (!taglist.has_key("name"))
				return;

			std::vector<std::string> keys={ "name", "highway", "railway", "waterway", "building", "traffic_sign", "landuse",
				"amenity", "natural", "man_made", "place", "operator", "brand", "ref", "public_transport", "shop", "leisure", "ford",
				"power", "emergency", "barrier", "areoway" };
			std::cout << id << " type=\"" << type << "\"";

			for (auto key : keys) {
				if (!taglist.has_key(key.c_str()))
					continue;
				std::cout << " " << key << "=\"" <<  taglist.get_value_by_key(key.c_str()) << "\"";
			}

			std::cout << std::endl;

		}
		void way(osmium::Way& way) {
			dumptags(way.tags(), way.id(), "way");
		}
		void node(osmium::Node& node) {
			dumptags(node.tags(), node.id(), "node");
		}
};

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

	po::options_description         desc("Allowed options");
        desc.add_options()
                ("help,h", "produce help message")
                ("infile,i", po::value<std::string>(), "Input file")
        ;
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	osmium::io::File input_file{vm["infile"].as<std::string>()};

	NameHandler	handler;
	osmium::io::Reader reader{input_file};
	osmium::apply(reader, handler);
	reader.close();
}

