#include "sqlite3.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
using namespace std;

class SDB
{
public:
	struct Null {};
	static Null null()
	{
		return Null();
	}

	struct Blob
	{
		Blob( const void *data = NULL, int len = 0 )
				: m_data( data )
				, m_len( len )
		{}
		const void* m_data;
		int m_len;
	};


	SDB( const char *fname )
	{
		check( sqlite3_open( fname, &sdb ) );
	}

	~SDB()
	{
		check( sqlite3_close( sdb ) );
	}

	class Statement
	{
	public:
		Statement( SDB &db, const char *query = NULL )
				: sdb( db ), sqlst( NULL )
		{
			prepare( query );
		}

		~Statement()
		{
			if ( sqlst )
				check( sqlite3_finalize( sqlst ) );
		}

		int prepare( const char *query )
		{
			if ( !query ) return SQLITE_ERROR;
			int ok = sqlite3_prepare_v2( sdb.sdb, query, strlen( query ), &sqlst, NULL );
			check( ok, query );
			return ok;
		}

		int step()
		{
			int ok = sqlite3_step( sqlst );
			check( ok );
			return ok;
		}

		int exec()
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			return step();
		}

		template<typename T1>
		int exec( T1 v1 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			return step();
		}

		template<typename T1, typename T2>
		int exec( T1 v1, T2 v2 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			return step();
		}

		template<typename T1, typename T2, typename T3>
		int exec( T1 v1, T2 v2, T3 v3 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4, T5 v5 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			bind( 5, v5 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok; bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			bind( 5, v5 );
			bind( 6, v6 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			bind( 5, v5 );
			bind( 6, v6 );
			bind( 7, v7 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			bind( 5, v5 );
			bind( 6, v6 );
			bind( 7, v7 );
			bind( 8, v8 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			bind( 5, v5 );
			bind( 6, v6 );
			bind( 7, v7 );
			bind( 8, v8 );
			bind( 9, v9 );
			return step();
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		int exec( T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10 )
		{
			int ok = reset();
			if ( ok != SQLITE_OK )
				return ok;
			bind( 1, v1 );
			bind( 2, v2 );
			bind( 3, v3 );
			bind( 4, v4 );
			bind( 5, v5 );
			bind( 6, v6 );
			bind( 7, v7 );
			bind( 8, v8 );
			bind( 9, v9 );
			bind( 10, v10 );
			return step();
		}

		Null getNull( int i = 0 )
		{
			return null();
		}

		Blob getBlob( int i = 0 )
		{
			return Blob( sqlite3_column_blob( sqlst, i ), sqlite3_column_bytes( sqlst, i ) );
		}

		int getInt( int i = 0 )
		{
			return sqlite3_column_int( sqlst, i );
		}

		sqlite3_int64 getInt64( int i = 0 )
		{
			return sqlite3_column_int64( sqlst, i );
		}

		double getDouble( int i = 0 )
		{
			return sqlite3_column_double( sqlst, i );
		}

		const char *getText( int i = 0 )
		{
			return ( const char * )sqlite3_column_text( sqlst, i );
		}

	private:
		SDB& sdb;
		sqlite3_stmt *sqlst;

		void check( int ok, const char *stmt = NULL )
		{
			sdb.check( ok, stmt );
		}

		int reset()
		{
			int ok = sqlite3_reset( sqlst );
			check( ok );
			return ok;
		}

		void bind( int i, const Null& null )
		{
			check( sqlite3_bind_null( sqlst, i ) );
		}

		void bind( int i, const Blob& blob )
		{
			if ( blob.m_data )
				check( sqlite3_bind_blob( sqlst, i, blob.m_data, blob.m_len, SQLITE_TRANSIENT ) );
			else
				check( sqlite3_bind_zeroblob( sqlst, i, blob.m_len ) );
		}

		void bind( int i, int v )
		{
			check( sqlite3_bind_int( sqlst, i, v ) );
		}

		void bind( int i, sqlite3_int64 v )
		{
			check( sqlite3_bind_int64( sqlst, i, v ) );
		}

		void bind( int i, double v )
		{
			check( sqlite3_bind_double( sqlst, i, v ) );
		}

		void bind( int i, const string& v )
		{
			check( sqlite3_bind_text( sqlst, i, v.c_str(), -1, SQLITE_TRANSIENT ) );
		}

		void bind( int i, const char *v, int n = -1 )
		{
			check( sqlite3_bind_text( sqlst, i, v, n, n == -1 ? SQLITE_TRANSIENT : SQLITE_STATIC ) );
		}
	};

	int exec( const char *query )
	{
		return Statement( *this, query ).exec();
	}

	string execStr( const char *query )
	{
		Statement stmt( *this, query );
		stmt.exec();
		const char *rc = stmt.getText( 0 );
		return rc ? rc : "";
	}

private:
	sqlite3 *sdb;
	void check( int ok, const char *stmt = NULL )
	{
		if ( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW )
		{
			const char *msg = sqlite3_errmsg( sdb );
			if ( strcmp( "not an error", msg ) )
			{
				if ( stmt )
					fprintf( stderr, "sqlite-check(%d:%s) \"%s\"\n", ok, msg, stmt );
				else
					fprintf( stderr, "sqlite-check(%d:%s)\n", ok, msg );
			}
		}
	}
};

int main( void )
{
	SDB sdb( "test.sdb" );
	sdb.exec( "drop table test" );
	sdb.exec( "create table test (x, y, z)" );
	sdb.exec( "create index testXIdx on test (x)" );
	sdb.exec( "create index testYIdx on test (y)" );
	sdb.exec( "create index testZIdx on test (z)" );
	SDB::Statement insert( sdb, "insert into test values(?, ?, ?)" );
	insert.exec( "hello", 42, "world" );
	insert.exec( "hello", "world", 42 );
	char *buf = ( char * )malloc( 1024 * 1024 );
	memset( buf, 0, 1024*1024 );
	insert.exec( "hello", SDB::null(), SDB::Blob( buf, 1024*1024 ) );
	free( buf );
	SDB::Statement select( sdb, "select x, y, z from test where x = ? order by x, y, z" );
	if ( select.exec( "hello" ) == SQLITE_ROW )
	{
		do
		{
			const char *x = select.getText( 0 );
			int y = select.getInt( 1 );
			const char *y2 = select.getText( 1 );
			SDB::Blob z = select.getBlob( 2 );
			const char *z2 = select.getText( 2 );
			printf( "x=%s, y=%d(%s), z=0x%08lx/%d(%s)\n", x, y, y2, z.m_data, z.m_len, z2 );
		}
		while ( select.step() == SQLITE_ROW );
	}
}
