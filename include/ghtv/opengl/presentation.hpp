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

#ifndef GHTV_OPENGL_PRESENTATION_HPP
#define GHTV_OPENGL_PRESENTATION_HPP

#include <ghtv/opengl/executor.hpp>
#include <ghtv/opengl/active_player.hpp>

#include <gntl/structure/composed/presentation.hpp>

namespace ghtv { namespace opengl {

template <typename Descriptor, typename PlayerFactory>
struct presentation
{
  typedef gntl::structure::composed::presentation
  <executor<Descriptor, PlayerFactory>, Descriptor> type;
};

template <typename Descriptor, typename PlayerFactory>
struct presentation_factory
{
  presentation_factory(boost::filesystem::path root_path
                       , std::list<active_player>& active_players
                       , PlayerFactory player_factory = PlayerFactory())
    : player_factory(player_factory), root_path(root_path)
    , active_players(&active_players)
  {}

  typedef executor<Descriptor, PlayerFactory> executor_type;
  typedef typename presentation<Descriptor, PlayerFactory>::type result_type;
  result_type operator()(Descriptor descriptor) const
  {
    return result_type(executor_type(descriptor, player_factory
                                     , root_path, active_players)
                       , descriptor);
  }

  PlayerFactory player_factory;
  boost::filesystem::path root_path;
  std::list<active_player>* active_players;
};

} }

#endif
