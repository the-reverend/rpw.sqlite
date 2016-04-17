#pragma once

#include "SqliteDatabase.h"
#include "SqliteException.h"
#include "sqlite3.h"
#include "CriticalSection.h"

#include <string>
#include <map>

namespace RPW
{
	namespace SQLite
	{
		class SqliteStatement
		{
		public:
			SqliteStatement( void );
			SqliteStatement( SqliteDatabase& db );
			SqliteStatement( SqliteDatabase& db, const char* const szSql );
			~SqliteStatement( void );

			SqliteStatement( const SqliteStatement& copy );
			SqliteStatement& operator=( const SqliteStatement& rhs );
			bool operator<( const SqliteStatement& rhs ) const { return m_pStmt < rhs.m_pStmt; }

			SqliteStatement& prepare( const char* const szSql );
			SqliteStatement& bind_text( int nParam, const char* const text, int len );
			SqliteStatement& bind_text( int nParam, const std::string& text );
			SqliteStatement& bind_int( int nParam, int number );
			SqliteStatement& bind_int( int nParam, unsigned long number );
			SqliteStatement& bind_int64( int nParam, sqlite3_int64 number );
			SqliteStatement& reset();
			SqliteStatement& step( bool& done );
			SqliteStatement& step();
			SqliteStatement& step_all();
			SqliteStatement& finalize( bool force = false );

			SqliteStatement& forgiveErrorCode( int errorCode );
			SqliteStatement& clearForgiveness();

			SqliteStatement& enter();
			SqliteStatement& leave();

			int get_column_count();
			SqliteStatement& get_column_count( int& count );
			std::string get_column_text( int zIndex );
			SqliteStatement& get_column_text( int zIndex, std::string& text );
			int get_column_int( int zIndex );
			SqliteStatement& get_column_int( int zIndex, int& i );
			sqlite3_stmt* get_statement_handle();

		protected:
			void handle_return_code( int rc );

			SqliteDatabase* m_pDb;
			sqlite3_stmt* m_pStmt;
			RPW::Core::CriticalSection m_cs;

			std::map<int, bool> m_mapErrorCodesToForgive;

			bool m_bNeedsFinalized;
			std::string m_sql;
		};
	}
}
