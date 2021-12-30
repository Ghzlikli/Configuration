# A C++20 Class for Reading Configuration Files

<!-- TOC depthFrom:2 -->

- [Introduction](#introduction)
- [Features](#features)
- [Usage](#usage)
- [Constructor](#constructor)
- [Member Functions](#member-functions)
- [Performance Test](#performance-test)
- [Version history](#version-history)
- [Feedback](#feedback)
- [Author and copyright](#author-and-copyright)

<!-- /TOC -->

## Introduction

Configuration files, also known as "config files", are used to set parameters and preferences of modern programs. They can also be used to select the input data.

The header file `MyConfiguration.hpp` contains a class for reading configuration files. The implementation requires no dependencies and is compatible with any C++20-compliant compiler.

The parameters are defined in `key = value; comments` format. When reading numeric values, if a parameter is not defined, the given default value is used.

## Features

- **Lightweight:** The code consists of only around 180 lines (excluding comments and blank lines).
- **Simple to use:** The user needs to include only a single header file  `#include "MyConfiguration.hpp"`.
- **Documented using Doxygen:** The comments are generated using Doxygen, making the code easy to understand and modify.
- **Printing available:** The `print_content()` member function prints the keys and values of the config file.

## Usage

To use the config class, include the header file:

```cpp
#include "MyConfiguration.hpp"
```

To define a new config object, simply write:

```cpp
config my_config("config.cfg");
```

`my_config` is the name of the config object, and the parentheses contain the name of the config file, `"config.cfg"`.

## Constructor

A config object can be constructed only by giving the name of the configuration file. The config file must be in the format of `key = value; comments`. The constructor reads the config file and stores the keys and values in a map container. It ignores empty lines and will throw several exceptions:

- `config::file_notfound`: Exception to be thrown if the file cannot be opened.
- `config::file_failed`: Exception to be thrown if reading the file encounters an error.
- `config::incompatible_format`: Exception to be thrown if the line is not in the valid format.
- `config::key_notfound`: Exception to be thrown if a line misses the key.
- `config::value_notfound`: Exception to be thrown if a line misses the value.

## Member Functions

There are three public member functions available:

- `get_value_string(key)`: Returns the string value of a key. This can be used when reading names, such as name of the input dataset. It will throw the `config::key_notfound` exception if the key is not found.
- `get_value(key, default_v)`: Returns the numeric value of a key. If the key is not found, the default value is returned. This can be used when initializing the parameters. The type of the return value is double (double-precision floating point format). It will throw the `config::num_invalid` exception if the value is invalid. To check the validity, the private member function `config::is_valid_number(value_st)` is used.
- `print_content()`: Prints the content of the config file in a `Key | Value` format, with comments and redundant spaces removed.

As mentioned earlier, there is one private member function:

- `config::is_valid_number(value_st)`: To check the validity of a number, the string is broken down into characters. It checks all the characters one by one, and if everything is fine, it will return `true`; otherwise, it will return `false`.

## Performance Test

The `test.cpp` file shows how to use the config class. It defines a config object named `my_config`, and reads the `config.cfg` file. First, it prints the content; then, it looks for a string value and a numeric value and prints them. To read a specific value type, such as a non-negative integer, we can run some tests on the received value. For example:

```cpp
double signed_v = my_config.get_value("n_iteration", 50);
if (signed_v < 0)
{
    cout << "n_iteration: ";
    throw invalid_argument("The parameter cannot be negative!\n");
}
if (signed_v - floor(signed_v))
{
    cout << "n_iteration: ";
    throw invalid_argument("The parameter must be an integer!\n");
}
n_iteration = (uint64_t)signed_v;
```

Then, it tries to get two missing keys. For the numeric value, the default value is returned. However, for the string value, the program terminates and prints an error. This library was tested on a system with Intel(R) Core(TM) i3-7100U CPU @ 2.40GHz 2.40 GHz processor and 8.00 GB (7.89 GB usable) of RAM, using GCC compiler v11.2.0 on Windows 10 build 19044.1415. To satisfy the C++20 requirement, the compiler must have the `-std=c++20` flag. The output is as follows:

```none
Configuration file is received.
Key   | Value
alpha | 0.95
datafile | mydata.csv
n_iteration | 100
theta | 0.9


Reading a string value: mydata.csv

Reading a numeric value: 0.95

Reading a non-negative integer value: 100


Reading a missing numeric value, returning the default value:
 The parameter beta is not defined in the configuration file; 
+ the default value of 0.89 is used.
0.89

Reading a missing string value: Requesting datafile2: Error: Key is not found!
```

## Version history

- Version 1.1 (2021-12-30)
  - Some cosmetic changes.
  - Added a separate exception for file failure (`config::file_failed`).
  - A `README.md` documentation file is added.
  - `test.cpp` and `config.cfg` files are added, which are helpful for testing.
  - Released in a separate [GitHub repository](https://github.com/Ghzlikli/Configuration).
- Version 1.0 (2021-12-23)
  - Initial release, as a part of [TSP-Hybrid_Optimization repository](https://github.com/Ghzlikli/TSP-Hybrid_Optimization).

## Feedback

Any feedback or request is highly appreciated through [open a new issue](https://github.com/Ghzlikli/Configuration/issues) or [email](mailto:khalili.ghazal.97@gmail.com).

## Author and copyright

Copyright (c) 2021 [Ghazal Khalili](mailto:khalili.ghazal.97@gmail.com).

If you use this class in your code, please acknowledge the author and provide a link to the [GitHub repository](https://github.com/Ghzlikli/CSE701_Ghazal).

**Thank you!**
