#ifndef ARCHIVABLE_H
#define ARCHIVABLE_H

/**
 * @file Archivable.h
 * Interface description of the abstract class Archivable
 *
 * @author Yi Qiao
 */

/*
The MIT License (MIT)

Copyright (c) 2013 Yi Qiao

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <string>
#include <vector>
#include <sqlite3/sqlite3.h>


namespace SubcloneSeeker {

	/**
	 * a vector of database object ids, in the type of sqlite3_int64
	 */
	typedef std::vector<sqlite3_int64> DBObjectID_vec;

	/**
	 * @interface Archivable
	 * @brief Abstract class that defines the interface to handle archiving objects into sqlite3 database
	 * 
	 * This abstract class defines the required behaviors when handling object archiving to and from a
	 * sqlite3 database, which will be used by the project to store computation results. As of now four
	 * methods are required for any class that wishes to support archiving:
	 *   1. return a string describing the name of the table
	 *   2. create the table in a given database
	 *   3. store an object into the table
	 *   4. retrieve an object from the table with an identifier
	 *
	 * The unarchiving procedure uses an integer id to determine which database record is to be used for
	 * unarchiving. This would require that an SERIAL column exists in the table.
	 */
	class Archivable {
		protected:

			sqlite3_int64 id;  /**< the database identifier for a specific record */

		protected:

			/**
			 * returns the name of the table in which all object of a specific class are stored
			 * @return Table name
			 */
			virtual std::string getTableName() = 0;

			/**
			 * return the table definition when creating the table in a sqlite3 database
			 *
			 * Imagine a generic SQL create statement: CREATE TABLE <tableName> (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT <other-columns>);
			 * Notice that the returned string must be prefixed with a comma ",", to separate any custom columns from the id column
			 *
			 * @return The extra column definitions, prefixed with ","
			 */
			virtual std::string createTableStatementStr() = 0;

			/**
			 * return the unbound statement for record creation
			 * @return unbound statement for record creation
			 */
			virtual std::string createObjectStatementStr() = 0;

			/**
			 * return the unbound statement for record update
			 * @return unbound statement for record update
			 */
			virtual std::string updateObjectStatementStr() = 0;

			/**
			 * return the list of columns to be used in select statement 
			 * for getting data from the database
			 *
			 * Imagine the generic SQL select statement: SELECT <col1>, <col2>, ... FROM <tableName> WHERE id=?;
			 * The method should return the <col1>,<col2>,... part
			 *
			 * @return the list of columns as string
			 */
			virtual std::string selectObjectColumnListStr() = 0;

			/**
			 * Bind archivable properties to a prepared, unbound sqlite3 statement
			 *
			 * @param statement A prepared, unbound sqlite3 statement instance
			 * @return How many parameters are bound to the statement + 1
			 */
			virtual int bindObjectToStatement(sqlite3_stmt *statement) = 0;

			/**
			 * Populate archivable properties from a prepared statement during unarchiving
			 *
			 * @param statement A prepared statement contains the retrieved row
			 */
			virtual void updateObjectFromStatement(sqlite3_stmt *statement) = 0;

		public:
			/**
			 * Minimal constructor to reset all member variables
			 */
			Archivable() : id(0) {;}

			/**
			 * get access function of id
			 * @return the database identifier of the object
			 */
			inline sqlite3_int64 getId() {return id;}

			/**
			 * Set the database id
			 * @param nid the new database id
			 */
			inline void setId(sqlite3_int64 nid) {id = nid;}


			/**
			 * Create the storage table in the database
			 * @param database An open sqlite3 database connection handle
			 * @return Whether the operation is successful or not
			 */
			bool createTableInDB(sqlite3 *database);

			/**
			 * Archive the object into the database
			 * @param database An open sqlite3 database connection handle
			 * @return The id of the newly inserted object, if successful; or -1 if error occurred
			 */
			sqlite3_int64 archiveObjectToDB(sqlite3 *database);

			/**
			 * Unarchive an object from the database
			 * @param database An open sqlite3 database connection handle
			 * @param id The identifier with which the correct record is to be found
			 * @return Whether the operation is successful or not
			 */
			bool unarchiveObjectFromDB(sqlite3 *database, sqlite3_int64 id);

			/**
			 * Return a std::vector of all ids of records of the current object's class
			 *
			 * @return A vector of sqlite3_int64, describing all records with the same class
			 */
			DBObjectID_vec vecAllObjectsID(sqlite3 *database);

	};
}

#endif
