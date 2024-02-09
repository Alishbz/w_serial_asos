/****************************************************************************************
* ALI SAHBAZ
*
*
* Serial Port Maneger for Windows
*
* Date          : 07.02.2024
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/
#include <iostream>
#include "serial_port_api.h"

class serial_tester {

public:

    serial_tester() {

        m_comm = new serial_port_api();

        m_comm->bind(&serial_tester::m_serial_callback, this);

        m_comm->set_baudrate(115200);

        m_comm->open("COM4");

        if (!m_comm->is_open() || m_comm->get_error_state() != serial_port_api::error_status_e::NONE_ERROR) {
            std::cout << " error xx \n \n";
        }
    }

    ~serial_tester() = default;

    void serial_send(std::string str) {

        if ( !m_comm->is_open() || m_comm->get_error_state() != serial_port_api::error_status_e::NONE_ERROR) {
            std::cout << " error xx \n \n" ;
            return;
        }

        m_comm->transmit(str);
    }


    void polling( ) {

        if (!m_comm->is_open() || m_comm->get_error_state() != serial_port_api::error_status_e::NONE_ERROR) {
            std::cout << " error xx \n \n";
            return;
        }

        m_comm->async_run();
    }
    
private:

    void m_serial_callback(const char* receive_buff, int size) {
     
        std::string str = std::string(receive_buff, size);

        std::cout << "serial rcv : " << str  << "\n";
    }

    serial_port_api* m_comm;
};


int main() {

    std::cout << " Test app starting \n \n" << std::endl;

    serial_tester * my_app;

    my_app = new serial_tester();

    while (true)
    {
        my_app->polling();
        
        Sleep(200);

        my_app->serial_send("my_data sending \n");
    }

    return 0;
}