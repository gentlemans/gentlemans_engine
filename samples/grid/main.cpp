#include <ge/input_subsystem.hpp>
#include <ge/mesh_actor.hpp>
#include <ge/rocket_document_asset.hpp>
#include <ge/rocket_input_consumer.hpp>
#include <ge/rocket_subsystem.hpp>
#include <ge/runtime.hpp>
#include <ge/sdl_subsystem.hpp>
#include <ge/timer_subsystem.hpp>
#include <ge/renderable.hpp>

#include <glm/glm.hpp>

#include <cmath>
#include <memory>
#include "grid.hpp"
#include "gridtick_interface.hpp"
#include "piece.hpp"
#include "turret.hpp"
#include "zombie.hpp"
#include "zombiespawner.hpp"
#include "grid_rocket_element.hpp"
#include "spike.hpp"

#include <Rocket/Debugger/Debugger.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

using namespace ge;

struct hud : actor {
  
  grid* g;
  Rocket::Core::ElementDocument* rdoc;
  Rocket::Core::ElementText* text;
  
  void initialize(grid* gr, Rocket::Core::ElementDocument* doc){
    g = gr;
    rdoc = doc;
    add_interface<hud, gridtick_interface>();
    
    auto zcount = doc->GetElementById("zcount");
    text = rdoc->CreateTextNode("0");
    zcount->AppendChild(text);
  }
  
  void tick_grid() {
    int zcount = g->get_z_count();
    
    text->SetText(std::to_string(zcount).c_str());
  }
  
};

int main()
{
	runtime r;
	
	r.m_asset_manager.add_asset_path("data/");
	r.add_subsystem<input_subsystem>({});
	r.add_subsystem<timer_subsystem>({});
	auto& sdl = r.add_subsystem<sdl_subsystem>(sdl_subsystem::config{"Example!", {1024, 720}});
	auto& rocket = r.add_subsystem<rocket_subsystem>({});

	r.register_interface<renderable>();
	r.register_interface<gridtick_interface>();

	// load UI
	auto doc = r.m_asset_manager.get_asset<rocket_document_asset>("gridui/doc.rocketdocument");
	doc->Show();

	auto root = actor::root_factory(&r);

    auto camera = actor::factory<camera_actor>(root.get(), 13.f, float(sdl.get_size().x) / float(sdl.get_size().y));

	sdl.set_background_color({.2f, .2f, .2f});
	sdl.set_camera(camera.get());
	r.set_root_actor(root.get());

	// initialize the grid
    auto g = actor::factory<grid>(root.get(), glm::ivec2{11, 11}, 20.f);
	actor::factory<spike>(g.get(), glm::ivec3(4, 4, 1));
	actor::factory<spike>(g.get(), glm::ivec3(4, 6, 1));
	actor::factory<spike>(g.get(), glm::ivec3(6, 4, 1));
	actor::factory<spike>(g.get(), glm::ivec3(6, 6, 1));

	actor::factory<turret>(g.get(), glm::ivec3(4, 5, 2), piece::WEST);
	actor::factory<turret>(g.get(), glm::ivec3(5, 6, 2), piece::NORTH);
	actor::factory<turret>(g.get(), glm::ivec3(6, 5, 2), piece::EAST);
	actor::factory<turret>(g.get(), glm::ivec3(5, 4, 2), piece::SOUTH);
	
	for (int x = 0; x < 12; x++) {
		actor::factory<zombiespawner>(g.get(), glm::ivec3(-1, x, 2));
		actor::factory<zombiespawner>(g.get(), glm::ivec3(11, x-1, 2));
		actor::factory<zombiespawner>(g.get(), glm::ivec3(x-1, -1, 2));
		actor::factory<zombiespawner>(g.get(), glm::ivec3(x, 11, 2));
	}
	g->try_spawn_z();



	grid_rocket_instancer::registerInstancer();

    // create virtual rocket elements
    auto vp = camera->get_vp_matrix();
    auto tmpActor = actor::factory<piece>(g.get(), glm::ivec3(0, 0, 0));
    for(int x = 0; x < 11; ++x) {
        for(int y = 0; y < 11; ++y) {

            auto start = vp * tmpActor->calculate_model_matrix() * glm::vec3(x - .5, y - .5, 1);
            auto end = vp * tmpActor->calculate_model_matrix() * glm::vec3(x + .5, y + .5, 1);

			start += 1;
			start.x *= (float)sdl.get_size().x / 2.f;
			start.y *= (float)sdl.get_size().y / 2.f;

			end += 1;
			end.x *= (float)sdl.get_size().x / 2.f;
			end.y *= (float)sdl.get_size().y / 2.f;

            auto xml = Rocket::Core::XMLAttributes();
            xml.Set("idx", x);
            xml.Set("idy", y);
            xml.Set("start", Rocket::Core::Vector2f{start.x, start.y});
            xml.Set("size", Rocket::Core::Vector2f{end.x - start.x, end.y - start.y});

            auto elem = Rocket::Core::Factory::InstanceElement(nullptr, "grid_rocket", "grid_rocket", xml);
            auto text = doc->CreateTextNode("1");
            //elem->AppendChild(text);
            doc->AppendChild(elem);

			auto str = "grid_" + std::to_string(x) + "_" + std::to_string(y);
			elem->SetId(str.c_str());
        }
    }

    /*Rocket::Debugger::Initialise(rocket.m_context);
    Rocket::Debugger::SetVisible(true);
*/
	auto elem = rocket.m_context->GetElementAtPoint({500, 500}, nullptr, doc.get());
	//std::cout << "CHosen: " << elem->GetId().CString();

	rocket_input_consumer ic{&r};
	ic.steal_input();

    actor::factory<hud>(root.get(), g.get(), doc.get());
    
#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg(
		[](void* run_ptr) {
			runtime* runt = (runtime*)run_ptr;

			runt->tick();
		},
		&r, 0, true);
#else
	while (r.tick())
		;
#endif
	
	int& a = *(int*)0;
}
