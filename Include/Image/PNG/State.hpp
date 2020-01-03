#ifndef LIBTCOD_STATE_HPP
#define LIBTCOD_STATE_HPP

#include "Image/PNG/Info.hpp"
#include "Image/PNG/ColorMode.hpp"
#include "Image/PNG/DecoderSettings.hpp"
#include "Image/PNG/EncoderSettings.hpp"

/*The settings, state and information for extended encoding and decoding.*/
class LodePNGState
{

public:

	LodePNGDecoderSettings decoder; /*the decoding settings*/
	LodePNGEncoderSettings encoder; /*the encoding settings*/
	LodePNGColorMode info_raw; /*specifies the format in which you would like to get the raw pixel buffer*/
	LodePNGInfo info_png; /*info of the PNG image obtained after decoding*/
	unsigned error = 1;

	LodePNGState() = default;

	//For the lodepng::State subclass.
	virtual ~LodePNGState() = default;

	// Methods

	/**
	 * Read the information from the header and store it
	 * in the LodePNGInfo. return value is error
	 *
	 * Read the PNG header, but not the actual data. This
	 * returns only the information that is in the header
	 * chunk of the PNG, such as width, height and color type.
	 * The information is placed in the info_png field
	 * of the LodePNGState.
	 *
	 * @param in Header chunk of the PNG.
	 * @return Value is error
	 */
	unsigned int inspect(unsigned* w, unsigned* h, const unsigned char* in, size_t insize);

	static unsigned read32BitInt(const unsigned char* buffer);

	/**
	 * Calculate CRC32 of buffer
	 *
	 * @return the CRC of the bytes bufffer[0..length-1].
	 */
	static unsigned crc32(const unsigned char* buffer, size_t length);

};

#endif //LIBTCOD_STATE_HPP