#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>

/// Collection of classes and other nifty utilities to perform argument
/// parsing and simplify the process of handling command line arguments.

/// Example usage:
///   \code{.cpp}
///   int main(int argc, const char **argv)
///   {
///     ArgList args;
///     bool ARG(verbose, false, args, "-v",
///              "Determines if verbose output is shown.");
///     std::string ARG(filepath, "", args, "--path", "Path to search");
///     int remainder = args.parse_args(argc, argv);
///     std::string search_term = argv[remainder];
///     for (int i = remainder + 1; i < argc; i++) {
///       if (verbose) {
///         std::cout << "Searching " << argv[i] << " for "
///                   << search_term << "." << std::endl;
///       }
///       search(argv[i], search_term);
///     }
///   }
///   \endcode

/// Utility macro to declare a variable and add it to an ArgList.
/// Usage: \code{.cpp} int ARG(size, 5, args, "--size", "The size of an apple"); \endcode
///   \param var: The variable to declare.
///   \param value: The default value of the argument.
///   \param arglist: The ArgList to add to.
///   \param ...0: The expected flag.
///   \param ...1: Information about the flag, printed if help is requested.
///   \param ...[2]: If the argument is required for success of the program.
#define ARG(var, value, arglist, ...) \
  var = value; \
  arglist.add_arg(var, __VA_ARGS__)

/// Returns a generic error message for the given type.
///   \param flag_name: The flag provided.
///   \param argument: The argument provided.
template<typename T>
inline std::string arg_error_text(std::string flag_name, std::string argument)
{
  return "error: `" + argument + "' is a bad argument for " + flag_name + ".";
}

namespace internal {

/// Internal abstract base class for a command line argument.
class Arg {
public:
  /// Constructs an Arg.
  ///   \param flag: The command line flag given, e.g. -l or --height.
  ///   \param help_text: Text printed when the --help arg is received.
  ///   \param required: If the argument is required by the program.
  inline Arg(std::string flag, std::string help_text, bool required = false);

  /// Parses the value from args. Modifies this Arg's stored variable.
  /// Returns how many arguments were consumed, or -1 on error.
  ///   \param args: The arguments to consume.
  ///   \param arg: The starting argument to consider.
  virtual int parse_value(const char **args, int arg) = 0;

  /// Returns a string containing the current value of this argument.
  virtual std::string value() const = 0;

  virtual ~Arg() { }

  /// The command line flag given, e.g. -l or --height.
  const std::string flag;
  /// Text printed when the --help arg is received.
  const std::string help_text;
  /// True iff the argument's value is valid.
  bool ok;
};

inline Arg::Arg(std::string flag, std::string help_text, bool required)
  : flag(flag), help_text(help_text), ok(!required) {}

/// Internal class that actually stores the value of an argument.
/// Boolean variables are always considered to be singular, i.e.
/// --flag will set a boolean variable to the opposite of its default.
template<typename T>
class TypedArg final : public Arg {
public:
  /// Constructs a TypedArg.
  ///   \param var: A reference to the variable the argument should be stored in.
  ///   \param flag: The command line flag given, e.g. -l or --height.
  ///   \param help_text: Text printed when the --help arg is received.
  ///   \param required: If the argument is required by the program.
  TypedArg(T &var, std::string flag, std::string help_text,
           bool required = false);

  int parse_value(const char **args, int arg) override;
  std::string value() const override;
private:
  /// A reference to the variable the received argument should be stored in.
  T &var;
};

template<typename T>
inline TypedArg<T>::TypedArg(T &var, std::string flag, std::string help_text,
                      bool required)
  : Arg(flag, help_text, required), var(var) {}

template<typename T>
inline int TypedArg<T>::parse_value(const char **args, int arg)
{
  if (arg < 0) {
    return -1;
  }
  std::stringstream ss(args[arg]);
  if (!(ss >> var)) {
    std::cerr << arg_error_text<T>(flag, args[arg]) << std::endl;
    return -1;
  }
  ok = true;
  return 1;
}

template<>
inline int TypedArg<bool>::parse_value(const char **args, int arg)
{
  var = !var;
  ok = true;
  return 0;
}

template<typename T>
inline std::string TypedArg<T>::value() const
{
  std::stringstream ss;
  ss << var;
  return ss.str();
}

} /// internal

/// Class that contains a list of arguments.
/// Add arguments to the list using ArgList::add_arg(), then parse
/// the argument list given to your program using Arglist::parse_args().
class ArgList {
public:
  /// Constructs an ArgList with the given help flag.
  ///   \param help_flag: The help flag to use. Defaults to --help.
  ///   \param break_flag: The flag to stop parsing args. Defaults to --.
  inline ArgList(std::string help_flag = "--help", std::string break_flag = "--");

  /// Adds an argument to this ArgList.
  /// Returns true if another argument with the given flag is not registered.
  ///   \param val: The variable to store the argument in.
  ///   \param flag: The expected flag.
  ///   \param help_text: Information about the flag, printed if help is requested.
  ///   \param required: If the argument is required for success of the program.
  template<typename T>
  inline bool add_arg(T& val, std::string flag, std::string help_text,
               bool required = false);

  /// Parses arguments from the argument list provided.
  /// Returns the last argument number considered.
  /// Stops parsing arguments on receipt of the argument break_flag.
  ///   \param argc: The number of arguments, as per in main.
  ///   \param argv: The argument vector, where the zeroth argument is ignored.
  inline int parse_args(int argc, const char **argv);

  /// Prints associated help information to stderr.
  inline void help();

  static const bool required = true;
  static const bool optional = false;
private:
  /// Contains the arguments and information about them.
  /// The key will always be the flag of the Arg referenced.
  std::map<std::string, std::unique_ptr<internal::Arg>> arguments;
  /// Flag that must be given to receive help text, --help by default.
  const std::string help_flag;
  /// Flag that must be given to stop parsing args, -- by default.
  const std::string break_flag;
};

inline ArgList::ArgList(std::string help_flag, std::string break_flag)
    : help_flag(help_flag), break_flag(break_flag) {}

template<typename T>
inline bool ArgList::add_arg(T& val, std::string flag, std::string help_text,
                      bool required)
{
  if (arguments.find(flag) != arguments.end()) {
    return false;
  }
  auto arg
      = std::make_unique<internal::TypedArg<T>>(val, flag, help_text, required);
  arguments[flag] = std::move(arg);
  return true;
}

inline int ArgList::parse_args(int argc, const char **argv)
{
  int i;
  for (i = 1; i < argc; i++) {
    if (help_flag == argv[i]) {
      help();
      exit(0);
    }

    if (break_flag == argv[i]) {
      break;
    }

    if (arguments.find(argv[i]) == arguments.end()) {
      std::cerr << "Bad argument " << argv[i] << std::endl;
      std::cerr << argv[0] << " --help for assistance." << std::endl;
      exit(1);
    }

    int res = arguments[argv[i]]->parse_value(argv, (i+1>=argc ? -1 : i+1));
    if (res == -1 && i+1 == argc) {
      std::cerr << "Not enough arguments for " << argv[i] << std::endl;
      std::cerr << argv[0] << " --help for assistance." << std::endl;
      exit(1);
    } else if (res == -1) {
      std::cerr << argv[0] << " --help for assistance." << std::endl;
      exit(1);
    }
    i += res;
  }

  for (auto &arg : arguments) {
    if (!arg.second->ok) {
      std::cerr << arg.first << " is required." << std::endl;
      std::cerr << argv[0] << " --help for assistance." << std::endl;
      exit(1);
    }
  }
  return i;
}

inline void ArgList::help()
{
  unsigned l1 = 0;
  unsigned l2 = 0;
  for (auto& arg : arguments) {
    std::stringstream ss;
    ss << " " << arg.second->flag << " ";
    if (ss.str().length() > l1) {
      l1 = ss.str().length();
    }
  }
  l1++;
  for (auto& arg : arguments) {
    std::stringstream ss;
    ss << "  " << arg.second->flag;
    ss << std::string(std::max(std::string::size_type(1), l1-ss.str().length()), ' ');
    ss << "= " << arg.second->value();
    if (ss.str().length() > l2) {
      l2 = ss.str().length();
    }
    ss << std::string(" ");
    ss << arg.second->help_text << std::endl;
  }
  l2++;
  for (auto& arg : arguments) {
    std::stringstream ss;
    ss << "  " << arg.second->flag;
    ss << std::string(std::max(std::string::size_type(1), l1-ss.str().length()), ' ');
    ss << "= " << arg.second->value();
    ss << std::string(std::max(std::string::size_type(1), l2-ss.str().length()), ' ');
    ss << arg.second->help_text << std::endl;
    std::cerr << ss.str();
  }
}

#endif // ARGUMENTS_H
