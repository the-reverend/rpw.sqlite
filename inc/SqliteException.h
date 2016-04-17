/// @file SqliteException.h
/// @author rev

#pragma once

#include "sqlite3.h"

#include <stdexcept>
#include <sstream>

namespace RPW
{
	namespace SQLite
	{

/// @brief exception type for unexpected case errors
		class SqliteException : public std::runtime_error
		{
		public:
			SqliteException() : std::runtime_error( "sqlite error" ) {}

			SqliteException( int returnCode, const char* const errorMsg, sqlite3_stmt* stmt = NULL )
					: std::runtime_error( toString( returnCode, errorMsg ).c_str() )
					, number( returnCode )
					, message( errorMsg )
			{
				if ( stmt )
					sqlite3_finalize( stmt );
			}

			SqliteException( int returnCode, const char* const source, const char* const errorMsg, sqlite3_stmt* stmt = NULL )
					: std::runtime_error( toString( returnCode, source, errorMsg ).c_str() )
					, number( returnCode )
					, source( source )
					, message( errorMsg )
			{
				if ( stmt )
					sqlite3_finalize( stmt );
			}

			int getNumber() { return number; }
			std::string getSource() { return source; }
			std::string getMessage() { return message; }

		protected:
			static std::string toString( int returnCode, const char* const errorMsg )
			{
				std::ostringstream oss;
				oss << "sqlite error #" << returnCode << " : " << errorMsg;
				return oss.str();
			}

			static std::string toString( int returnCode, const char* const source, const char* const errorMsg )
			{
				std::ostringstream oss;
				oss << source << " - sqlite error #" << returnCode << " : " << errorMsg;
				return oss.str();
			}

			int number;
			std::string source;
			std::string message;
		};

	}
}
