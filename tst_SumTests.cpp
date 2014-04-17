#include <QString>
#include <QtTest>
#include <stdexcept>
#include <sstream>

class LogicError : public std::logic_error {
public:

    LogicError( int argument ) : std::logic_error( "" ), m_argument( argument ) {

    }

    virtual const char *what( ) const throw () {
        return m_message.c_str( );
    }

    virtual ~LogicError( ) throw () {

    }

protected:
    int m_argument;
    std::string m_message;
};

class OutOfRange : public LogicError {
public:

    OutOfRange( int argument, int beginOfRange, int endOfRange ) : LogicError( argument ) {
        std::string str_argument, str_beginOfRange, str_endOfRange;

        str_argument = intToString( argument );
        str_beginOfRange = intToString( beginOfRange );
        str_endOfRange = intToString( endOfRange );

        m_message = "Argument " + str_argument + " doen't hit in the range [" +
                str_beginOfRange + ", " + str_endOfRange + "]";
    }

private:

    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

int sum( int number ) throw (OutOfRange);

class SumTests : public QObject {
    Q_OBJECT

public:
    SumTests( );

    private
Q_SLOTS:
    void testCase1_data( );
    void testCase1( );
};

SumTests::SumTests( ) {
}

void SumTests::testCase1_data( ) {
    QTest::addColumn<int>("number");
    QTest::addColumn<int>("expected");
    QTest::addColumn<bool>("isExpectedException");

    bool exception = true;
    bool no_exception = false;

    QTest::newRow( "sum_01" ) << 10001 << 0 << exception;
    QTest::newRow( "sum_02" ) << -10001 << 0 << exception;
    QTest::newRow( "sum_03" ) << 5 << 15 << no_exception;
    QTest::newRow( "sum_04" ) << 15 << 120 << no_exception;
    QTest::newRow( "sum_05" ) << 0 << 1 << no_exception;
    QTest::newRow( "sum_06" ) << 2 << 3 << no_exception;
    QTest::newRow( "sum_07" ) << -1 << 0 << no_exception;
    QTest::newRow( "sum_08" ) << -2 << -2 << no_exception;
    QTest::newRow( "sum_09" ) << -3 << -5 << no_exception;
    QTest::newRow( "sum_10" ) << -4 << -9 << no_exception;
    QTest::newRow( "sum_11" ) << -5 << -14 << no_exception;
    QTest::newRow( "sum_12" ) << -6 << -20 << no_exception;
}

void SumTests::testCase1( ) {
    QFETCH( int, number );
    QFETCH( int, expected );
    QFETCH( bool, isExpectedException );

    try {
        int actual = sum( number );
        if ( isExpectedException ) {
            QVERIFY2( false, "There is no exception." );
        } else {
            QCOMPARE( actual, expected );
        }
    } catch ( const LogicError& e ) {
        if ( !isExpectedException ) {
            QVERIFY2( false, "Exception was occur." );
        } else {
            QVERIFY2( true, "" );
        }
    } catch ( ... ) {
        QVERIFY2( false, "Uncaught exception." );
    }

}

QTEST_APPLESS_MAIN( SumTests )

#include "tst_SumTests.moc"
