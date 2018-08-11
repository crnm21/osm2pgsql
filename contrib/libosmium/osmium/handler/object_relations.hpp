#ifndef OSMIUM_HANDLER_OBJECT_RELATIONS_HPP
#define OSMIUM_HANDLER_OBJECT_RELATIONS_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2018 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <osmium/handler.hpp>
#include <osmium/index/multimap.hpp>
#include <osmium/osm/item_type.hpp>
#include <osmium/osm/node_ref.hpp>
#include <osmium/osm/relation.hpp>
#include <osmium/osm/types.hpp>
#include <osmium/osm/way.hpp>

namespace osmium {

    namespace handler {

        /**
         * This handler updates the indexes given to the constructor with
         * the relations between objects.
         *
         * Note: This handler will only work if either all object IDs are
         *       positive or all object IDs are negative.
         */
        class ObjectRelations : public osmium::handler::Handler {

            using index_type = osmium::index::multimap::Multimap<unsigned_object_id_type, unsigned_object_id_type>;

            index_type& m_index_n2w;
            index_type& m_index_n2r;
            index_type& m_index_w2r;
            index_type& m_index_r2r;

        public:

            explicit ObjectRelations(index_type& n2w, index_type& n2r, index_type& w2r, index_type& r2r) :
                m_index_n2w(n2w),
                m_index_n2r(n2r),
                m_index_w2r(w2r),
                m_index_r2r(r2r) {
            }

            void way(const osmium::Way& way) {
                for (const auto& node_ref : way.nodes()) {
                    m_index_n2w.set(node_ref.positive_ref(), way.positive_id());
                }
            }

            void relation(const osmium::Relation& relation) {
                for (const auto& member : relation.members()) {
                    switch (member.type()) {
                        case osmium::item_type::node:
                            m_index_n2r.set(member.positive_ref(), relation.positive_id());
                            break;
                        case osmium::item_type::way:
                            m_index_w2r.set(member.positive_ref(), relation.positive_id());
                            break;
                        case osmium::item_type::relation:
                            m_index_r2r.set(member.positive_ref(), relation.positive_id());
                            break;
                        default:
                            break;
                    }
                }
            }

        }; // class ObjectRelations

    } // namespace handler

} // namespace osmium

#endif // OSMIUM_HANDLER_OBJECT_RELATIONS_HPP
