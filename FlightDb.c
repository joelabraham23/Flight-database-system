
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "FlightDb.h"
#include "AVLTree.h"

struct flightDb {
	Tree byFlightNumber;
	Tree byAirportDepartureDay;
	Tree byDepartureTime;
};
int compareByFlightNumber(Record r1, Record r2);
int compareByAirportDepartureDay(Record r1, Record r2);
int compareByDepartureTime(Record r1, Record r2);
int compareByTime(Record r1, Record r2);

/**
 * Creates a new flight DB. 
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
FlightDb DbNew(void) {
	FlightDb db = malloc(sizeof(*db));
	db->byFlightNumber = TreeNew(compareByFlightNumber);
	db->byAirportDepartureDay = TreeNew(compareByAirportDepartureDay);
	db->byDepartureTime = TreeNew(compareByDepartureTime);
	return db;
}

/**
 * Frees all memory allocated to the given flight DB
 */
void     DbFree(FlightDb db) {
	TreeFree(db->byFlightNumber, false);
	TreeFree(db->byAirportDepartureDay, false);
	TreeFree(db->byDepartureTime, true);
	free(db);
}

/**
 * Inserts  a  flight  record  into the given DB if there is not already
 * record with the same flight number, departure airport, day, hour  and
 * minute.
 * If  inserted successfully, this function takes ownership of the given 
 * record (so the caller should not modify or free it). 
 * Returns true if the record was successfully inserted,  and  false  if
 * the  DB  already  contained  a  record  with  the same flight number,
 * departure airport, day, hour and minute.
 * The time complexity of this function must be O(log n).
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
bool     DbInsertRecord(FlightDb db, Record r) {
	if (TreeInsert(db->byFlightNumber, r) && TreeInsert(db->byAirportDepartureDay, r) 
	&& TreeInsert(db->byDepartureTime, r)){
		return true;
	}
	return false;
}

/**
 * Searches  for  all  records with the given flight number, and returns
 * them all in a list in increasing order of  (day, hour, min).  Returns
 * an empty list if there are no such records. 
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List     DbFindByFlightNumber(FlightDb db, char *flightNumber) {
	// Creating lowest and highest possible flight variables with the same flight number
	Record low_dummy = RecordNew(flightNumber, "", "", 0, 0, 0, 0);
	Record max_dummy = RecordNew(flightNumber, "zzzzzzzz", "zzzzzzzz", 6, 23, 59,99999999);
	// Searching the tree between the lower and the maximum and appending to new_list
	List new_list = TreeSearchBetween(db->byFlightNumber, low_dummy, max_dummy);
	// Freeing Records
	RecordFree(low_dummy);
	RecordFree(max_dummy);
	return new_list;
}

/**
 * Searches  for all records with the given departure airport and day of
 * week (0 to 6), and returns them all in a list in increasing order  of
 * (hour, min, flight number).
 * Returns an empty list if there are no such records.
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List     DbFindByDepartureAirportDay(FlightDb db, char *departureAirport,
                                     int day) {
	// Creating lowest and highest possible flight variables with the same departing airport.
	Record low_dummy = RecordNew("", departureAirport, "", day, 0, 0, 0);
	Record max_dummy = RecordNew("zzzzzzzz", departureAirport, "zzzzzzzz", day, 23, 59, 99999999);
	// Searching the tree between the lower and the maximum and appending to new_list
	List new_list = TreeSearchBetween(db->byAirportDepartureDay, low_dummy, max_dummy);
	// Freeing records
	RecordFree(low_dummy);
	RecordFree(max_dummy);

	return new_list;
}


/**
 * Searches  for  all  records  between  (day1, hour1, min1)  and (day2,
 * hour2, min2), and returns them all in a list in increasing  order  of
 * (day, hour, min, flight number).
 * Returns an empty list if there are no such records.
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List     DbFindBetweenTimes(FlightDb db, 
                            int day1, int hour1, int min1, 
                            int day2, int hour2, int min2) {
	// Creating lowest and highest possible flight variables with different departing times
	Record low_dummy = RecordNew("", "", "", day1, hour1, min1, 0);
	Record max_dummy = RecordNew("zzzzzzzz", "zzzzzzzz", "zzzzzzzz", day2, hour2, min2, 99999999);
	// Appending to new_list all flights between the given times
	List new_list = TreeSearchBetween(db->byDepartureTime, low_dummy, max_dummy);
	// Freeing Records
	RecordFree(low_dummy);
	RecordFree(max_dummy);
	return new_list;
}

/**
 * Searches  for  and  returns  the  earliest next flight from the given
 * departure airport, on or after the given (day, hour, min).
 * The returned record must not be freed or modified. 
 * The time complexity of this function must be O(log n).
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
//  */
Record   DbFindNextFlight(FlightDb db, char *departureAirport, 
                          int day, int hour, int min) {
	// Creatinging a record with the given departureAirport and departing time
	Record dummy = RecordNew("", departureAirport, "", day, hour, min, 0);
	// Finding next record in tree
	Record next = TreeNext(db->byAirportDepartureDay, dummy);
	// Freeing dummy record
	RecordFree(dummy);
	// Find next record at start of week
	if (next == NULL) {
		Record dummyWrap = RecordNew("", departureAirport, "", 0, 0, 0, 0);
		Record nextDummy = TreeNext(db->byAirportDepartureDay, dummyWrap);
		RecordFree(dummyWrap);
		// Incase next flight is not from same departuring Airport
		if (nextDummy == NULL || strcmp(RecordGetDepartureAirport(nextDummy), departureAirport) != 0) {
			return NULL;
		}
		return nextDummy;
	}
	return next;
}

// Used for DbFindByFlightNumber
// Compares Flight number then time
int compareByFlightNumber(Record r1, Record r2) {
	int cmp_flightNum = strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
	if (cmp_flightNum == 0) {
		return compareByTime(r1, r2);
	}
	return cmp_flightNum;
}

// Used for DbFindByDepartureAirportDay and DbFindNextFLight
// Compares departing airport then time then Flight number
int compareByAirportDepartureDay(Record r1, Record r2) {
	int cmp_depAirport = strcmp(RecordGetDepartureAirport(r1), RecordGetDepartureAirport(r2));
	if (cmp_depAirport == 0) {
		return compareByDepartureTime(r1, r2);
	}
	return cmp_depAirport;
}

// Used for DbFindBetweenTimes
// Compares by time then flight number
int compareByDepartureTime(Record r1, Record r2) {
	int cmp_time = compareByTime(r1, r2);
	if (cmp_time == 0) {
		return strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
	}
	return cmp_time;
}
// Helper function made to compare time by day, hour then minute.
int compareByTime(Record r1, Record r2) {
	int cmp_day = RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
	if (cmp_day == 0) {
		int cmp_hour = RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
		if (cmp_hour == 0) {
			int cmp_min = RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
			return cmp_min;
		}
		return cmp_hour;
	}
	return cmp_day;
}
