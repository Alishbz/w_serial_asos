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
#include "serial_port_api.h"


/**
    ex _port_name: "COM5"
**/
serial_port_api::serial_port_api(std::string _port_name) : m_serial(nullptr), m_port_name{}, m_state(false), m_rcv_callback(nullptr), m_baudrate(115200), m_error(error_status_e::NONE_ERROR)
{
    std::wstring wPortName(_port_name.begin(), _port_name.end());
    LPCWSTR port_name = wPortName.c_str();

    if (m_windows_ll_connect(port_name)) {
        m_state = true;
        m_port_name = _port_name;
    }
}

serial_port_api::serial_port_api() :m_serial(nullptr), m_port_name{}, m_state(false), m_rcv_callback(nullptr), m_baudrate(115200), m_error(error_status_e::NONE_ERROR)
{
}

serial_port_api::~serial_port_api() {
    if (m_state == true) {
        CloseHandle(m_serial);
        m_state == false;
    }
}

/**
    ex _port_name: "COM5"
**/
bool serial_port_api::open(std::string _port_name)
{
    std::wstring wPortName(_port_name.begin(), _port_name.end());
    LPCWSTR port_name = wPortName.c_str();

    if (m_state == false &&
        m_windows_ll_connect(port_name)) {
        m_state = true;
        m_port_name = _port_name;
    }

    return m_state;
}

bool serial_port_api::close()
{
    if (m_state == true) {
        CloseHandle(m_serial);
        m_state == false;
    }

    return m_state;
}



void serial_port_api::transmit(const char* data, int size)
{
    if (m_state == true) {

        m_windows_ll_transmit(data, size);
    }
}

void serial_port_api::transmit(std::string str)
{
    const char* cstr = str.c_str();

    int length = static_cast<int>(str.length());

    if (m_state == true) {

        m_windows_ll_transmit(cstr, length);
    }
}

bool serial_port_api::operator()()const
{
    return m_state;
}

bool serial_port_api::is_open()const
{
    return m_state;
}

void serial_port_api::set_baudrate(int br)
{
    if (br)
        m_baudrate = br;
}

int serial_port_api::get_baudrate()const
{
    return m_baudrate;
}

serial_port_api::error_status_e serial_port_api::get_error_state()const
{
    return m_error;
}

/**
polling - timer - thread?
*/
void serial_port_api::async_run(void)
{
    if (m_state == true) {

        m_windows_ll_polling();
    }
}

void serial_port_api::m_windows_ll_transmit(const char* data, int size)
{
    DWORD bytesWritten;
    if (!WriteFile(m_serial, data, size, &bytesWritten, NULL)) {
        m_error = error_status_e::TRANSMIT_ERROR;
        CloseHandle(m_serial);
        m_state == false;
    }
}

void serial_port_api::m_windows_ll_polling()
{
    constexpr int MAX_RECEIVE_SIZE = 1024 * 4;

    char buffer[MAX_RECEIVE_SIZE] = { 0 };

    DWORD  bytes_read = 0, errmask = 0;
    COMSTAT st;

    if (!ClearCommError(m_serial, &errmask, &st))
        return;

    if (st.cbInQue <= 0)
        return;

    OVERLAPPED ov;
    ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ov.hEvent == NULL)
        return;
    ov.Internal = ov.InternalHigh = 0;
    ov.Offset = ov.OffsetHigh = 0;


    if (!ReadFile(m_serial, buffer, st.cbInQue, &bytes_read, &ov)) {
        // @ref: lib in read() ->  https://github.com/firmata/firmata_test/blob/master/serial.cpp
        //                         https://github.com/jjuiddong/SerialPort/blob/master/SerialPortTest/Serial.cpp
        if (!(GetLastError() == ERROR_IO_PENDING && GetOverlappedResult(m_serial, &ov, &bytes_read, TRUE))) {
            // WaitForSingleObject( ov.hEvent, 2000 );
            m_error = error_status_e::READ_ERROR;

            CloseHandle(ov.hEvent);

            return;
        }
    }

    if (bytes_read > 0 && bytes_read < MAX_RECEIVE_SIZE - 1) {

        buffer[bytes_read++] = '\0';

        if (m_rcv_callback) {
            m_rcv_callback((const char*)buffer, bytes_read);
        }
    }
    else if (bytes_read == 0) {
        m_state == false;
    }

    CloseHandle(ov.hEvent);
}

bool serial_port_api::m_windows_ll_connect(LPCWSTR port_name)
{
    m_serial = CreateFile(port_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (m_serial == INVALID_HANDLE_VALUE) {
        m_error = error_status_e::CREATE_ERROR;
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(m_serial, &dcbSerialParams)) {
        m_error = error_status_e::CONFIG_ERROR;
        CloseHandle(m_serial);
        return false;
    }

    dcbSerialParams.BaudRate = m_baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(m_serial, &dcbSerialParams)) {
        m_error = error_status_e::CONFIG_ERROR;
        CloseHandle(m_serial);
        return false;
    }

    return true;
}

