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
#pragma once
#include <iostream> 
#include <functional>
#include <string>

#ifdef _WIN32
#include <windows.h>
#if  defined(__x86_64__) || defined(__ppc64__)
// ** 64 bit
#else
// ** 32 bit  
#endif
#elif __linux__
#warning "you must add linux serial lib"
#elif __APPLE__
#warning "you must add mac serial lib"
#else
#warning "OS is not known"
#endif

class serial_port_api {

public:

    enum class error_status_e { NONE_ERROR, READ_ERROR, TRANSMIT_ERROR, CONFIG_ERROR, CREATE_ERROR };

    /**
        ex _port_name: "COM5"
    **/
    explicit serial_port_api(std::string _port_name);

    serial_port_api();

    ~serial_port_api();

    serial_port_api(const serial_port_api&) = delete;

    serial_port_api& operator=(const serial_port_api&) = delete;

    bool open(std::string _port_name);

    bool close();

    template <typename Function, typename Class>
    void bind(Function&& function, Class* handler) {
        m_rcv_callback = std::bind(std::forward<Function>(function), handler, std::placeholders::_1, std::placeholders::_2);
    }

    void transmit(const char* data, int size);

    void transmit(std::string str);

    bool operator()()const;

    bool is_open()const;

    void set_baudrate(int br);

    int get_baudrate()const;

    error_status_e get_error_state()const;

    void async_run();

private:

    std::function<void(const char*, int)> m_rcv_callback;

    bool m_state;

    HANDLE m_serial;

    std::string m_port_name;

    int m_baudrate;

    error_status_e m_error;

    /** API reach functions **/

    bool m_windows_ll_connect(LPCWSTR port_name);
    void m_windows_ll_transmit(const char* data, int size);
    void m_windows_ll_polling();

};



