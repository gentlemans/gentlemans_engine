#ifndef GE_SDL_SUBSYSTEM_HPP
#define GE_SDL_SUBSYSTEM_HPP

#pragma once

#include "ge/camera_actor.hpp"
#include "ge/input_event.hpp"
#include "ge/subsystem.hpp"

#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <memory>
#include <vector>

struct SDL_Window;

namespace ge
{
/// A subsystem that uses SDL to open a window and draw in it
struct sdl_subsystem : subsystem {
	/// The subsystem config
	struct config {
		/// The title of the window
		std::string title;

		/// The location of the window on the screen, or {} to let the OS decide
		boost::optional<glm::uvec2> location;

		///  The size of the window
		glm::uvec2 size;

		/// Should the window be fullscreen?
		bool fullscreen = false;

		/// Should it have decorations (minimize button, borders, etc)
		bool decorated = true;
	};

	/// The subsystem initialize function
	bool initialize(const config& conf);

	/// The update function from `subsystem`
	virtual bool update(std::chrono::duration<float> delta) override;

	/// The shutdown function from `subsystem`
	virtual bool shutdown() override;

	/// Gets the size of the window
	/// \return The size of the window
	glm::uvec2 get_size() const;

	/// Set the size of the window
	/// \param new_size Set the size of the window
	void set_size(glm::uvec2 new_size);

	/// Gets the title of the window
	/// \return The title
	std::string get_title() const;

	/// Sets the title of the window
	/// \param newTitle The new title
	void set_title(const std::string& newTitle);

	/// Sets the background color of the viewport where there are no polygons
	/// \param newColor The new color
	void set_background_color(const glm::vec3& newColor) noexcept { m_background_color = newColor; }
	/// Gets the background color of the viewport
	/// \return The background color
	glm::vec3 get_background_color() const noexcept { return m_background_color; }
	/// Get the camera actor to view from
	/// \return The camera
	camera_actor* get_camera() const noexcept { return m_camera.get(); }
	/// Set the camera actor
	/// \param cam The new camera to view from
	void set_camera(camera_actor* cam) noexcept { m_camera = actor::shared(cam); }
	/// Gets the root actor to draw all subactors from
	/// \return The root actor
	actor* get_root_actor() const noexcept { return m_root_actor.get(); }
	/// Set the root actor
	/// \param new_root The new root actor
	void set_root_actor(actor* new_root) noexcept { m_root_actor = actor::shared(new_root); }
private:
	void* m_context = nullptr;  // turns out SDL_GLContext is literally just void*
	SDL_Window* m_window = nullptr;
	glm::vec3 m_background_color;
	std::shared_ptr<camera_actor> m_camera;
	std::shared_ptr<actor> m_root_actor;
};

}  // namespace ge

#endif  // GE_SDL_SUBSYSTEM_HPP