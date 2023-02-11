
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Record.h"
#include "AVLTree.h"
#include "FlightDb.h"

// Functions
static void testDbFindNextFlight(void);
static void testNullDbFindNextFlight(void);
static void doTestDbFindNextFlight(FlightDb db, char *departureAirport, int day, int hour, int minute);
static FlightDb createSampleDb(void);
static FlightDb createSampleNullDb(void);
static void showRecordList(List l);
static char *dayToName(int day);
FlightDb getFlightDb_test1(void);


// TODO: Add your tests to this file 

int main(int argc, char *argv[]) {

    FlightDb db = getFlightDb_test1();
    testDbFindNextFlight();
    testNullDbFindNextFlight();

	(void) db; // avoid unused variable error

}

////////////////////////////////////////////////////////////////////////
// Trying to find flight from airport that doesnt exist in database
static void testDbFindNextFlight(void) {
    printf("Testing DbFindNextFlight\n");
    printf("Using sample DB from testAss1.c\n\n");
    
    FlightDb db = createSampleDb();
    
    doTestDbFindNextFlight(db, "Paris", 0,  7, 5);
    
    DbFree(db);
}
// Trying to find flight in null database
static void testNullDbFindNextFlight(void) {
    printf("Testing DbFindNextFlight\n");
    printf("Using sample NULL db\n\n");
    
    FlightDb db = createSampleNullDb();
    
    doTestDbFindNextFlight(db, "Paris", 0,  7, 5);
    
    DbFree(db);
}


static void doTestDbFindNextFlight(FlightDb db, char *departureAirport,
                                   int day, int hour, int minute) {
    printf("Finding first available flight at %s from %s %02d%02d\n",
           departureAirport, dayToName(day), hour, minute);
    
    Record r = DbFindNextFlight(db, departureAirport, day, hour, minute);
    
    if (r == NULL) {
        printf("No flights found\n");
    } else {
        printf("Found flight:\n");
        RecordShow(r);
        printf("\n");
    }
    
    printf("\n");
}

// NORMAL DATABASE
static FlightDb createSampleDb(void) {
    FlightDb db = DbNew();

    // QF409 from SYD to MEL
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 0,  7,  5,  90));
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 1,  7,  5,  90));
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 2,  7,  5,  90));
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 3,  7,  5,  90));
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 4,  7,  5,  90));
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 5,  7,  5,  90));
    DbInsertRecord(db, RecordNew("QF409", "SYD", "MEL", 6,  7,  5,  90));
    
    return db;
}

// NULL DATABASE
static FlightDb createSampleNullDb(void) {
    FlightDb db = DbNew();
    return db;
}










////////////////////////////////////////////////////////////////////////////////
// RANDOM STUFF
static void showRecordList(List l) {
    ListIterator it = ListItNew(l);
    while (ListItHasNext(it)) {
        RecordShow(ListItNext(it));
        printf("\n");
    }
    ListItFree(it);
}

static char *dayToName(int day) {
    assert(day >= 0 && day <= 6);
    
    char *days[] = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
        "Saturday", "Sunday"
    };
    
    return days[day];
}

/*
 * The following template is just a partial example to quickly 
 * create FlightDb for your testing.
*/
FlightDb getFlightDb_test1() {

    bool result = false;

    FlightDb fldb = DbNew();

    // Daily flights QF409 at 07:05 from SYD to MEL
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 0, 7, 05, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 1, 7, 05, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 2, 7, 05, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 3, 7, 05, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 4, 7, 05, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 5, 7, 05, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 6, 7, 05, 90));

    // Three flights QF419 at 08:00 from SYD to MEL
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 0, 8, 00, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 2, 8, 00, 90));
    result = DbInsertRecord(fldb, RecordNew("QF409", "SYD", "MEL", 5, 8, 00, 90));

    // Flights VA323 from MEL to BNE (Brisbane)
    result = DbInsertRecord(fldb, RecordNew("VA323", "MEL", "BNE", 1, 11, 00, 110));
    result = DbInsertRecord(fldb, RecordNew("VA323", "MEL", "BNE", 3, 11, 30, 110));
    result = DbInsertRecord(fldb, RecordNew("VA323", "MEL", "BNE", 4, 11, 00, 110));
    result = DbInsertRecord(fldb, RecordNew("VA323", "MEL", "BNE", 6, 11, 30, 110));

    (void) result;
    // add more flights ...
    

    return fldb;
}