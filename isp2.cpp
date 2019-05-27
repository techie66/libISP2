#include "isp2.h"

int ISP2::isp2_read(int file,isp2_t& isp_data )
{
	/* The only public function of libISP2
	*  reads from a file descriptor and parses it
	*  to pass back a isp2_t struct containing 
	*  relevant information.
	*  Makes use of the other private funtions.
	*/
	uint16_t	header;
	uint16_t	current_word;


	// Get header
	header = ISP2::get_header(file);
	if (header == 0 ) {
		return -1;
	}
	// get packet length
	isp_data.packet_length = ISP2::get_packet_length(header);
	// get sender can log
	isp_data.sender_can_log = ISP2::get_can_log(header);
	// get is sensor data
	isp_data.is_sensor_data = ISP2::get_is_sensor(header);
	// get is recording
	isp_data.is_recording = ISP2::get_is_recording(header);
	// get remainder of packet
	int remaining = isp_data.packet_length;
	// get next word
	while (remaining > 0) {
		current_word = ISP2::get_next_word(file);
		remaining--;

		switch (ISP2::get_word_type(current_word)) {
			case ISP2_LC2_HEADER_WORD:
				// get status
				isp_data.status = ISP2::get_status(current_word);
				// get afr multiplier
				isp_data.afr_multiplier = ISP2::get_afr_multiplier(current_word);
				// get next word
				current_word = ISP2::get_next_word(file);
				remaining--;
				// verify is DATA_WORD
				if (ISP2::get_word_type(current_word) == ISP2_DATA_WORD) {
					// TODO get lambda
					isp_data.lambda = ISP2::get_lambda(current_word);
				}
				break;
			
			// TODO aux packet
			case ISP2_DATA_WORD:
				break;

			// TODO lm-1 packet
			case ISP2_UNK_WORD:
				break;
		}
	}

	return 0;
	
}

uint16_t ISP2::get_header(int file) {
	uint8_t		header[2] = {0};

	// Read from 'file' until bytes match header
	int		result;
	do {
		header[0] = header[1];
		result = read(file,&header[1],1);
		if (result == -1) {
			// Error in read
			perror("ISP2 read error");
			return 0;
		}
		else if (result == 0) {
			// Didn't read anything
			perror("ISP2 read nothing");
			return 0;
		}
	}
	while (ISP2::get_word_type(*(uint16_t*)header) != ISP2_HEADER_WORD);
	
	//Header found
	return *(uint16_t*)header;
}

bool ISP2::get_is_recording(uint16_t header) {
	if (header & ISP2_RECORDING_BIT) {
		return true;
	}
	else {
		return false;
	}
}

bool ISP2::get_is_sensor(uint16_t header) {
	if (header & ISP2_IS_SENSOR_BIT) {
		return true;
	}
	else {
		return false;
	}
}

bool ISP2::get_can_log(uint16_t header) {
	if (header & ISP2_CAN_LOG_BIT) {
		return true;
	}
	else {
		return false;
	}
}

uint8_t ISP2::get_packet_length(uint16_t header) {
	uint8_t* header_ptr = (uint8_t*)&header;
	
	// first byte
	header_ptr[0] <<= 7; // move low bit to high bit
	header_ptr[0] |= 0x7F; // make all lower bits high

	// second byte
	header_ptr[1] &= header_ptr[0]; // mask second byte with first to put length together

	return header_ptr[1];
}

uint16_t ISP2::get_next_word(int file) {
	uint8_t	buf[2] = {0};
	int result;
#define OLD
#ifdef OLD
	result = read(file,buf,2);
	if (result == -1) {
		// Error in read
		perror("ISP2 read error");
		return 0;
	}
	else if (result == 0) {
		// Didn't read anything
		perror("ISP2 read nothing");
		return 0;
	}
	else if (result == 1) {
		// Didn't read enough
		perror("ISP2 short read");
		return 0;
	}
	else {
		return *(uint16_t*)buf;
	}
#endif
#ifdef NEW
	// Read from 'file' until bytes match known word type
	do {
		buf[0] = buf[1];
		result = read(file,&buf[1],1);
		if (result == -1) {
			// Error in read
			perror("ISP2 read error");
			return 0;
		}
		else if (result == 0) {
			// Didn't read anything
			perror("ISP2 read nothing");
			return 0;
		}
	}
	while (ISP2::get_word_type(*(uint16_t*)buf) == ISP2_UNK_WORD);
	
	// found
	return *(uint16_t*)buf;
#endif
}

word_type ISP2::get_word_type(uint16_t word) {
	uint16_t temp = word;
	temp &= ISP2_LC2_HEADER_BITS;
	if (!(temp^ISP2_LC2_HEADER_SIGNATURE)) {
		perror("ISP2: LC-2 header word");
		return ISP2_LC2_HEADER_WORD;
	}
	temp = word;
	temp &= ISP2_DATA_BITS;
	if (!(temp^ISP2_DATA_SIGNATURE)) {
		perror("ISP2: Data word");
		return ISP2_DATA_WORD;
	}
	temp = word;
	temp &= ISP2_HEADER_BITS;
	if (!(temp^ISP2_HEADER_SIGNATURE)) {
		perror("ISP2: Header word");
		return ISP2_HEADER_WORD;
	}
	perror("ISP2: Invalid Data Word");
	return ISP2_UNK_WORD;
}

isp2_status ISP2::get_status(uint16_t word) {
	uint8_t byte1 = *(uint8_t*)&word;
	byte1 >>= 2;
	byte1 &= 0x07;
	return (isp2_status)byte1;
}

uint8_t ISP2::get_afr_multiplier(uint16_t word) {
	uint8_t* word_ptr = (uint8_t*)&word;
	
	// first byte
	word_ptr[0] <<= 7; // move low bit to high bit

	// second byte
	word_ptr[1] |= word_ptr[0]; // mask second byte with first to put length together

	return word_ptr[1];
}

uint16_t ISP2::get_lambda(uint16_t word) {
	uint8_t* word_ptr = (uint8_t*)&word;
	uint8_t low_byte_high_bit = word_ptr[0] << 7; // save low bit as high bit
	word_ptr[1] |= low_byte_high_bit; // put high bit back in the right byte
	word_ptr[0] >>= 1; // shift high byte over
	word_ptr[0] &= 0x1F; // mask high byte just in case extra bits got in there
	
	#ifndef WORDS_BIGENDIAN
	// swap bytes, standard XOR swap algorithm
	word_ptr[0] = word_ptr[0] ^ word_ptr[1];
	word_ptr[1] = word_ptr[1] ^ word_ptr[0];
	word_ptr[0] = word_ptr[0] ^ word_ptr[1];
	#endif /* WORDS_BIGENDIAN */

	return word;	
}
