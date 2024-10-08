// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef USER_ROLES_ODB_HXX
#define USER_ROLES_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "user_roles.hxx"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // user_roles
  //
  template <>
  struct class_traits< ::user_roles >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::user_roles >
  {
    public:
    typedef ::user_roles object_type;
    typedef ::user_roles* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef long unsigned int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/pgsql/version.hxx>
#include <odb/pgsql/forward.hxx>
#include <odb/pgsql/binding.hxx>
#include <odb/pgsql/pgsql-types.hxx>
#include <odb/pgsql/query.hxx>

namespace odb
{
  // user_roles
  //
  template <typename A>
  struct query_columns< ::user_roles, id_pgsql, A >
  {
    // id
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    id_type_;

    static const id_type_ id;

    // user_id
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    user_id_type_;

    static const user_id_type_ user_id;

    // role_id
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    role_id_type_;

    static const role_id_type_ role_id;

    // is_active
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        bool,
        pgsql::id_boolean >::query_type,
      pgsql::id_boolean >
    is_active_type_;

    static const is_active_type_ is_active;

    // created_at
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::boost::posix_time::ptime,
        pgsql::id_timestamp >::query_type,
      pgsql::id_timestamp >
    created_at_type_;

    static const created_at_type_ created_at;

    // updated_at
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::boost::posix_time::ptime,
        pgsql::id_timestamp >::query_type,
      pgsql::id_timestamp >
    updated_at_type_;

    static const updated_at_type_ updated_at;
  };

  template <typename A>
  const typename query_columns< ::user_roles, id_pgsql, A >::id_type_
  query_columns< ::user_roles, id_pgsql, A >::
  id (A::table_name, "\"id\"", 0);

  template <typename A>
  const typename query_columns< ::user_roles, id_pgsql, A >::user_id_type_
  query_columns< ::user_roles, id_pgsql, A >::
  user_id (A::table_name, "\"user_id\"", 0);

  template <typename A>
  const typename query_columns< ::user_roles, id_pgsql, A >::role_id_type_
  query_columns< ::user_roles, id_pgsql, A >::
  role_id (A::table_name, "\"role_id\"", 0);

  template <typename A>
  const typename query_columns< ::user_roles, id_pgsql, A >::is_active_type_
  query_columns< ::user_roles, id_pgsql, A >::
  is_active (A::table_name, "\"is_active\"", 0);

  template <typename A>
  const typename query_columns< ::user_roles, id_pgsql, A >::created_at_type_
  query_columns< ::user_roles, id_pgsql, A >::
  created_at (A::table_name, "\"created_at\"", 0);

  template <typename A>
  const typename query_columns< ::user_roles, id_pgsql, A >::updated_at_type_
  query_columns< ::user_roles, id_pgsql, A >::
  updated_at (A::table_name, "\"updated_at\"", 0);

  template <typename A>
  struct pointer_query_columns< ::user_roles, id_pgsql, A >:
    query_columns< ::user_roles, id_pgsql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::user_roles, id_pgsql >:
    public access::object_traits< ::user_roles >
  {
    public:
    struct id_image_type
    {
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // id_
      //
      long long id_value;
      bool id_null;

      // user_id_
      //
      int user_id_value;
      bool user_id_null;

      // role_id_
      //
      int role_id_value;
      bool role_id_null;

      // is_active_
      //
      bool is_active_value;
      bool is_active_null;

      // created_at_
      //
      long long created_at_value;
      bool created_at_null;

      // updated_at_
      //
      long long updated_at_value;
      bool updated_at_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const id_image_type&);

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (pgsql::bind*,
          image_type&,
          pgsql::statement_kind);

    static void
    bind (pgsql::bind*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          pgsql::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef pgsql::object_statements<object_type> statements_type;

    typedef pgsql::query_base query_base_type;

    static const std::size_t column_count = 6UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    static const char persist_statement_name[];
    static const char find_statement_name[];
    static const char update_statement_name[];
    static const char erase_statement_name[];
    static const char query_statement_name[];
    static const char erase_query_statement_name[];

    static const unsigned int persist_statement_types[];
    static const unsigned int find_statement_types[];
    static const unsigned int update_statement_types[];

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::user_roles, id_common >:
    public access::object_traits_impl< ::user_roles, id_pgsql >
  {
  };

  // user_roles
  //
}

#include "user_roles-odb.ixx"

#include <odb/post.hxx>

#endif // USER_ROLES_ODB_HXX
