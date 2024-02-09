# w_serial_asos

# Serial Port API for Windows

This library provides a C++ API for serial port operations on Windows operating systems. It offers basic functionalities for serial communication and enables users to open, close, transmit data, and perform other operations on serial ports.

## Features

- Opening and closing serial ports
- Data transmission
- Managing connection errors
- Baud rate setting
- Asynchronous operation capability

## Usage

### Installation

Clone the library and include `serial_port_api.h` header file in your project.

### Example Usage

```cpp
#include "serial_port_api.h"
#include <iostream>

int main() {
    // Creating a serial port object
    serial_port_api serial_port("COM5");

    // Opening the serial port
    if (!serial_port.open("COM5")) {
        std::cerr << "Failed to open the serial port!\n";
        return 1;
    }
  
    while (true)
    {
        // Data transmission
        serial_port.transmit("Hello, serial port!");

        serial_port.polling();
        
        Sleep(200);
   
    }

    // Closing the serial port
    serial_port.close();

    return 0;
}

Contributing
If you encounter any bugs or issues while using the library, please file an issue.
Add new features or improve existing code and submit a pull request.
License
This library is licensed under the MIT License. See the LICENSE file for more information.

Contact
For questions, suggestions, or feedback, you can email me

' Modern CPP 17 ' 
Visual Studio 2022 for test. 
it can be use in every Hardware/PC/Framework.
You can use this library for your Projects.

Ali Şahbaz
ali_sahbaz@outlook.com
