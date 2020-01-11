#ifndef LIBTCOD_HUFFMANTREE_HPP
#define LIBTCOD_HUFFMANTREE_HPP

#include <vector>

class HuffmanTree
{

private:

	/**
	 * 256 literals, the end code, some length
	 * codes, and 2 unused codes
	 */
	static constexpr short NUM_DEFLATE_CODE_SYMBOLS = 288;

	/**
	 * the code length codes. 0-15: code
	 * lengths, 16: copy previous 3-6 times,
	 * 17: 3-10 zeros, 18: 11-138 zeros
	 */
	static constexpr short NUM_CODE_LENGTH_CODES = 19;

	/**
	 * the distance codes have their own symbols,
	 * 30 used, 2 unused
	 */
	static constexpr short NUM_DISTANCE_SYMBOLS = 32;

	/**
	 * the order in which "code length alphabet code lengths"
	 * are stored, out of this the huffman tree of the dynamic
	 * huffman tree lengths is generated
	 */
	static constexpr short CLCL_ORDER[NUM_CODE_LENGTH_CODES] =
			{ 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

public:

	std::vector <unsigned> tree2d;
	std::vector <unsigned> tree1d;

	/**
	 * the lengths of the codes of the 1d-tree.
	 */
	std::vector <unsigned> lengths;

	/**
	 * maximum number of bits a single code can get.
	 */
	unsigned maxbitlen = 15;

	/**
	 * number of symbols in the alphabet = number of codes.
	 */
	unsigned numcodes = NUM_DEFLATE_CODE_SYMBOLS;

	// Methods

	/**
	 * get the tree of a deflated block with fixed
	 * tree, as specified in the deflate specification.
	 */
	static void getTreeInflateFixed(
			HuffmanTree& tree_ll, HuffmanTree& tree_d);

	/**
	 * get the tree of a deflated block with dynamic tree,
	 * the tree itself is also Huffman compressed with a known tree
	 */
	static void getTreeInflateDynamic(
			HuffmanTree* tree_ll, HuffmanTree* tree_d,
			const std::vector <unsigned char>& in, size_t* bp);

private:

	/**
	 * get the literal and length code tree of a
	 * deflated block with fixed tree, as per the
	 * deflate specification.
	 */
	void generateFixedLitLenTree();

	/**
	 * get the distance code tree of a deflated block wit
	 * fixed tree, as specified in the deflate specification
	 */
	void generateFixedDistanceTree();

	void makeTreeDimensional();

	void makeTreeMultiDimensional();

	static unsigned readBitsFromStream(
			size_t* bitpointer,
			const std::vector <unsigned char>& bitstream,
			size_t nbits);

	/**
	 * @return the code, or (unsigned)(-1) if error happened
	 * inbitlength is the length of the complete buffer, in
	 * bits (so its byte length times 8)
	 */
	unsigned huffmanDecodeSymbol(const std::vector <unsigned char>& in,
			size_t* bp, size_t inbitlength);

};


#endif //LIBTCOD_HUFFMANTREE_HPP
