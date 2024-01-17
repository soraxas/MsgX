# MsgX: Message eXchange

This repository is **MsgX**, a message exchange interface to send binary data between languages, e.g., from C++ to Python. This is achived by serialising and deserialising native binary representation from some common interface.

This can be used as a *publisher* and *subscriber* model between C++ and Python, and a stateful model in the **Jupyter notebook** ecosystem that captures the incoming message. It supports asynchronous refresh within the notebook whenever a new message is published from your C++ files, and automatically display them within the notebook. This library also support non-notebook usage where it uses MsgX's native figure display from within your browser.

# Requirements

- zeromq (messaging protocol)

- capnproto (serialisation of message)

# Using the library in your C++ project

You can include the project it as you normally. The following shows a snippet to quickly visualise your code using MsgX without much setup

## Using CPM

Using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) to auto-install MsgX:

```cmake
include(cmake/CPM.cmake)
cpmaddpackage("gh:soraxas/msgx@1.0.5")
```

## Quick and dirty out-of-tree build in cmake

In your **CMakeLists.txt**:

```cmake
cmake_minimum_required(...)

...

# Add as out-of-tree build (with absolute path & binary directory)
add_subdirectory(
        /home/soraxas/git-repo/msgx
        msgx
)
find_package( msgx REQUIRED )

...

# link the library to your target
target_link_libraries( ${MY_TARGET} msgx )
```

# Example Usage

Say you have something like this in your `${MY_TARGET}`

```cpp
#include "msgx/message.h"

using namespace msgx::kwargs;

int main(int argc, char const *argv[]) {
    msgx::MessageX msg;
    std::vector<double> x{5, 2, 0, 4, 4, 6, 7};
    std::vector<double> y{1, 2, 3, 2, 3, 4, 5};

    msg = msg.Mapping("xs"_kw = x, "ys"_kw = y);
    msg.send();
}
```
From python:
```python
>>> from msgx import spin_default
>>> spin_default()

# msg.send(); in c++

{'ys': array([1., 2., 3., 2., 3., 4., 5.]), 'xs': array([5., 2., 0., 4., 4., 6., 7.])}
```

