/* (c) Copyright 2011-2014 Felipe Magno de Almeida
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GHTV_OPENGL_EXECUTOR_HPP
#define GHTV_OPENGL_EXECUTOR_HPP

#include <ghtv/opengl/active_player.hpp>

#include <gntl/concept/structure/executor.hpp>
#include <gntl/parser/libxml2/dom/color.hpp>
#include <gntl/parser/libxml2/dom/xml_string.hpp>
#include <gntl/algorithm/structure/media/dimensions.hpp>
#include <gntl/structure/composed/component_location.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem/path.hpp>

namespace ghtv { namespace opengl {

template <typename Descriptor, typename PlayerFactory>
struct executor
{
  typedef gntl::parser::libxml2::dom::xml_string<> component_identifier;
  executor(boost::optional<Descriptor> descriptor, PlayerFactory player_factory
           , boost::filesystem::path root_path
           , std::list<opengl::active_player>* active_players)
    : player_factory(player_factory), root_path(root_path)
    , active_players(active_players)
    , active_player(active_players->end())
  {
  }

  void key_process(std::string const& key, bool pressed)
  {
    if(player)
      player->key_process(key, pressed);
  }

  PlayerFactory player_factory;
  boost::shared_ptr<player_base> player;
  boost::filesystem::path root_path;
  std::list<opengl::active_player>* active_players;
  std::list<opengl::active_player>::iterator active_player;
};

} }

namespace gntl { namespace concept { namespace structure {

template <typename Descriptor, typename PlayerFactory>
struct executor_traits<ghtv::opengl::executor<Descriptor, PlayerFactory> >
{
  typedef boost::mpl::true_ is_executor;
  typedef ghtv::opengl::executor<Descriptor, PlayerFactory> executor;
  typedef boost::posix_time::time_duration time_duration;
  typedef gntl::parser::libxml2::dom::color color_type;
  typedef std::string component_identifier;

  typedef boost::mpl::vector
  <void(executor, component_identifier /*source*/
        , boost::optional<component_identifier> /*interface_*/
        , gntl::algorithm::structure::media::dimensions
        , gntl::structure::composed::component_location<std::string, std::string>)>
    start_function_overloads;
  typedef boost::mpl::vector
  <void(executor)> stop_function_overloads;
  typedef boost::mpl::vector
  <void(executor)> pause_function_overloads;
  typedef boost::mpl::vector
  <void(executor)> resume_function_overloads;
  typedef boost::mpl::vector
  <void(executor)> abort_function_overloads;

  static void area_time_begin(executor& e, component_identifier, time_duration)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void area_time_end(executor& e, component_identifier, time_duration)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void area_time_begin_end(executor& e, component_identifier, time_duration, time_duration);
  static void area_frame_begin(executor& e, component_identifier, unsigned int)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void area_frame_end(executor& e, component_identifier, unsigned int);
  static void area_frame_begin_end(executor& e, component_identifier, unsigned int, unsigned int);
  static void area_npt_begin(executor& e, component_identifier, int);
  static void area_npt_end(executor& e, component_identifier, int);
  static void area_npt_begin_end(executor& e, component_identifier, int, int);

  static void add_border (executor& e, int border_width, color_type color)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
    ghtv::opengl::color c = {color.r/255.0f, color.g/255.0f, color.b/255.0f};
    if(e.active_player == e.active_players->end())
    {
      boost::shared_ptr<ghtv::opengl::player_base> p;
      ghtv::opengl::active_player a = {p, 0u, 0u, 0u, 0u, 0u, border_width, c};
      e.active_player = e.active_players->insert(e.active_players->end(), a);
    }
    else
    {
      e.active_player->border_width = border_width;
      e.active_player->color = c;
    }
  }
  static void remove_border (executor& e)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
    if(e.player)
      e.active_player->border_width = 0;
  }

  static void start_area(executor& e, component_identifier)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void explicit_duration(executor& e, time_duration)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }

  static void start(executor& e, component_identifier source
                    , boost::optional<component_identifier> interface_
                    , gntl::algorithm::structure::media::dimensions dim
                    , gntl::structure::composed::component_location<std::string, std::string>)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
    std::cout << "Start " << source << " at " << dim << std::endl;
    if(!e.player)
    {
      e.player = e.player_factory(source, dim, e.root_path);
      if(e.active_player == e.active_players->end())
      {
        static ghtv::opengl::color red = {1.0f, 0.0f, 0.0f};

        ghtv::opengl::active_player a = {e.player, dim.x, dim.y, dim.width, dim.height
                                         , dim.zindex, 0u, red};
        e.active_player = e.active_players->insert(e.active_players->end(), a);
      }
      else
      {
        e.active_player->player = e.player;
        e.active_player->x = dim.x;
        e.active_player->y = dim.y;
        e.active_player->w = dim.width;
        e.active_player->h = dim.height;
        e.active_player->zindex = dim.zindex;
      }
    }
  }
  static void abort(executor& e)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void stop(executor& e)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
    if(e.player)
    {
      e.active_players->erase(e.active_player);
      e.player.reset();
      e.active_player = e.active_players->end();
    }
  }
  static void pause(executor& e)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void resume(executor& e)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }

  static void listen_time(executor& e, component_identifier m, component_identifier i
                          , component_identifier c, time_duration time, gntl::transition_type transition)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void listen_frame(executor& e, component_identifier m, component_identifier i
                           , component_identifier c, unsigned int frame, gntl::transition_type transition)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static void listen_npt(executor& e, component_identifier m, component_identifier i
                         , component_identifier c, int npt, gntl::transition_type transition)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
  }
  static bool start_set_property(executor& e, std::string name, std::string value)
  {
    assert(e.active_player != e.active_players->end());
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__
              << " property_name " << name << " property_value " << value << std::endl;
    return false;
  }
  static bool start_set_property(executor& e, std::string name, int value)
  {
    assert(e.active_player != e.active_players->end());
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__
              << " property_name " << name << " property_value " << value << std::endl;
    if(name == "left")
    {
      e.active_player->x = value;
      return true;
    }
    return false;
  }
  static bool commit_set_property(executor& e, std::string property_name)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << " property_name " << property_name << std::endl;
    ::ghtv::opengl::draw();
    return true;
  }
  static bool wants_keys(executor const& e)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;
    return false;
  }
};

} } }

#endif
