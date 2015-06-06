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

#ifndef GHTV_OPENGL_MAIN_STATE_HPP
#define GHTV_OPENGL_MAIN_STATE_HPP

#include <ghtv/opengl/presentation.hpp>
#include <ghtv/opengl/executor.hpp>

#include <gntl/structure/composed/document.hpp>
#include <gntl/structure/composed/descriptor.hpp>
#include <gntl/concept/parser/document/document.hpp>
#include <gntl/concept/parser/descriptor/descriptor_base.hpp>

namespace ghtv { namespace opengl {

template <typename ParserDocument, typename ExecutorFactory>
struct main_state
{
  boost::shared_ptr<ParserDocument> parser_document;
  typedef gntl::concept::parser::document_traits<ParserDocument> parser_document_traits;
  typedef typename parser_document_traits::head_type head_type;
  typedef gntl::concept::parser::head_traits<head_type> head_traits;
  typedef typename head_traits::descriptor_base_type descriptor_base;
  typedef gntl::concept::parser::descriptor_base_traits<descriptor_base> descriptor_base_traits;
  typedef typename descriptor_base_traits::descriptor_range descriptor_range;
  typedef typename boost::range_value<descriptor_range>::type parser_descriptor_type;

  typedef std::string document_uri_type;
  typedef gntl::structure::composed::descriptor<parser_descriptor_type, document_uri_type>
    descriptor_type;
  typedef presentation_factory<descriptor_type, ExecutorFactory> presentation_factory_type;
  typedef gntl::structure::composed::document
  <ParserDocument, presentation_factory_type, document_uri_type> document_type;
  boost::shared_ptr<document_type> document;

  std::list<active_player> active_players;
};

} }

#endif
