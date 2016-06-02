#include "unity.h"
#include "chesstypes.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_is_castling_available(void)
{

	CastlingAvailability castl = 0 | WhiteKingSide | BlackQueenSide;

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));
	
	castl = 0 | WhiteQueenSide;

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));

	castl = 0;

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	castl = 0 | WhiteQueenSide | WhiteKingSide |
		BlackKingSide | BlackQueenSide;

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));


	castl = 0 | WhiteQueenSide |
		BlackKingSide | BlackQueenSide;

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));

}

void test_set_castling_availability(void)
{

	CastlingAvailability castl = 0;

	setCastlingAvailability(&castl, WhiteKingSide, true);
	setCastlingAvailability(&castl, BlackQueenSide, true);

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, false);
	setCastlingAvailability(&castl, BlackQueenSide, true);
	setCastlingAvailability(&castl, WhiteQueenSide, false);
	setCastlingAvailability(&castl, BlackKingSide, false);

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, false);
	setCastlingAvailability(&castl, BlackQueenSide, false);
	setCastlingAvailability(&castl, WhiteQueenSide, false);
	setCastlingAvailability(&castl, BlackKingSide, false);

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, true);
	setCastlingAvailability(&castl, BlackQueenSide, true);
	setCastlingAvailability(&castl, WhiteQueenSide, true);
	setCastlingAvailability(&castl, BlackKingSide, true);

	TEST_ASSERT(isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(isCastlingAvailable(castl, WhiteQueenSide));

	setCastlingAvailability(&castl, WhiteKingSide, false);
	setCastlingAvailability(&castl, BlackQueenSide, false);
	setCastlingAvailability(&castl, WhiteQueenSide, false);
	setCastlingAvailability(&castl, BlackKingSide, true);

	TEST_ASSERT(!isCastlingAvailable(castl, WhiteKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, BlackQueenSide));
	TEST_ASSERT(isCastlingAvailable(castl, BlackKingSide));
	TEST_ASSERT(!isCastlingAvailable(castl, WhiteQueenSide));

}
