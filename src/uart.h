#pragma once

#include <termios.h>
#include "threaded_fd.h"

class Uart : public Threaded_Fd
{
  public:

	enum SerialPort
	{
		Uart1,
		Uart2
	};
	
	enum BaudRate
	{
		b50=B50,
		b75=B75,
		b110=B110,
		b134=B134,
		b150=B150,
		b200=B200,
		b300=B300,
		b600=B600,
		b1200=B1200,
		b1800=B1800,
		b2400=B2400,
		b4800=B4800,
		b9600=B9600,
		b19200=B19200,
		b38400=B38400,
		b57600=B57600,
		b115200=B115200,
		b230400=B230400,
		b460800=B460800,
		b500000=B500000,
		b576000=B576000,
		b921600=B921600,
		b1000000=B1000000,
		b1152000=B1152000,
		b1500000=B1500000,
		b2000000=B2000000,
		b2500000=B2500000,
		b3000000=B3000000,
		b3500000=B3500000,
		b4000000=B4000000
	};

	enum Parity
	{
		None = 0,
		Odd = (PARENB | PARODD),
		Even = (PARENB)
	};

	enum StopBits
	{
		One = 0,
		Two = CSTOPB
	};

	enum DataBits
	{
		d5=CS5,
		d6=CS6,
		d7=CS7,
		d8=CS8
	};

	struct DataFormat
	{
		DataFormat():
			db(d8),
			p(None),
			sb(One)
		{}
		
		DataBits db;
		Parity p;
		StopBits sb;
	};

	Uart(SerialPort uart_num);
	~Uart();

	const std::string & device_path();

	void set_baud(BaudRate baud);
	BaudRate baud();

	bool start();

	void set_format(DataBits db, Parity p, StopBits sb);
	void set_format(const DataFormat & data_format);
	const DataFormat & format();

  private:

	int32_t _raw_read(uint8_t * buffer, uint32_t max_size);
	int32_t _raw_write(uint8_t * buffer, uint32_t max_size);

	void _set_attribs();

	DataFormat m_df;
	BaudRate m_baud;
	std::string m_devpath;
};
