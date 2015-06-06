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

#ifndef GHTV_OPENGL_TEXTURE_HPP
#define GHTV_OPENGL_TEXTURE_HPP

#include <boost/shared_ptr.hpp>
#include <GL/gl.h>

#include <cassert>
#include <iostream>

namespace ghtv { namespace opengl {

struct texture
{
  struct release_texture
  {
    void operator()(GLuint* t)
    {
      glDeleteTextures(1, t);
      delete t;
    }
  };

  texture ()
    : texture_id(new GLuint(0)
                 , release_texture())
  {
    glGenTextures(1, texture_id.get());
    assert(glGetError() == GL_NO_ERROR);
  }

  void bind()
  {
    glBindTexture(GL_TEXTURE_2D, *texture_id);
    assert(glGetError() == GL_NO_ERROR);
  }

  GLuint raw() const
  {
    return *texture_id;
  }

  boost::shared_ptr<GLuint> texture_id;
};

} }

#endif
