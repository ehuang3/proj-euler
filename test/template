#include "Prime.h"
#include <iostream>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>

using namespace std;
using namespace CppUnit;
using namespace Prime;

class PrimeTest : public TestFixture
{
   public:
   void testPhi() {
      CPPUNIT_ASSERT( phi(24) == 8 );
      CPPUNIT_ASSERT( phi(99) == 60 );
      CPPUNIT_ASSERT( phi(100000000) == 40000000 );
      CPPUNIT_ASSERT( phi(99999997) == 99921600 );
   }

   static Test* suite() {
      TestSuite* s = new TestSuite( "PrimeTest" );
      s->addTest(new TestCaller<PrimeTest>("testPhi",&PrimeTest::testPhi));
      return s;
   }
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   runner.addTest(PrimeTest::suite());
   runner.run();
}
