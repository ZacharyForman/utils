#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace utils {

/// Returns a generic error message for the given type.
///   flag_name: The flag provided.
///   argument: The argument provided.
template<typename T>
std::string arg_error_text(std::string flag_name, std::string argument)
{
  return "error: `" + argument "' is a bad argument for " + flag_name + ".";
}

namespace {

/// Internal abstract base class for a command line argument.
class Arg {
public:
  /// Constructs an Arg.
  ///   flag: The command line flag given, e.g. -l or --height.
  ///   help_text: Text printed when the --help arg is received.
  ///   required: If the argument is required by the program.
  Arg(std::string flag, std::string help_text, bool required = false);

  /// Parses the value from arg. In case of error, terminates the program.
  /// Modifies this Arg's stored variable.
  /// Returns true if the argument pointer should be moved along.
  ///   arg: A null terminated string to extract the value from.
  virtual bool parse_value(const char *arg) = 0;

  /// Returns a string containing the current value of this argument.
  virtual std::string value() const = 0;

  /// The command line flag given, e.g. -l or --height.
  const std::string flag;
  /// Text printed when the --help arg is received.
  const std::string help_text;
  /// True iff the argument's value is valid.
  bool ok;
};

Arg(std::string flag, std::string help_text, bool required)
  : flag(flag), help_text(help_text), required(required) {}

/// Internal class that actually stores the value of an argument.
/// Boolean variables are always considered to be singular, i.e.
/// --flag will set a boolean variable to the opposite of its default.
template<typename T>
class TypedArg final : Arg {
public:
  /// Constructs a TypedArg.
  ///   flag: The command line flag given, e.g. -l or --height.
  ///   help_text: Text printed when the --help arg is received.
  ///   val: A reference to the variable the argument should be stored in.
  ///   required: If the argument is required by the program.
  TypedArg(std::string flag, std::string help_text,
           T &val, bool required = false);

  bool parse_value(const char *arg) override;
  std::string value() const override;
private:
  /// A reference to the variable the received argument should be stored in.
  T &var;
};

TypedArg(std::string flag, std::string help_text,
         T &val, bool required = false)
  : Arg(flag, help_text, required), val(val) {}

template<typename T>
bool TypedArg<T>::parse_value(const char *arg)
{
  std::stringstream ss(arg);
  if (!(ss >> val)) {
    std::cerr << arg_error_text<T>(flag, arg) << std::endl;
    exit(1);
  }
  ok = true;
  return true;
}

template<>
bool TypedArg<bool>::parse_value(const char *arg)
{
  var = !var;
  ok = true;
  return false;
}

template<typename T>
std::string TypedArg<T>::value()
{
  std::stringstream ss;
  ss << val;
  return ss.str();
}

} // namespace

/// Class that contains a list of arguments.
/// Add arguments to the list using ArgList::add_arg(), then parse
/// the argument list given to your program using Arglist::parse_args().
class ArgList {
public:
  /// Adds an argument to this ArgList.
  ///   val: The variable to store the argument in.
  ///   flag: The expected flag.
  ///   help_text: Information about the flag, printed when --help is received.
  ///   required: If the argument is required for success of the program.
  template<typename T>
  void add_arg(T& val, std::string flag, std::string help_text,
               bool required = false);

  /// Parses arguments from the argument list provided.
  /// Returns the last argument number considered.
  /// Stops parsing arguments on receipt of the argument --.
  ///   argc: The number of arguments, as per in main.
  ///   argv: The argument vector, where the zeroth argument is ignored.
  int parse_args(int argc, const char **argv);

  /// Prints associated help information to stdout.
  void help();
private:
  /// Contains the arguments and information about them.
  /// The key will always be the flag of the Arg referenced.
  std::map<const std::string&, std::unique_ptr<Arg>> arguments;
};

template<typename T>
void ArgList::add_arg(T& val, std::string flag, std::string help_text,
                      bool required = false)
{

}

int ArgList::parse_args(int argc, const char **argv)
{

}

void ArgList::help()
{

}

} // utils

#endif // ARGUMENTS_H
