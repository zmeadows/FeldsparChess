#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import unstd.core;
import unstd.random;

import prelude;
import bitboard;
import board;
import quad;

namespace _FELDSPAR_TEST {

TEST_CLASS(_QUAD_TEST)
  {
 
  TEST_METHOD(_PACK_UNPACK)
  {
      QuadBitboard qbb =
          pack(0, 1, std::numeric_limits<U64>::max(), (U64)std::numeric_limits<S64>::max() + 1);
  
      alignas(QuadBitboard) Bitboard bbs[4];
      unpack(qbb, bbs);
  
      Assert::AreEqual(bbs[0], static_cast<Bitboard>(0));
      Assert::AreEqual(bbs[1], static_cast<Bitboard>(1));
      Assert::AreEqual(bbs[2], static_cast<Bitboard>(std::numeric_limits<U64>::max()));
      Assert::AreEqual(bbs[3], static_cast<Bitboard>(std::numeric_limits<S64>::max() + 1));
  
      for (auto i = 0; i < 1000; i++) {
          const Bitboard b1 = rand<U64>();
          const Bitboard b2 = rand<U64>();
          const Bitboard b3 = rand<U64>();
          const Bitboard b4 = rand<U64>();
  
          QuadBitboard qbb = pack(b1, b2, b3, b4);
  
          alignas(QuadBitboard) Bitboard bbs[4];
          unpack(qbb, bbs);
  
          Assert::AreEqual(bbs[0], b1);
          Assert::AreEqual(bbs[1], b2);
          Assert::AreEqual(bbs[2], b3);
          Assert::AreEqual(bbs[3], b4);
      }
  }
  
  TEST_METHOD(OPERATOR_OR_EQUALS)
  {
      for (auto i = 0; i < 1000; i++) {
          const Bitboard b1 = rand<U64>();
          const Bitboard b2 = rand<U64>();
          const Bitboard b3 = rand<U64>();
          const Bitboard b4 = rand<U64>();
  
          const Bitboard b5 = rand<U64>();
          const Bitboard b6 = rand<U64>();
          const Bitboard b7 = rand<U64>();
          const Bitboard b8 = rand<U64>();
  
          QuadBitboard qbb1 = pack(b1, b2, b3, b4);
          QuadBitboard qbb2 = pack(b5, b6, b7, b8);
          qbb1 |= qbb2;
  
          alignas(QuadBitboard) Bitboard bbs[4];
          unpack(qbb1, bbs);
  
          Assert::AreEqual(bbs[0], b1 | b5);
          Assert::AreEqual(bbs[1], b2 | b6);
          Assert::AreEqual(bbs[2], b3 | b7);
          Assert::AreEqual(bbs[3], b4 | b8);
      }
  }
  
  TEST_METHOD(OPERATOR_AND)
  {
      for (auto i = 0; i < 1000; i++) {
          const Bitboard b1 = rand<U64>();
          const Bitboard b2 = rand<U64>();
          const Bitboard b3 = rand<U64>();
          const Bitboard b4 = rand<U64>();
  
          const Bitboard b5 = rand<U64>();
          const Bitboard b6 = rand<U64>();
          const Bitboard b7 = rand<U64>();
          const Bitboard b8 = rand<U64>();
  
          QuadBitboard qbb1 = pack(b1, b2, b3, b4);
          QuadBitboard qbb2 = pack(b5, b6, b7, b8);
          const QuadBitboard qbb3 = qbb1 & qbb2;
  
          alignas(QuadBitboard) Bitboard bbs[4];
          unpack(qbb3, bbs);
  
          Assert::AreEqual(bbs[0], b1 & b5);
          Assert::AreEqual(bbs[1], b2 & b6);
          Assert::AreEqual(bbs[2], b3 & b7);
          Assert::AreEqual(bbs[3], b4 & b8);
      }
  }
  
  TEST_METHOD(LEFT_SHIFT)
  {
      for (auto i = 0; i < 1000; i++) {
          const Bitboard b1 = rand<U64>();
          const Bitboard b2 = rand<U64>();
          const Bitboard b3 = rand<U64>();
          const Bitboard b4 = rand<U64>();
  
          const Bitboard b5 = 0;
          const Bitboard b6 = 7;
          const Bitboard b7 = 13;
          const Bitboard b8 = 15;
  
          const QuadBitboard qbb1 = pack(b1, b2, b3, b4);
          const QuadBitboard qbb2 = pack(b5, b6, b7, b8);
          const QuadBitboard qbb3 = qbb1 << qbb2;
  
          alignas(QuadBitboard) Bitboard bbs[4];
          unpack(qbb3, bbs);
  
          Assert::AreEqual(bbs[0], b1 << b5);
          Assert::AreEqual(bbs[1], b2 << b6);
          Assert::AreEqual(bbs[2], b3 << b7);
          Assert::AreEqual(bbs[3], b4 << b8);
      }
  }
 };

}