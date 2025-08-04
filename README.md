![C++](https://img.shields.io/badge/language-C++-blue)
[![License](https://img.shields.io/badge/License-GPLv3-white.svg)](LICENSE)

# mee
## Table of Contents
- [Description](#description)
- [Requirements](#requirements)
- [Build](#build)
- [Features](#features)
- [Examples](#examples)
- [Extensions](#extensions)

## Description
<b>mee</b> is a lightweight C++ library designed to simplify the process of creating expression parsers. It provides the user with abstract key elements that integrate seamlessly with your code. The developers can use these provided components to create their own parsers. By design, the library includes two built-in parsers:
 - <b>Recursive Descent Parser</b>;
 - <b>Top-Down Operator Precedence Parser</b>.

## Requirements
- <b>C++17 or newer</b> standard revision
### On Windows:
- When building with MSYS2, Shared Libraries use <b>.so</b> file extension.
- When building with native MinGW-w64, Shared Libraries use the <b>.dll</b> default Windows file extension.
- In order to make sure the program will work properly, place compiled Shared Library in <b>bin</b> directory.

## Build
| Command            | Description                             |
|--------------------|-----------------------------------------|
| `make`             |Build programs linked with both libraries|
| `make static`      |Build program linked with Static Library |
| `make shared`      |Build program linked with Shared Library |
| `make libs`        |Build Static and Shared Library          |
| `make clean`       |Clean build files                        |

## Features
- <b>Extensible Project Architecture</b>;
- <b>Tokenizer</b>;
- <b>Abstracted Nodes</b> for numbers and operators;
- <b>Lightweight</b> & <b>single-header file design</b>;
- <b>Two Ready-To-Use built-in parsers</b> for use.

## Examples
<b>mee</b> project provides examples in <b>source/examples</b> for each built-in parser.
They are built using the <b>mee tokenizer</b>.

### Manually adding nodes
Recursive Descent Parser:
```cpp
#include "mee.hpp"
#include <iostream>

int main(int argc, char **argv)
{
  mee::common::num num1(2.5);
  mee::common::num num2(2.5);
  mee::common::op op(mee::common::optype::add);
// Upper code is common and is used by both parsers.
// We need a standard vector to store the nodes.
// Note: when manually adding nodes, ensure they are ordered like this: number-operator-number.
// This ordering is due to the parser logic, this is automatic when using the lexical analysis.
  std::vector<mee::common::node> vec1 = { num1, op, num2 };
// Right now we will be using RDP.
  mee::rdp::expr expr(vec1);

// Get the evaluated result.
  double expr_res = expr.parse_expr();
  std::cout << "result: " << expr_res << '\n';
}
```
Top-Down Operator Precedence Parser:
```cpp
#include "mee.hpp"
#include <iostream>

int main(int argc, char **argv)
{
  mee::common::num num1(2.5);
  mee::common::num num2(2.5);
  mee::common::op op(mee::common::optype::add);
  std::vector<mee::common::node> vec1 = { num1, op, num2 };
// The only change we made is here.
  mee::tddp::expr expr(vec1);

// Get the evaluated result.
// The method name is different due to the different parser functionality.
  double expr_res = expr.parse();
  std::cout << "result: " << expr_res << '\n';
}
```

Let's now run the program:
```
$ make
$ ./main
result: 5
```

## Extensions
Programmers can extend the architecture of <b>mee</b> at any time adding different parser logics, additional common code or improving the lexical analyzer.
