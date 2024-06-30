#include <stdio.h>
#include <string.h>

// Frame structure
typedef struct 
{
    char name[2];
    int currentPage;
    int time;
    int chance;
} Frame;

// Array of frames
#define NUM_FRAMES 4
static Frame frames[NUM_FRAMES];

// Array of integers representing page requests
static int requests[] = {4,1,7,10,8,4,8,7,1,4,7,10,4,8,4,1,7,10};
#define NUM_REQUESTS (sizeof(requests) / sizeof(int))

static int clock = 0;
                         
// Debug function to print the frames
static void printFrames(int pageRequested ) {
	printf( "========== REQUEST: %d\n", pageRequested );
    for ( int i = 0; i < NUM_FRAMES; i++ ) {
        printf("%s   p=%2d   t=%d    chans:%d\n", frames[i].name, frames[i].currentPage, frames[i].time, frames[i].chance );
    }
}

// returns a empty index of frame for page insertion or -1 if not found
static int findSpace() {
    for ( int i = 0; i < NUM_FRAMES; i++ ) {
        if ( frames[i].currentPage == -1 ) {
            return i;
        }
    }
    return -1;
}

// checks if the given page is a hit in the frames
static int checkHit(int page) {
    for ( int i = 0; i < NUM_FRAMES; i++ ) {
        if ( frames[i].currentPage == page ) {
            return i;
        }
    }
    return -1;
}

// loads a page into given frame, assuming everything is allowed
static void loadPage(int page, int indexOfFrameToLoadInto) {
    frames[indexOfFrameToLoadInto].currentPage = page;
    clock++;
    frames[indexOfFrameToLoadInto].time = clock;
}

static int lowestClock() {
    int lowest = frames[0].time;
    int lowestIndex = 0;
    for ( int i = 1; i < NUM_FRAMES; i++ ) {
        if (frames[i].time < lowest) {
            lowest = frames[i].time;
            lowestIndex = i;
        }
    }
    return lowestIndex;
}

// FIRST IN FIRST OUT ALGORITHM
static void fifo(int pageRequested) {
    // check hit if page is already loaded
    int frameIndex = checkHit(pageRequested);
    if ( frameIndex >= 0 ) {
        printf("%d   H   %s\n", pageRequested, frames[frameIndex].name);
        return;
    } 
    // page is not loaded, find space
    frameIndex = findSpace();
    // if space is found, load page into found space
    if ( frameIndex >= 0 ) {
        loadPage(pageRequested, frameIndex);
        printf("%d   F   %s\n", pageRequested, frames[frameIndex].name);
        return;
    }
    // space is not found, need eviction
    frameIndex = lowestClock();
    loadPage(pageRequested, frameIndex);
    printf("%d   F   %s\n", pageRequested, frames[frameIndex].name);
    return;
}

// LEAST RECENTLY USED ALGORITHM
static void lru(int pageRequested) {
    // check hit if page is already loaded
    int frameIndex = checkHit(pageRequested);
    if ( frameIndex >= 0 ) {
        frames[frameIndex].time = clock; // LRU
        printf("%d   H   %s\n", pageRequested, frames[frameIndex].name);
        return;
    } 
    // page is not loaded, find space
    frameIndex = findSpace();
    // if space is found, load page into found space
    if ( frameIndex >= 0 ) {
        loadPage(pageRequested, frameIndex);
        printf("%d   F   %s\n", pageRequested, frames[frameIndex].name);
        return;
    }
    // space is not found, need eviction
    frameIndex = lowestClock();
    loadPage(pageRequested, frameIndex);
    printf("%d   F   %s\n", pageRequested, frames[frameIndex].name);
    return;
}

// SECOND CHANCE ALGORITHM
static void secondChance(int pageRequested) {
	
    // check hit if page is already loaded
    int frameIndex = checkHit(pageRequested);
    if ( frameIndex >= 0 ) {
        frames[frameIndex].chance = 1;
        printf("%2d   H   %s\n", pageRequested, frames[frameIndex].name);
        return;
    } 
    // page is not loaded, find space
    frameIndex = findSpace();
    // if space is found, load page into found space
    if ( frameIndex >= 0 ) {
        loadPage(pageRequested, frameIndex);
        printf("%2d   F   %s  LOADED TO FREE FRAME\n", pageRequested, frames[frameIndex].name);
        return;
    }
 
    // space is not found, need eviction
    while(1)
    {
        frameIndex = lowestClock();
        //printf( "		oldest frame %s\n", frames[frameIndex].name );
        if( frames[frameIndex].chance == 1 )
        {
            clock++;
            frames[frameIndex].time = clock;    //  mark as newest
            frames[frameIndex].chance = 0;
            // second oldest is already the oldest now
            //printf( "		chance SET, so mark frame %s as newest\n", frames[frameIndex].name );
        }
        else
        {
            loadPage(pageRequested, frameIndex);
            printf("%2d   F   %s\n", pageRequested, frames[frameIndex].name);
            break;
        }
    }
}

// --------------------------------------
void main(void) {
    // initialize frames
    for ( int i = 0; i < NUM_FRAMES; i++ ) {
        clock++;
        frames[i].name[0] = 'A' + i;
        frames[i].name[1] = '\0';
        frames[i].currentPage = -1;
        frames[i].time = clock;
        frames[i].chance = 0;
    }
    
    // process requests
    for ( int i = 0; i < NUM_REQUESTS; i++ ) {
        clock++;
        //printFrames(requests[i]);

        // Select which function to call
        //fifo(requests[i]);
        //lru(requests[i]);
        secondChance(requests[i]);
    }

    //printFrames();
}