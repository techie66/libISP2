#ifndef ISP2_H
#define ISP2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

#define ISP2_LAMBDA_DIVISOR 1000
#define ISP2_LAMBDA_OFFSET 500

// Endianness Dependant
//  Multi-byte comparisons need correct order
#ifdef WORDS_BIGENDIAN
// Big-Endian
#define ISP2_HEADER_BITS 0xA280
#define ISP2_LC2_HEADER_BITS 0xE280
#define ISP2_DATA_BITS 0xC080
#define ISP2_HEADER_SIGNATURE 0xA280
#define ISP2_LC2_HEADER_SIGNATURE 0x4200
#define ISP2_DATA_SIGNATURE 0x0000
#define ISP2_RECORDING_BIT 0x4000
#define ISP2_IS_SENSOR_BIT 0x1000
#define ISP2_CAN_LOG_BIT 0x0800

#else
// Litle-Endian
#define ISP2_HEADER_BITS 0x80A2
#define ISP2_LC2_HEADER_BITS 0x80E2
#define ISP2_DATA_BITS 0x80C0
#define ISP2_HEADER_SIGNATURE 0x80A2
#define ISP2_LC2_HEADER_SIGNATURE 0x0042
#define ISP2_DATA_SIGNATURE 0x0000
#define ISP2_RECORDING_BIT 0x0040
#define ISP2_IS_SENSOR_BIT 0x0010
#define ISP2_CAN_LOG_BIT 0x0008

#endif /* WORDS_BIGENDIAN */

//Deprecated
//#define ISP2_HEADER_BYTE1 0xA2
//#define ISP2_HEADER_BYTE2 0x80

enum isp2_status{
	ISP2_NORMAL=0,
	ISP2_O2,
	ISP2_CALIBRATING,
	ISP2_NEED_CALIBRATION,
	ISP2_WARMING,
	ISP2_HEATER_CALIBRATING,
	ISP2_LAMBDA_ERROR_CODE,
	ISP2_RESERVED
};

enum word_type {
	ISP2_HEADER_WORD,
	ISP2_LC2_HEADER_WORD,
	ISP2_DATA_WORD,
	ISP2_UNK_WORD
};

struct isp2_t{
	uint16_t	lambda;
	isp2_status	status;
	uint8_t		afr_multiplier;
	uint8_t		packet_length;
	bool		is_recording;
	bool		is_sensor_data;
	bool		sender_can_log;
};

class ISP2
{
	public:
		static int isp2_read( int file, isp2_t& isp_data );
	private:
		static uint16_t get_header(int file);
		static bool get_is_recording(uint16_t header);
		static bool get_is_sensor(uint16_t header);
		static bool get_can_log(uint16_t header);
		static uint8_t get_packet_length(uint16_t header);
		static uint16_t get_next_word(int file);
		static word_type get_word_type(uint16_t word);
		static isp2_status get_status(uint16_t word);
		static uint8_t get_afr_multiplier(uint16_t word);
		static uint16_t get_lambda(uint16_t word);
};

#endif



