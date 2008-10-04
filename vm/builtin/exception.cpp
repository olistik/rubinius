#include "builtin/class.hpp"
#include "builtin/exception.hpp"
#include "builtin/lookuptable.hpp"
#include "builtin/task.hpp"
#include "builtin/contexts.hpp"
#include "builtin/fixnum.hpp"
#include "builtin/symbol.hpp"
#include "builtin/string.hpp"

#include "vm.hpp"
#include "exception.hpp"
#include "type_info.hpp"

#include <sstream>

namespace rubinius {
  Exception* Exception::create(STATE) {
    return (Exception*)state->new_object(G(exception));
  }

  Exception* Exception::make_exception(STATE, Class* exc_class, const char* message) {
    Exception* exc = (Exception*)state->new_object(exc_class);

    MethodContext* ctx = G(current_task)->active();
    ctx->reference(state);

    exc->context(state, ctx);
    exc->message(state, String::create(state, message));

    return exc;
  }

  void Exception::argument_error(STATE, int expected, int given) {
    std::ostringstream msg;

    msg << "given " << given << ", expected " << expected;

    argument_error(state, msg.str().c_str());
  }

  void Exception::argument_error(STATE, const char* reason) {
    RubyException::raise(make_exception(state, get_argument_error(state), reason));
  }

  Exception* Exception::make_type_error(STATE, object_type type, OBJECT object,
                                   const char* reason) {
    std::ostringstream msg;

    TypeInfo* wanted = state->find_type(type);

    if(!object->reference_p()) {
      msg << "  Tried to use non-reference value " << object;
    } else {
      TypeInfo* was = state->find_type(object->obj_type);
      msg << "  Tried to use object of type " <<
        was->type_name << " (" << was->type << ")";
    }

    msg << " as type " << wanted->type_name << " (" << wanted->type << ")";

    return make_exception(state, get_type_error(state), msg.str().c_str());
  }

  void Exception::type_error(STATE, const char* reason) {
    RubyException::raise(make_exception(state, get_type_error(state), reason));
  }

  void Exception::type_error(STATE, object_type type, OBJECT object,
                                   const char* reason) {
    RubyException::raise(make_type_error(state, type, object, reason));
  }

  void Exception::float_domain_error(STATE, const char* reason) {
    RubyException::raise(make_exception(state, get_float_domain_error(state), reason));
  }

  void Exception::zero_division_error(STATE, const char* reason) {
    RubyException::raise(make_exception(state, get_zero_division_error(state), reason));
  }

  void Exception::assertion_error(STATE, const char* reason) {
    RubyException::raise(make_exception(state, get_assertion_error(state), reason));
  }

  void Exception::object_bounds_exceeded_error(STATE, OBJECT obj, size_t index) {
    TypeInfo* info = state->find_type(obj->obj_type); // HACK use object
    std::ostringstream msg;

    msg << "Bounds of object exceeded:" << std::endl;
    msg << "      type: " << info->type_name << ", fields: " <<
           obj->num_fields() << ", accessed: " << index << std::endl;

    RubyException::raise(make_exception(state, get_object_bounds_exceeded_error(state),
                                        msg.str().c_str()));
  }

  void Exception::errno_error(STATE, const char* reason, int ern) {
    if(ern == 0) ern = errno;
    Class* exc_class = get_errno_error(state, ern);

    if(!reason) reason = strerror(ern);

    if(exc_class->nil_p()) {
      std::ostringstream msg;
      msg << "Unknown errno " << ern;
      RubyException::raise(make_exception(state,
            get_argument_error(state), msg.str().c_str()));
    }

    RubyException::raise(make_exception(state, exc_class, reason));
  }

  bool Exception::argument_error_p(STATE, Exception* exc) {
    return exc->kind_of_p(state, get_argument_error(state));
  }

  bool Exception::type_error_p(STATE, Exception* exc) {
    return exc->kind_of_p(state, get_type_error(state));
  }

  bool Exception::zero_division_error_p(STATE, Exception* exc) {
    return exc->kind_of_p(state, get_zero_division_error(state));
  }

  bool Exception::float_domain_error_p(STATE, Exception* exc) {
    return exc->kind_of_p(state, get_float_domain_error(state));
  }

  bool Exception::assertion_error_p(STATE, Exception* exc) {
    return exc->kind_of_p(state, get_assertion_error(state));
  }

  bool Exception::object_bounds_exceeded_error_p(STATE, Exception* exc) {
    return exc->kind_of_p(state, get_object_bounds_exceeded_error(state));
  }

  bool Exception::errno_error_p(STATE, Exception* exc) {
    if(Class* cls = try_as<Class>(G(object)->get_const(state, "SystemCallError"))) {
      return exc->kind_of_p(state, cls);
    }

    return false;
  }

  Class* Exception::get_argument_error(STATE) {
    return G(exc_arg);
  }

  Class* Exception::get_type_error(STATE) {
    return G(exc_type);
  }

  Class* Exception::get_zero_division_error(STATE) {
    return as<Class>(G(object)->get_const(state, "ZeroDivisionError"));
  }

  Class* Exception::get_float_domain_error(STATE) {
    return as<Class>(G(object)->get_const(state, "FloatDomainError"));
  }

  Class* Exception::get_assertion_error(STATE) {
    return as<Class>(G(rubinius)->get_const(state, "AssertionError"));
  }

  Class* Exception::get_object_bounds_exceeded_error(STATE) {
    return as<Class>(G(rubinius)->get_const(state, "ObjectBoundsExceededError"));
  }

  Class* Exception::get_errno_error(STATE, int ern) {
    if(Class* cls = try_as<Class>(G(errno_mapping)->fetch(
            state, Fixnum::from(ern)))) {
      return cls;
    }

    return (Class*)Qnil;
  }
}
