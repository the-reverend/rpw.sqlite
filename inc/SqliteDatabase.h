#pragma once

#include "SqliteException.h"
#include "sqlite3.h"
#include "CriticalSection.h"

#include <map>
#include <string>

namespace RPW
{
	namespace SQLite
	{

		class SqliteStatement;

		typedef std::map<std::string, RPW::SQLite::SqliteStatement> StatementMap;

		class SqliteDatabase
		{
		public:
			SqliteDatabase( void );
			virtual ~SqliteDatabase( void );

			int open( const char* const dbName );
			void close( void );

			bool isOpen( void );
			const char* errmsg( void );
			int prepare( const char* const szSql, sqlite3_stmt **ppStmt );

			RPW::SQLite::SqliteStatement& getPreparedStatement( const std::string& sql );

			void beginTransaction( void );
			void commit( void );
			bool inTransaction( void );

		protected:
			void throwIfClosed( void );

			sqlite3* m_pDb;

			StatementMap m_mapPreparedStatements;
			bool m_inTransaction;
			RPW::Core::CriticalSection m_cs;
		};

	}
}
