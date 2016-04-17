#include "StdAfx.h"
#include "SqliteDatabase.h"

#include "NullArgumentException.h"
#include "SqliteStatement.h"

RPW::SQLite::SqliteDatabase::SqliteDatabase( void )
		: m_pDb( NULL )
		, m_inTransaction( false )
{
}

RPW::SQLite::SqliteDatabase::~SqliteDatabase( void )
{
	close();
}

int RPW::SQLite::SqliteDatabase::open( const char* const dbName )
{
	RPW::Core::CriticalSection::Scope scope( m_cs );
	close();

	int rc = sqlite3_open_v2(
	            dbName,
	            &m_pDb,
	            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
	            NULL );

	if ( SQLITE_OK == rc )
	{
		// enable the extended result codes feature of sqlite
		rc = sqlite3_extended_result_codes( m_pDb, 1 );
	}

	return rc;
}

void RPW::SQLite::SqliteDatabase::close( void )
{
	RPW::Core::CriticalSection::Scope scope( m_cs );
	m_mapPreparedStatements.clear();

	// close the database
	if ( m_pDb != NULL )
	{
		sqlite3_close( m_pDb );
		m_pDb = NULL;
	}
}

bool RPW::SQLite::SqliteDatabase::isOpen( void )
{
	return ( m_pDb != NULL );
}

void RPW::SQLite::SqliteDatabase::throwIfClosed( void )
{
	if ( !isOpen() )
	{
		throw new RPW::Core::NullArgumentException();
	}
}

int RPW::SQLite::SqliteDatabase::prepare( char const*const szSql, sqlite3_stmt **ppStmt )
{
	RPW::Core::CriticalSection::Scope scope( m_cs );
	throwIfClosed();
	return sqlite3_prepare_v2( m_pDb, szSql, -1, ppStmt, 0 );
}

char const* RPW::SQLite::SqliteDatabase::errmsg( void )
{
	RPW::Core::CriticalSection::Scope scope( m_cs );
	throwIfClosed();
	return sqlite3_errmsg( m_pDb );
}

RPW::SQLite::SqliteStatement& RPW::SQLite::SqliteDatabase::getPreparedStatement( const std::string& sql )
{
	RPW::Core::CriticalSection::Scope scope( m_cs );
	if ( m_mapPreparedStatements.find( sql ) == m_mapPreparedStatements.end() )
	{
		m_mapPreparedStatements.insert( std::make_pair( sql, RPW::SQLite::SqliteStatement( *this, sql.c_str() ) ) );
	}

	return m_mapPreparedStatements[sql];
}

void RPW::SQLite::SqliteDatabase::beginTransaction( void )
{
	if ( !m_inTransaction )
	{
		RPW::Core::CriticalSection::Scope scope( m_cs );
		m_inTransaction = true;
		RPW::SQLite::SqliteStatement t( *this );
		t.prepare( "BEGIN TRANSACTION" ).step_all();
	}
	else
	{
		throw std::logic_error( "cannot enter a transaction that already exists" );
	}
}

void RPW::SQLite::SqliteDatabase::commit( void )
{
	if ( m_inTransaction )
	{
		RPW::Core::CriticalSection::Scope scope( m_cs );
		RPW::SQLite::SqliteStatement t( *this );
		t.prepare( "COMMIT" ).step_all();
		m_inTransaction = false;
	}
	else
	{
		throw std::logic_error( "cannot commit a transaction that doesn't exist" );
	}
}

bool RPW::SQLite::SqliteDatabase::inTransaction( void )
{
	return m_inTransaction;
}