#include "StdAfx.h"
#include "SqliteStatement.h"

#include <limits.h>

#include "NullArgumentException.h"

RPW::SQLite::SqliteStatement::SqliteStatement( void )
		: m_pDb( NULL )
		, m_pStmt( NULL )
		, m_bNeedsFinalized( false )
{
}

RPW::SQLite::SqliteStatement::SqliteStatement( SqliteDatabase& db )
		: m_pDb( &db )
		, m_pStmt( NULL )
		, m_bNeedsFinalized( false )
{
}

RPW::SQLite::SqliteStatement::SqliteStatement( SqliteDatabase& db, const char* const szSql )
		: m_pDb( &db )
		, m_pStmt( NULL )
		, m_bNeedsFinalized( false )
{
	prepare( szSql );
}

RPW::SQLite::SqliteStatement::~SqliteStatement( void )
{
	finalize();
}

RPW::SQLite::SqliteStatement::SqliteStatement( const RPW::SQLite::SqliteStatement& copy )
		: m_pDb( copy.m_pDb )
		, m_pStmt( NULL )
		, m_bNeedsFinalized( false )
{
	m_mapErrorCodesToForgive.insert( copy.m_mapErrorCodesToForgive.begin(), copy.m_mapErrorCodesToForgive.end() );

	if ( copy.m_bNeedsFinalized )
	{
		prepare( copy.m_sql.c_str() );
	}
	else
	{
		m_sql = copy.m_sql;
	}
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::operator=( const RPW::SQLite::SqliteStatement & rhs )
{
	finalize();

	m_pDb = rhs.m_pDb;
	m_mapErrorCodesToForgive.clear();
	m_mapErrorCodesToForgive.insert( rhs.m_mapErrorCodesToForgive.begin(), rhs.m_mapErrorCodesToForgive.end() );

	if ( rhs.m_bNeedsFinalized )
	{
		prepare( rhs.m_sql.c_str() );
	}
	else
	{
		m_sql = rhs.m_sql;
	}

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::finalize( bool force /* = false */ )
{
	if ( m_pStmt && ( force || m_bNeedsFinalized ) )
	{
		int rc = sqlite3_finalize( m_pStmt );

		m_bNeedsFinalized = false;
		m_pStmt = NULL;

		handle_return_code( rc );
	}

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::prepare( const char* const szSql )
{
	finalize();

	int rc = m_pDb->prepare( szSql, &m_pStmt );
	m_sql = szSql;

	m_bNeedsFinalized = true;

	handle_return_code( rc );

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::reset()
{
	handle_return_code( sqlite3_reset( m_pStmt ) );

	return *this;
}

void RPW::SQLite::SqliteStatement::handle_return_code( int rc )
{
	switch ( rc )
	{
		case SQLITE_OK:   // Successful result
		case SQLITE_ROW:  // sqlite3_step() has another row ready
		case SQLITE_DONE: // sqlite3_step() has finished executing
			break;

		case SQLITE_ERROR:       // SQL error or missing database
		case SQLITE_INTERNAL:    // Internal logic error in SQLite
		case SQLITE_PERM:        // Access permission denied
		case SQLITE_ABORT:       // Callback routine requested an abort
		case SQLITE_BUSY:        // The database file is locked
		case SQLITE_LOCKED:      // A table in the database is locked
		case SQLITE_NOMEM:       // A malloc() failed
		case SQLITE_READONLY:    // Attempt to write a readonly database
		case SQLITE_INTERRUPT:   // Operation terminated by sqlite3_interrupt()
		case SQLITE_IOERR:       // Some kind of disk I/O error occurred
		case SQLITE_IOERR_READ:       // extended error codes
		case SQLITE_IOERR_SHORT_READ:
		case SQLITE_IOERR_WRITE:
		case SQLITE_IOERR_FSYNC:
		case SQLITE_IOERR_DIR_FSYNC:
		case SQLITE_IOERR_TRUNCATE:
		case SQLITE_IOERR_FSTAT:
		case SQLITE_IOERR_UNLOCK:
		case SQLITE_IOERR_RDLOCK:
		case SQLITE_IOERR_DELETE:
		case SQLITE_IOERR_BLOCKED:
		case SQLITE_IOERR_NOMEM:
		case SQLITE_IOERR_ACCESS:
		case SQLITE_IOERR_CHECKRESERVEDLOCK:
		case SQLITE_IOERR_LOCK:
		case SQLITE_IOERR_CLOSE:
		case SQLITE_IOERR_DIR_CLOSE:
		case SQLITE_CORRUPT:     // The database disk image is malformed
		case SQLITE_NOTFOUND:    // NOT USED. Table or record not found
		case SQLITE_FULL:        // Insertion failed because database is full
		case SQLITE_CANTOPEN:    // Unable to open the database file
		case SQLITE_PROTOCOL:    // NOT USED. Database lock protocol error
		case SQLITE_EMPTY:       // Database is empty
		case SQLITE_SCHEMA:      // The database schema changed
		case SQLITE_TOOBIG:      // String or BLOB exceeds size limit
		case SQLITE_CONSTRAINT:  // Abort due to constraint violation
		case SQLITE_MISMATCH:    // Data type mismatch
		case SQLITE_MISUSE:      // Library used incorrectly
		case SQLITE_NOLFS:       // Uses OS features not supported on host
		case SQLITE_AUTH:        // Authorization denied
		case SQLITE_FORMAT:      // Auxiliary database format error
		case SQLITE_RANGE:       // 2nd parameter to sqlite3_bind out of range
		case SQLITE_NOTADB:      // File opened that is not a database file
		default:
		{
			std::map<int, bool>::iterator it = m_mapErrorCodesToForgive.find( rc );
			if ( it == m_mapErrorCodesToForgive.end() )
			{
				throw SqliteException( rc, m_pDb->errmsg() );
			}
			else if ( !it->second )
			{
				throw SqliteException( rc, m_pDb->errmsg() );
			}
		}
	}
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::bind_text( int nParam, const char* const text, int len )
{
	handle_return_code( sqlite3_bind_text( m_pStmt, nParam, text, len, SQLITE_STATIC ) );

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::bind_text( int nParam, const std::string& text )
{
	return bind_text( nParam, text.c_str(), static_cast<int>( text.length() ) );
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::bind_int( int nParam, int number )
{
	handle_return_code( sqlite3_bind_int( m_pStmt, nParam, number ) );

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::bind_int( int nParam, unsigned long number )
{
	if ( number > INT_MAX )
	{
		throw std::range_error( "cannot bind large unsigned value to sqlite signed integer" );
	}

	return bind_int( nParam, static_cast<int>( number ) );
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::bind_int64( int nParam, sqlite3_int64 number )
{
	handle_return_code( sqlite3_bind_int64( m_pStmt, nParam, number ) );

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::step()
{
	bool done = false;

	return step( done );
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::step( bool& done )
{
	int rc = sqlite3_step( m_pStmt );
	switch ( rc )
	{
		case SQLITE_DONE:
			done = true;
			break;

		case SQLITE_ROW:
			done = false;
			break;

		default:
			done = true;
			handle_return_code( rc );
			break;
	}

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::step_all()
{
	bool done = false;
	while ( !done )
	{
		step( done );
	}

	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::get_column_count( int& count )
{
	count = get_column_count();
	return *this;
}

int RPW::SQLite::SqliteStatement::get_column_count()
{
	return sqlite3_column_count( m_pStmt );
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::get_column_text( int zIndex, std::string& text )
{
	text = get_column_text( zIndex );
	return *this;
}

std::string RPW::SQLite::SqliteStatement::get_column_text( int zIndex )
{
	const char* const txt = ( const char * ) sqlite3_column_text( m_pStmt, zIndex );

	return std::string( txt ? txt : "" );
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::get_column_int( int zIndex, int& i )
{
	i = get_column_int( zIndex );
	return *this;
}

int RPW::SQLite::SqliteStatement::get_column_int( int zIndex )
{
	return sqlite3_column_int( m_pStmt, zIndex );
}

sqlite3_stmt* RPW::SQLite::SqliteStatement::get_statement_handle()
{
	return m_pStmt;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::forgiveErrorCode( int errorCode )
{
	m_mapErrorCodesToForgive[errorCode] = true;
	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::clearForgiveness()
{
	m_mapErrorCodesToForgive.clear();
	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::enter()
{
	m_cs.enter();
	return *this;
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteStatement::leave()
{
	m_cs.leave();
	return *this;
}

